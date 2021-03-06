/**
 * Copyright 2019-2020 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "session/inner_session.h"
#include <map>
#include <memory>
#include <vector>
#include "graph/load/new_model_manager/model_manager.h"
#include "graph/ge_global_options.h"
#include "graph/ge_local_context.h"
#include "graph/ge_context.h"
#include "framework/common/debug/ge_log.h"
#include "common/util.h"
#include "graph/manager/graph_var_manager.h"
#include "graph/utils/tensor_adapter.h"
#include "runtime/mem.h"

namespace ge {
static std::mutex mutex_;  // BuildGraph and RunGraph use

InnerSession::InnerSession(uint64_t session_id, const std::map<string, string> &options)
    : init_flag_(false), session_id_(session_id), options_(options) {}

Status InnerSession::Initialize() {
  if (init_flag_) {
    GELOGW("[InnerSession:%lu] session already initialize.", session_id_);
    return SUCCESS;
  }
  UpdateThreadContext(std::map<std::string, std::string>{});

  GE_CHK_RT_RET(rtSetDevice(GetContext().DeviceId()));

  Status ret = graph_manager_.Initialize(options_);
  if (ret != SUCCESS) {
    GELOGE(ret, "[InnerSession:%lu] initialize failed.", session_id_);
    return ret;
  }

  int32_t version = static_cast<int32_t>(SessionVersion::ClOUD_VERSION);
  const int DEFAULT_DEVICE_ID = 0;
  const int DEFAULT_JOB_ID = 0;
  ret = VarManager::Instance(session_id_)->Init(version, session_id_, DEFAULT_DEVICE_ID, DEFAULT_JOB_ID);
  if (ret != SUCCESS) {
    GELOGE(ret, "failed to init session instance");
  }
  init_flag_ = true;
  return SUCCESS;
}

Status InnerSession::Finalize() {
  std::lock_guard<std::mutex> lock(resource_mutex_);
  if (!init_flag_) {
    GELOGW("[InnerSession:%lu] session does not initialize.", session_id_);
    return SUCCESS;
  }
  UpdateThreadContext(std::map<std::string, std::string>{});
  Status ret = graph_manager_.Finalize();
  if (ret != SUCCESS) {
    // Subsequent code execution is required, so no return is required
    GELOGE(ret, "[InnerSession:%lu] finalize failed.", session_id_);
  }

  ModelManager::GetInstance()->DestroyAicpuSession(session_id_);
  init_flag_ = false;
  // release var memory
  GELOGI("VarManager free var memory.");
  (void)VarManager::Instance(session_id_)->FreeVarMemory();
  GE_CHK_RT(rtDeviceReset(static_cast<int32_t>(GetContext().DeviceId())));

  return ret;
}

Status InnerSession::GetVariable(const std::string &name, Tensor &val) {
  UpdateThreadContext(std::map<std::string, std::string>{});
  return graph_manager_.GetVariable(name, val);
}

Status InnerSession::AddGraph(uint32_t graph_id, const Graph &graph) {
  std::map<std::string, std::string> options;
  return AddGraph(graph_id, graph, options);
}

Status InnerSession::AddGraph(uint32_t graph_id, const Graph &graph,
                              const std::map<std::string, std::string> &options) {
  std::lock_guard<std::mutex> lock(resource_mutex_);
  if (!init_flag_) {
    GELOGE(GE_SESS_INIT_FAILED, "[InnerSession:%lu] initialize failed.", session_id_);
    return GE_SESS_INIT_FAILED;
  }
  UpdateThreadContext(options);
  Status ret = graph_manager_.AddGraph(graph_id, graph, options);
  if (ret != SUCCESS) {
    GELOGE(ret, "[InnerSession:%lu] add graph %u failed.", session_id_, graph_id);
    return ret;
  }

  GELOGI("[InnerSession:%lu] add graph success, graph_id=%u.", session_id_, graph_id);
  return SUCCESS;
}

Status InnerSession::RunGraph(uint32_t graph_id, const std::vector<Tensor> &inputs, std::vector<Tensor> &outputs) {
  GELOGI("[InnerSession:%lu] run graph on session, graph_id=%u.", session_id_, graph_id);
  if (mutex_.try_lock()) {
    std::lock_guard<std::mutex> lock(mutex_, std::adopt_lock);
    if (!init_flag_) {
      GELOGE(GE_SESS_INIT_FAILED, "[InnerSession:%lu] initialize failed.", session_id_);
      return GE_SESS_INIT_FAILED;
    }
    UpdateThreadContext(graph_id);
    vector<GeTensor> geInputs;
    for (auto &item : inputs) {
      geInputs.push_back(TensorAdapter::AsGeTensor(item));
    }
    vector<GeTensor> geOutputs;
    Status ret = graph_manager_.RunGraph(graph_id, geInputs, geOutputs, session_id_);
    domi::GetContext().out_nodes_map.clear();
    domi::GetContext().user_out_nodes.clear();
    if (ret != SUCCESS) {
      GELOGE(ret, "[InnerSession:%lu] run graph failed, graph_id=%u.", session_id_, graph_id);
      return ret;
    }
    outputs.clear();
    for (auto &item : geOutputs) {
      outputs.push_back(TensorAdapter::AsTensor(item));
    }

    GELOGI("[InnerSession:%lu] run graph success, graph_id=%u.", session_id_, graph_id);
    return SUCCESS;
  } else {
    GELOGE(GE_SESS_ALREADY_RUNNING, "[InnerSession:%lu] run graph failed, graph_id=%u.", session_id_, graph_id);
    return GE_SESS_ALREADY_RUNNING;
  }
}

Status InnerSession::RemoveGraph(uint32_t graph_id) {
  std::lock_guard<std::mutex> lock(resource_mutex_);
  if (!init_flag_) {
    GELOGE(GE_SESS_INIT_FAILED, "[InnerSession:%lu] initialize failed.", session_id_);
    return GE_SESS_INIT_FAILED;
  }
  UpdateThreadContext(graph_id);
  Status ret = graph_manager_.RemoveGraph(graph_id);
  if (ret != SUCCESS) {
    GELOGE(ret, "[InnerSession:%lu] remove graph failed, graph_id=%u.", session_id_, graph_id);
    return ret;
  }

  GELOGI("[InnerSession:%lu] remove graph success, graph_id=%u.", session_id_, graph_id);
  return SUCCESS;
}

Status InnerSession::RegisterCallBackFunc(
  const std::string &key, const std::function<Status(uint32_t, const std::map<std::string, ge::Tensor> &)> &callback) {
  std::lock_guard<std::mutex> lock(resource_mutex_);
  if (!init_flag_) {
    GELOGE(GE_SESS_INIT_FAILED, "[InnerSession:%lu] initialize failed.", session_id_);
    return GE_SESS_INIT_FAILED;
  }
  UpdateThreadContext(std::map<std::string, std::string>{});
  Status ret = graph_manager_.RegisterCallBackFunc(key, callback);
  if (ret != SUCCESS) {
    GELOGE(ret, "[InnerSession:%lu] register %s callback function failed.", session_id_, key.c_str());
    return ret;
  }

  GELOGI("[InnerSession:%lu] register %s callback function success.", session_id_, key.c_str());
  return SUCCESS;
}

Status InnerSession::RunGraphAsync(uint32_t graph_id, const std::vector<TensorInfo> &inputs,
                                   std::vector<TensorInfo> &outputs, std::function<void(Status)> callback) {
  UpdateThreadContext(graph_id);
  GELOGI("[InnerSession:%lu] run graph on session, graph_id=%u.", session_id_, graph_id);
  Status ret = graph_manager_.RunGraphAsync(graph_id, inputs, outputs, session_id_, callback);
  if (ret != SUCCESS) {
    GELOGE(ret, "[InnerSession:%lu] run graph failed, graph_id=%u.", session_id_, graph_id);
    return ret;
  }
  GELOGI("[InnerSession:%lu] run graph success, graph_id=%u.", session_id_, graph_id);
  return ret;
}

const GraphManager &InnerSession::getGraphManagerObj() const { return graph_manager_; }

void InnerSession::UpdateThreadContext(const std::map<std::string, std::string> &options) {
  GetThreadLocalContext().SetGlobalOption(GetMutableGlobalOptions());
  GetThreadLocalContext().SetSessionOption(options_);
  GetThreadLocalContext().SetGraphOption(options);
}

void InnerSession::UpdateThreadContext(uint32_t graph_id) {
  auto options = graph_manager_.GetGraphOptions(graph_id);
  if (options == nullptr) {
    GELOGW("graph level options is null.");
    UpdateThreadContext(std::map<std::string, std::string>{});
  } else {
    UpdateThreadContext(*options);
  }
}

bool InnerSession::IsGraphNeedRebuild(uint32_t graph_id) {
  UpdateThreadContext(graph_id);
  return graph_manager_.IsGraphNeedRebuild(graph_id);
}
}  // namespace ge
