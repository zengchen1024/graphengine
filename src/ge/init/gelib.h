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

#ifndef GE_INIT_GELIB_H_
#define GE_INIT_GELIB_H_
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "engine_manager/dnnengine_manager.h"
#include "opskernel_manager/ops_kernel_manager.h"
#include "session/session_manager.h"
#include "common/ge_inner_error_codes.h"
#include "common/ge_types.h"

using std::map;
using std::string;
using std::vector;

namespace ge {
class GELib {
 public:
  GELib() = default;
  ~GELib() = default;

  // get GELib singleton instance
  static std::shared_ptr<GELib> GetInstance();

  // GE Environment Initialize, return Status: SUCCESS,FAILED
  static Status Initialize(const map<string, string> &options);

  static string GetPath();

  // GE Environment Finalize, return Status: SUCCESS,FAILED
  Status Finalize();

  // get DNNEngineManager object
  DNNEngineManager &DNNEngineManagerObj() { return engine_manager_; }

  // get OpsKernelManager object
  OpsKernelManager &OpsKernelManagerObj() { return ops_manager_; }

  // get SessionManager object
  SessionManager &SessionManagerObj() { return session_manager_; }

  // get Initial flag
  bool InitFlag() const { return init_flag_; }

  // get TrainMode flag
  bool isTrainMode() const { return is_train_mode_; }

  // add head stream to model
  bool HeadStream() const { return head_stream_; }

  Status InitSystemWithoutOptions();
  Status InitSystemWithOptions(Options &options);
  Status SystemShutdownWithOptions(const Options &options);

 private:
  GELib(const GELib &);
  const GELib &operator=(const GELib &);
  Status InnerInitialize(const map<string, string> &options);
  Status SystemInitialize(const map<string, string> &options);
  void RollbackInit();
  void InitOptions(const map<string, string> &options);

  DNNEngineManager engine_manager_;
  OpsKernelManager ops_manager_;
  SessionManager session_manager_;
  std::mutex status_mutex_;
  bool init_flag_ = false;
  Options options_;
  bool is_train_mode_ = false;
  bool is_system_inited = false;
  bool is_shutdown = false;
  bool is_use_hcom = false;

  bool head_stream_ = false;
};
}  // namespace ge

#endif  // GE_INIT_GELIB_H_
