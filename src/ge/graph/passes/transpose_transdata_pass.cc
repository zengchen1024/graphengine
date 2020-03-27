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

#include "graph/passes/transpose_transdata_pass.h"

#include <memory>
#include <string>
#include <vector>

#include "common/formats/utils/formats_trans_utils.h"
#include "framework/common/debug/ge_log.h"
#include "graph/utils/type_utils.h"
#include "graph/debug/ge_attr_define.h"
#include "init/gelib.h"
#include "opskernel_manager/ops_kernel_manager.h"

namespace {
const char *const kAttrNameSrcFormat = "src_format";
}

namespace ge {
Status TransposeTransDataPass::Run(NodePtr &node) {
  if (node == nullptr) {
    GELOGE(PARAM_INVALID, "param [node] must not be null.");
    return PARAM_INVALID;
  }
  auto op_desc = node->GetOpDesc();
  if (op_desc == nullptr) {
    GELOGE(PARAM_INVALID, "OpDesc of param [node] must not be null.");
    return PARAM_INVALID;
  }

  if (op_desc->GetType() != TRANSPOSE && op_desc->GetType() != TRANSPOSED) {
    return SUCCESS;
  }
  if (CheckOneInAndOneOutDataAnchor(node) != SUCCESS) {
    return FAILED;
  }
  GELOGD("[%s] TransposeTransDataPass in.", node->GetName().c_str());

  auto out_nodes = node->GetOutDataNodes();
  bool is_add_flag = false;
  for (auto &out_node : out_nodes) {
    GE_CHECK_NOTNULL(out_node);
    OpDescPtr out_op_desc = out_node->GetOpDesc();
    if (out_op_desc == nullptr) {
      GELOGE(FAILED, "OpDesc of out data node of [%s] must not be null.", node->GetName().c_str());
      return FAILED;
    }
    if (out_op_desc->GetType() != TRANSDATA) {
      continue;
    }
    if (CheckOneInAndOneOutDataAnchor(out_node)) {
      return FAILED;
    }
    if (!FusionIfNeed(op_desc, out_op_desc)) {
      continue;
    }
    CopyInputEdges(node, out_node);
    is_add_flag = true;
  }

  if (is_add_flag) {
    AddRePassNode(node->GetInDataNodes().at(0));
  }
  if (node->GetOutDataNodesSize() == 0) {
    // all output nodes of transpose has fused, delete transpose
    return RemoveTranspose(node);
  }
  return SUCCESS;
}

Status TransposeTransDataPass::CheckOneInAndOneOutDataAnchor(NodePtr &node) const {
  GE_CHECK_NOTNULL(node);
  // Trans op has one input one output data anchor
  uint32_t in_data_anchor_nums = node->GetAllInDataAnchorsSize();
  uint32_t out_data_anchor_nums = node->GetAllOutDataAnchorsSize();
  // Trans op has one input data node, maybe has N output data nodes
  uint32_t in_data_node_nums = node->GetInDataNodes().size();
  if (in_data_anchor_nums != 1 || out_data_anchor_nums != 1 || in_data_node_nums != 1) {
    GELOGE(FAILED, "[%s] %s has %u in %u out data anchor, has %u in data node.", node->GetType().c_str(),
           node->GetName().c_str(), in_data_anchor_nums, out_data_anchor_nums, in_data_node_nums);
    return FAILED;
  }
  return SUCCESS;
}

Status TransposeTransDataPass::RemoveTranspose(NodePtr &node) {
  GE_CHECK_NOTNULL(node);
  ComputeGraphPtr graph = node->GetOwnerComputeGraph();
  if (graph == nullptr) {
    GELOGE(FAILED, "[%s] The owner graph must not be null.", node->GetName().c_str());
    return FAILED;
  }

  // If delete Transpos/TransposeD, change its peer in ctrl anchor to its input node
  // If not delete, need do nothing
  auto origin_node_in = node->GetInDataNodes().at(0);
  for (auto &peer_anchor : node->GetOutControlAnchor()->GetPeerInControlAnchors()) {
    GE_CHECK_NOTNULL(origin_node_in);
    GE_CHECK_NOTNULL(origin_node_in->GetOutControlAnchor());
    GE_CHK_STATUS_RET(origin_node_in->GetOutControlAnchor()->LinkTo(peer_anchor), "link failed");
  }

  for (const auto &anchor : node->GetAllInAnchors()) {
    GE_CHECK_NOTNULL(anchor);
    anchor->UnlinkAll();
  }
  for (const auto &anchor : node->GetAllOutAnchors()) {
    GE_CHECK_NOTNULL(anchor);
    anchor->UnlinkAll();
  }
  AddNodeDeleted(node.get());
  if (GraphUtils::RemoveNodeWithoutRelink(graph, node) != GRAPH_SUCCESS) {
    GELOGE(FAILED, "[%s] RemoveNodeWithoutRelink failed.", node->GetName().c_str());
    return FAILED;
  }
  return SUCCESS;
}

bool TransposeTransDataPass::FusionIfNeed(OpDescPtr &op_desc, OpDescPtr &transdata_op_desc) {
  GE_CHECK_NOTNULL(op_desc);
  GE_CHECK_NOTNULL(transdata_op_desc);
  auto out_input_desc = transdata_op_desc->MutableInputDesc(0);
  GE_CHECK_NOTNULL(out_input_desc);
  auto out_input_format = out_input_desc->GetFormat();
  auto out_input_shape = out_input_desc->GetShape();

  auto input_desc = op_desc->MutableInputDesc(0);
  auto out_desc = op_desc->MutableOutputDesc(0);
  GE_CHECK_NOTNULL(input_desc);
  GE_CHECK_NOTNULL(out_desc);
  auto src_format = input_desc->GetFormat();
  auto dst_format = out_desc->GetFormat();
  auto &dst_shape = out_desc->MutableShape();
  if (dst_format != out_input_format || !formats::IsShapeEqual(dst_shape, out_input_shape) || src_format == FORMAT_ND) {
    GELOGD("Output of transpose isn't the same as input of transdata, or transpose input format must not be ND.");
    GELOGD("Transpose input format %s, output format %s shape %s. transdata in %s %s.",
           TypeUtils::FormatToSerialString(src_format).c_str(), TypeUtils::FormatToSerialString(dst_format).c_str(),
           formats::ShapeToString(dst_shape.GetDims()).c_str(),
           TypeUtils::FormatToSerialString(out_input_format).c_str(),
           formats::ShapeToString(out_input_shape.GetDims()).c_str());
    return false;
  }

  auto &src_shape = input_desc->MutableShape();
  GELOGI("Begin to fuse transpose transdata, transpose in format %s shape %s, transdata in %s %s",
         TypeUtils::FormatToSerialString(src_format).c_str(), formats::ShapeToString(src_shape.GetDims()).c_str(),
         TypeUtils::FormatToSerialString(out_input_format).c_str(),
         formats::ShapeToString(out_input_shape.GetDims()).c_str());

  // Transpose can change format and shape
  out_input_desc->SetFormat(src_format);
  out_input_desc->SetShape(src_shape);

  if (!TransDataCheckAccuracySupported(transdata_op_desc)) {
    out_input_desc->SetFormat(out_input_format);
    out_input_desc->SetShape(out_input_shape);
    return false;
  }

  // add attr to fused TransData, then will be rebuild
  string new_node_name = op_desc->GetName() + transdata_op_desc->GetName();
  transdata_op_desc->SetName(new_node_name);
  GE_IF_BOOL_EXEC(!AttrUtils::SetBool(transdata_op_desc, ATTR_NEED_COMPILE, true),
                  GELOGW("set ext attr failed"); return false);

  string format_val = TypeUtils::FormatToSerialString(src_format);
  GE_IF_BOOL_EXEC(!AttrUtils::SetStr(transdata_op_desc, kAttrNameSrcFormat, format_val),
                  GELOGW("set kAttrNameSrcFormat failed"); return false);
  GELOGI("TransposeTransDataPass, fuse to be node %s.", transdata_op_desc->GetName().c_str());
  return true;
}

void TransposeTransDataPass::CopyInputEdges(NodePtr &origin_node, NodePtr &new_node) {
  if (origin_node == nullptr || new_node == nullptr) {
    return;
  }
  InDataAnchorPtr new_in_data_anchor = new_node->GetInDataAnchor(0);
  if (new_in_data_anchor == nullptr || origin_node->GetInDataAnchor(0) == nullptr) {
    return;
  }
  OutDataAnchorPtr out_anchor = origin_node->GetInDataAnchor(0)->GetPeerOutAnchor();
  new_in_data_anchor->UnlinkAll();
  GE_IF_BOOL_EXEC(new_in_data_anchor->LinkFrom(out_anchor) != GRAPH_SUCCESS, GELOGW("Link failed"); return);

  // control anchor only link to control anchor
  GE_IF_BOOL_EXEC(GraphUtils::CopyInCtrlEdges(origin_node, new_node) != GRAPH_SUCCESS,
                  GELOGW("Copy in ctrl edges failed"); return);
}

bool TransposeTransDataPass::TransDataCheckAccuracySupported(const OpDescPtr &op_desc) {
  std::shared_ptr<GELib> instance_ptr = ge::GELib::GetInstance();
  if ((instance_ptr == nullptr) || (!instance_ptr->InitFlag())) {
    GELOGW("GELib not initialized");
    return false;
  }

  OpsKernelManager &ops_kernel_manager = instance_ptr->OpsKernelManagerObj();
  vector<OpInfo> op_infos = ops_kernel_manager.GetOpsKernelInfo(op_desc->GetType());
  if (op_infos.empty()) {
    GELOGW("Can not get op info by op type %s", op_desc->GetType().c_str());
    return false;
  }

  std::string unsupported_reason;
  for (auto &it : op_infos) {
    auto kernel_map = ops_kernel_manager.GetAllOpsKernelInfoStores();
    auto &kernel_name = it.opKernelLib;
    auto kernel_info_store = kernel_map.find(kernel_name);
    if (kernel_info_store != kernel_map.end()) {
      if (kernel_info_store->second->CheckAccuracySupported(op_desc, unsupported_reason, true)) {
        return true;
      }
    }
  }
  GELOGI("TransposeTransDataPass CheckAccuracySupported[%s] all not support, reason:%s.", op_desc->GetName().c_str(),
         unsupported_reason.c_str());
  return false;
}
}  // namespace ge