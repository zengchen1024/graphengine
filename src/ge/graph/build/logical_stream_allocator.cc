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

#include "graph/build/logical_stream_allocator.h"
#include "common/ge/ge_util.h"
#include "framework/common/debug/ge_log.h"
#include "framework/common/fmk_error_codes.h"
#include "framework/common/types.h"
#include "graph/utils/graph_utils.h"
#include "graph/debug/ge_attr_define.h"

using std::map;
using std::set;
using std::string;
using std::vector;

namespace ge {
LogicalStreamPass::LogicalStreamPass(const string &name) : name_(name) {}

const string &LogicalStreamPass::GetName() const { return name_; }

bool LogicalStreamPass::IsEngineSkip(const Subgraph &subgraph) const { return subgraph.engine_conf.skip_assign_stream; }

bool LogicalStreamPass::IsEngineAttach(const Subgraph &subgraph) const { return subgraph.engine_conf.attach; }

bool LogicalStreamPass::IsEngineIndependent(const Subgraph &subgraph) const { return subgraph.engine_conf.independent; }

bool LogicalStreamPass::HasStreamLabel(const Subgraph &subgraph) const {
  return !subgraph.subgraph_info.GetStreamLabel().empty();
}

bool LogicalStreamPass::HasAssignedStream(const Subgraph &subgraph) const {
  return subgraph.stream_id != kInvalidStream;
}

Status AssignByLabelPass::Run(ComputeGraphPtr whole_graph, const vector<SubgraphPtr> &subgraphs, Context &context) {
  bool changed = false;
  int64_t &next_stream = context.next_stream;
  map<string, int64_t> label_streams;

  for (const SubgraphPtr &subgraph : subgraphs) {
    const string &stream_label = subgraph->subgraph_info.GetStreamLabel();
    if (!stream_label.empty()) {
      // Subgraphs of the same stream_label are assigned to the same stream,
      // and different stream_labels are assigned new streams.
      auto iter = label_streams.find(stream_label);
      if (iter != label_streams.end()) {
        subgraph->stream_id = iter->second;
      } else {
        subgraph->stream_id = next_stream;
        GELOGI("Assign new stream %ld for label %s.", next_stream, stream_label.c_str());

        label_streams.emplace(stream_label, next_stream);
        ++next_stream;
      }
      changed = true;
    }
  }

  return changed ? SUCCESS : NOT_CHANGED;
}

Status IndependentStreamPass::Run(ComputeGraphPtr whole_graph, const vector<SubgraphPtr> &subgraphs, Context &context) {
  bool changed = false;
  int64_t &next_stream = context.next_stream;

  // <engine, <label, stream>>
  map<string, map<string, int64_t>> engine_streams;

  for (const SubgraphPtr &subgraph : subgraphs) {
    if (!IsEngineIndependent(*subgraph)) {
      continue;
    }

    const string &engine = subgraph->engine_conf.id;
    const string &stream_label = subgraph->subgraph_info.GetStreamLabel();
    auto &label_streams = engine_streams[engine];
    auto iter = label_streams.find(stream_label);
    if (iter != label_streams.end()) {
      subgraph->stream_id = iter->second;
    } else {
      subgraph->stream_id = next_stream;
      GELOGI("Assign new independent stream %ld for engine %s (label: %s).", next_stream, engine.c_str(),
             stream_label.c_str());

      label_streams.emplace(stream_label, next_stream);
      ++next_stream;
    }
    changed = true;
  }

  return changed ? SUCCESS : NOT_CHANGED;
}

Status AssignByDependencyPass::Run(ComputeGraphPtr whole_graph, const vector<SubgraphPtr> &subgraphs,
                                   Context &context) {
  bool changed = false;
  map<NodePtr, SubgraphPtr> end_subgraph_map;
  map<NodePtr, SubgraphPtr> pld_subgraph_map;
  InitEndSubgraphMap(subgraphs, end_subgraph_map);
  InitPldSubgraphMap(subgraphs, pld_subgraph_map);

  for (const SubgraphPtr &subgraph : subgraphs) {
    if (HasAssignedStream(*subgraph)) {
      continue;
    }

    SubgraphPtr reusable_subgraph = GetReusableSubgraph(subgraph, end_subgraph_map, pld_subgraph_map);
    if (reusable_subgraph != nullptr) {
      if (HasAssignedStream(*reusable_subgraph)) {
        subgraph->stream_id = reusable_subgraph->stream_id;
      } else {
        int64_t stream_id = AssignNewStream(reusable_subgraph);
        subgraph->stream_id = stream_id;
        GELOGI("Reusable subgraph %s has not been assigned a stream, now assign new stream %ld.",
               reusable_subgraph->name.c_str(), stream_id);
      }

      if (reusable_subgraph->reused_subgraph != nullptr) {
        reusable_subgraph = reusable_subgraph->reused_subgraph;
      }

      subgraph->reused_subgraph = reusable_subgraph;
      reused_subgraphs_.emplace_back(subgraph, reusable_subgraph);
      GELOGI("Subgraph %s of engine %s reuses stream of subgraph %s of engine %s.", subgraph->name.c_str(),
             subgraph->engine_conf.id.c_str(), reusable_subgraph->name.c_str(),
             reusable_subgraph->engine_conf.id.c_str());
    } else {
      (void)AssignNewStream(subgraph);
    }
    changed = true;
  }

  UpdateAssignedSubgraphs(context);
  UpdateReusedSubgraphs();

  return changed ? SUCCESS : NOT_CHANGED;
}

void AssignByDependencyPass::InitEndSubgraphMap(const vector<SubgraphPtr> &subgraphs,
                                                map<NodePtr, SubgraphPtr> &end_subgraph_map) {
  for (const auto &subgraph : subgraphs) {
    const SubGraphInfo &subgraph_info = subgraph->subgraph_info;
    for (const auto &item : subgraph_info.GetEnd2PldMap()) {
      end_subgraph_map.emplace(item.first, subgraph);
    }
  }
}

void AssignByDependencyPass::InitPldSubgraphMap(const vector<SubgraphPtr> &subgraphs,
                                                map<NodePtr, SubgraphPtr> &pld_subgraph_map) {
  for (const auto &subgraph : subgraphs) {
    const SubGraphInfo &subgraph_info = subgraph->subgraph_info;
    for (const auto &item : subgraph_info.GetPld2EndMap()) {
      pld_subgraph_map.emplace(item.first, subgraph);
    }
  }
}

bool AssignByDependencyPass::CouldReuse(const SubgraphPtr &subgraph, const SubgraphPtr &pred_subgraph,
                                        const map<NodePtr, SubgraphPtr> &pld_subgraph_map) {
  if ((subgraph == nullptr) || (pred_subgraph == nullptr)) {
    return false;
  }

  if (subgraph->engine_conf.scheduler_id != pred_subgraph->engine_conf.scheduler_id) {
    return false;
  }

  if (IsEngineIndependent(*pred_subgraph) || HasStreamLabel(*pred_subgraph)) {
    return false;
  }

  // If the engine of the predecessor subgraph is the same as the other successor subgraphs, the stream is not reused.
  for (const auto &end_pld_pair : pred_subgraph->subgraph_info.GetEnd2PldMap()) {
    auto iter = pld_subgraph_map.find(end_pld_pair.second);
    if (iter != pld_subgraph_map.end()) {
      const SubgraphPtr &pred_subgraph_succ = iter->second;
      if (pred_subgraph_succ != subgraph && pred_subgraph_succ->engine_conf.id == pred_subgraph->engine_conf.id) {
        return false;
      }
    }
  }

  if ((subgraph->engine_conf.id == pred_subgraph->engine_conf.id) || IsEngineAttach(*subgraph)) {
    return true;
  }

  if ((pred_subgraph->reused_subgraph != nullptr) &&
      (pred_subgraph->reused_subgraph->engine_conf.id == subgraph->engine_conf.id)) {
    return true;
  }

  return false;
}

LogicalStreamPass::SubgraphPtr AssignByDependencyPass::GetReusableSubgraph(
  const SubgraphPtr &subgraph, const map<NodePtr, SubgraphPtr> &end_subgraph_map,
  const map<NodePtr, SubgraphPtr> &pld_subgraph_map) {
  const SubGraphInfo &subgraph_info = subgraph->subgraph_info;
  for (const auto &pld_2_end : subgraph_info.GetPld2EndMap()) {
    const NodePtr &peer_end = pld_2_end.second;
    auto iter = end_subgraph_map.find(peer_end);
    if (iter != end_subgraph_map.end()) {
      const SubgraphPtr &pred_subgraph = iter->second;
      if (CouldReuse(subgraph, pred_subgraph, pld_subgraph_map)) {
        return pred_subgraph;
      }
    }
  }

  return nullptr;
}

int64_t AssignByDependencyPass::AssignNewStream(SubgraphPtr subgraph) {
  const string &engine_name = subgraph->engine_conf.id;
  int64_t max_parallel_num = subgraph->max_parallel_num;

  int64_t stream_id = 0;
  auto next_iter = engine_next_streams_.find(engine_name);
  if (next_iter != engine_next_streams_.end()) {
    stream_id = next_iter->second;
  }

  if (stream_id >= max_parallel_num) {
    stream_id = 0;
  }

  subgraph->stream_id = stream_id;
  engine_next_streams_[engine_name] = stream_id + 1;
  assigned_subgraphs_.emplace(subgraph);

  if ((stream_id + 1) > engine_stream_num_[engine_name]) {
    engine_stream_num_[engine_name] = stream_id + 1;
  }

  GELOGI("Subgraph %s assigns new temp stream %ld (engine: %s).", subgraph->name.c_str(), stream_id,
         engine_name.c_str());

  return stream_id;
}

void AssignByDependencyPass::UpdateAssignedSubgraphs(Context &context) {
  // Update the starting stream id for each engine.
  int64_t &next_stream = context.next_stream;
  map<string, int64_t> engine_start_streams;
  for (const auto &item : engine_stream_num_) {
    int64_t stream_count = item.second;
    engine_start_streams[item.first] = next_stream;
    next_stream += stream_count;
  }

  // Update the subgraphs assigned by the engine.
  for (auto &subgraph : assigned_subgraphs_) {
    subgraph->stream_id += engine_start_streams[subgraph->engine_conf.id];
  }
}

void AssignByDependencyPass::UpdateReusedSubgraphs() {
  // Update streams for the subgraphs of reusing stream.
  for (const auto &item : reused_subgraphs_) {
    auto &cur_subgraph = item.first;
    auto &reused_graph = item.second;
    cur_subgraph->stream_id = reused_graph->stream_id;
    GELOGI("Stream of subgraph %s has been updated to %ld.", cur_subgraph->name.c_str(), cur_subgraph->stream_id);
  }
}

Status NodeStreamUpdatePass::Run(ComputeGraphPtr whole_graph, const vector<SubgraphPtr> &subgraphs, Context &context) {
  // Check if all subgraphs have been assigned a stream.
  for (const SubgraphPtr &subgraph : subgraphs) {
    const string &engine_name = subgraph->engine_conf.id;

    if (!IsEngineSkip(*subgraph) && !HasAssignedStream(*subgraph)) {
      GELOGE(INTERNAL_ERROR, "Subgraph %s has not yet been assigned a stream (engine: %s).", subgraph->name.c_str(),
             engine_name.c_str());
      return INTERNAL_ERROR;
    } else {
      GELOGI("Subgraph %s is assigned stream %ld (engine: %s).", subgraph->name.c_str(), subgraph->stream_id,
             engine_name.c_str());
    }
  }

  // Init the stream id of node.
  for (NodePtr &node : whole_graph->GetDirectNode()) {
    GE_CHECK_NOTNULL(node->GetOpDesc());
    node->GetOpDesc()->SetStreamId(kInvalidStream);
  }

  // Set the stream id of the subgraph to the node.
  for (const SubgraphPtr &subgraph : subgraphs) {
    int64_t stream_id = subgraph->stream_id;
    const string &engine_name = subgraph->engine_conf.id;
    auto compute_graph = subgraph->subgraph_info.GetSubGraph();
    for (NodePtr &node : compute_graph->GetDirectNode()) {
      GE_CHECK_NOTNULL(node->GetOpDesc());
      if (IsEngineSkip(*subgraph) && node->GetInNodes().empty()) {
        GELOGD("Node %s of type %s in subgraph %s doesn't need to assign a stream (engine: %s).",
               node->GetName().c_str(), node->GetType().c_str(), subgraph->name.c_str(), engine_name.c_str());
      } else {
        node->GetOpDesc()->SetStreamId(stream_id);
      }
    }
  }

  // Update stream id for nodes belong to skipped engine subgraph
  GE_CHK_STATUS_RET(UpdateForSkippedEngine(whole_graph, subgraphs));

  return SUCCESS;
}

Status AllReduceParallelPass::Run(ComputeGraphPtr whole_graph, const vector<SubgraphPtr> &subgraphs, Context &context) {
  if (!context.hcom_parallel) {
    return NOT_CHANGED;
  }

  GELOGI("AllReduceParallelPass is enabled.");
  GraphUtils::DumpGEGraph(whole_graph, "BeforeAllReduceParallel");

  // All successors of HcomAllReduce.
  set<NodePtr> all_reduce_succs;

  for (const NodePtr &node : whole_graph->GetDirectNode()) {
    if (node->GetType() != HCOMALLREDUCE || node->GetInDataNodes().size() <= 1) {
      continue;
    }

    string reduce_stream_label;
    GE_CHECK_NOTNULL(node->GetOpDesc());
    // ATTR_NAME_STREAM_LABEL is optional.
    (void)AttrUtils::GetStr(node->GetOpDesc(), ATTR_NAME_STREAM_LABEL, reduce_stream_label);

    set<NodePtr> cur_nodes = {node};
    while (!cur_nodes.empty()) {
      set<NodePtr> all_out_data_nodes;
      for (auto &curr_node : cur_nodes) {
        for (const NodePtr &out_node : curr_node->GetOutDataNodes()) {
          string out_stream_label;
          GE_CHECK_NOTNULL(out_node->GetOpDesc());
          // ATTR_NAME_STREAM_LABEL is optional.
          (void)AttrUtils::GetStr(out_node->GetOpDesc(), ATTR_NAME_STREAM_LABEL, out_stream_label);
          if (out_stream_label == reduce_stream_label) {
            all_reduce_succs.emplace(out_node);
            all_out_data_nodes.emplace(out_node);
          }
        }
      }
      cur_nodes = all_out_data_nodes;
    }
  }

  map<int64_t, int64_t> old_stream_to_new;
  for (const NodePtr &node : all_reduce_succs) {
    GE_CHECK_NOTNULL(node->GetOpDesc());
    auto old_stream = node->GetOpDesc()->GetStreamId();
    if (old_stream != kInvalidStream) {
      int64_t new_stream = kInvalidStream;
      auto iter = old_stream_to_new.find(old_stream);
      if (iter != old_stream_to_new.end()) {
        new_stream = iter->second;
      } else {
        new_stream = context.next_stream;
        context.next_stream++;
        old_stream_to_new.emplace(old_stream, new_stream);
      }

      GELOGI("Stream of node %s has been updated from %ld to %ld.", node->GetName().c_str(), old_stream, new_stream);
      node->GetOpDesc()->SetStreamId(new_stream);
    }
  }

  return !all_reduce_succs.empty() ? SUCCESS : NOT_CHANGED;
}

int64_t NodeStreamUpdatePass::GetSingleInoutStream(const NodePtr &node) const {
  set<int64_t> stream_ids;

  for (const auto &in_node : node->GetInAllNodes()) {
    GE_CHECK_NOTNULL_EXEC(in_node->GetOpDesc(), return kInvalidStream);
    int64_t stream_id = in_node->GetOpDesc()->GetStreamId();
    if (stream_id != kInvalidStream) {
      stream_ids.insert(stream_id);
    }
  }
  for (const auto &out_node : node->GetOutAllNodes()) {
    GE_CHECK_NOTNULL_EXEC(out_node->GetOpDesc(), return kInvalidStream);
    int64_t stream_id = out_node->GetOpDesc()->GetStreamId();
    if (stream_id != kInvalidStream) {
      stream_ids.insert(stream_id);
    }
  }
  if (stream_ids.size() == 1) {
    int64_t stream_id = *(stream_ids.begin());
    GELOGI("Node %s of type %s: its all input and output nodes are in same stream id[%ld].", node->GetName().c_str(),
           node->GetType().c_str(), stream_id);
    return stream_id;
  }

  return kInvalidStream;
}

Status NodeStreamUpdatePass::UpdateForSkippedEngine(const ComputeGraphPtr &whole_graph,
                                                    const vector<SubgraphPtr> &subgraphs) {
  set<OpDescPtr> nodes_to_be_updated;

  // Check if sub graph is engine skipped and without stream label or not
  for (const SubgraphPtr &subgraph : subgraphs) {
    if (IsEngineSkip(*subgraph) && !HasStreamLabel(*subgraph)) {
      auto compute_graph = subgraph->subgraph_info.GetSubGraph();
      for (NodePtr &node : compute_graph->GetDirectNode()) {
        auto op_desc = node->GetOpDesc();
        GE_CHECK_NOTNULL(op_desc);
        auto stream_id = op_desc->GetStreamId();
        if (stream_id != kInvalidStream) {
          nodes_to_be_updated.insert(op_desc);
        }
      }
    }
  }

  // Try reassign the stream id
  for (ge::NodePtr &node : whole_graph->GetDirectNode()) {
    auto op_desc = node->GetOpDesc();
    GE_CHECK_NOTNULL(op_desc);
    int64_t stream_id = op_desc->GetStreamId();
    if (nodes_to_be_updated.find(op_desc) != nodes_to_be_updated.end()) {
      if (AreAllPredStreamsInvalid(node)) {
        op_desc->SetStreamId(kInvalidStream);
      } else {
        int64_t inout_stream = GetSingleInoutStream(node);
        if (inout_stream != kInvalidStream) {
          op_desc->SetStreamId(inout_stream);
          GELOGI("Node %s of type %s reassign to stream id[%ld] from stream id[%ld].", node->GetName().c_str(),
                 node->GetType().c_str(), inout_stream, stream_id);
        }
      }
    }
  }
  return SUCCESS;
}

bool NodeStreamUpdatePass::AreAllPredStreamsInvalid(const NodePtr &node) const {
  for (const auto &pre_node : node->GetInAllNodes()) {
    auto pre_node_desc = pre_node->GetOpDesc();
    if (pre_node_desc != nullptr) {
      int64_t stream_id = pre_node_desc->GetStreamId();
      if (stream_id != kInvalidStream) {
        return false;
      }
    }
  }
  return true;
}

LogicalStreamAllocator::LogicalStreamAllocator(const map<string, SchedulerConf> &scheduler_confs,
                                               const map<string, int> &max_parallel_num, bool hcom_parallel)
    : scheduler_confs_(scheduler_confs), max_parallel_num_(max_parallel_num) {
  context_.hcom_parallel = hcom_parallel;
}

Status LogicalStreamAllocator::Assign(const ComputeGraphPtr &whole_graph, const vector<SubGraphInfoPtr> &subgraph_infos,
                                      int64_t &stream_num) {
  GE_CHECK_NOTNULL(whole_graph);
  map<string, EngineConfPtr> engine_confs;
  for (const auto &item : scheduler_confs_) {
    const SchedulerConf &scheduler = item.second;
    for (const auto &engine_pair : scheduler.cal_engines) {
      EngineConfPtr engine_conf = engine_pair.second;
      if (engine_conf != nullptr) {
        engine_confs[engine_pair.first] = engine_conf;
      }
    }
  }

  vector<SubgraphPtr> subgraphs;
  Status status = ConvertSubgraphs(subgraph_infos, engine_confs, subgraphs);
  if (status != SUCCESS) {
    GELOGE(status, "Create subgraphs failed.");
    return status;
  }

  return RunPasses(whole_graph, subgraphs, stream_num);
}

Status LogicalStreamAllocator::ConvertSubgraphs(const vector<SubGraphInfoPtr> &subgraph_infos,
                                                const map<string, EngineConfPtr> &engine_confs,
                                                vector<SubgraphPtr> &subgraphs) {
  for (auto &subgraph_info : subgraph_infos) {
    GE_CHECK_NOTNULL(subgraph_info);

    string subgraph_name;
    ComputeGraphPtr computer_graph = subgraph_info->GetSubGraph();
    if (computer_graph != nullptr) {
      subgraph_name = computer_graph->GetName();
    }

    const string &engine_name = subgraph_info->GetEngineName();
    auto engine_conf_iter = engine_confs.find(engine_name);
    if ((engine_conf_iter == engine_confs.end()) || (engine_conf_iter->second == nullptr)) {
      GELOGE(INTERNAL_ERROR, "Engine conf of subgraph %s not found (engine name: %s).", subgraph_name.c_str(),
             engine_name.c_str());

      return INTERNAL_ERROR;
    }

    SubgraphPtr subgraph = MakeShared<Subgraph>(*subgraph_info, *engine_conf_iter->second);
    GE_CHECK_NOTNULL(subgraph);
    subgraph->name = subgraph_name;

    auto parallel_iter = max_parallel_num_.find(engine_name);
    if (parallel_iter != max_parallel_num_.end()) {
      subgraph->max_parallel_num = parallel_iter->second;
    }

    subgraphs.emplace_back(subgraph);
  }

  return SUCCESS;
}

Status LogicalStreamAllocator::RunPasses(const ComputeGraphPtr &whole_graph, const vector<SubgraphPtr> &subgraphs,
                                         int64_t &stream_num) {
  vector<LogicalStreamPassPtr> passes;
  passes.emplace_back(MakeShared<AssignByLabelPass>());
  passes.emplace_back(MakeShared<IndependentStreamPass>());
  passes.emplace_back(MakeShared<AssignByDependencyPass>());
  passes.emplace_back(MakeShared<NodeStreamUpdatePass>());
  passes.emplace_back(MakeShared<AllReduceParallelPass>());

  for (auto &pass : passes) {
    GE_CHECK_NOTNULL(pass);

    Status status = pass->Run(whole_graph, subgraphs, context_);
    if (status == SUCCESS) {
      GELOGI("Stream pass %s return SUCCESS.", pass->GetName().c_str());
    } else if (status == NOT_CHANGED) {
      GELOGI("Stream pass %s return NOT_CHANGED.", pass->GetName().c_str());
    } else {
      GELOGE(status, "Stream pass %s failed.", pass->GetName().c_str());
      return status;
    }
  }

  stream_num = context_.next_stream;
  GELOGI("Assigned logical stream num: %ld.", stream_num);

  return SUCCESS;
}
}  // namespace ge
