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

#include "framework/common/op/ge_op_utils.h"

#include <list>

#include "common/ge/ge_util.h"
#include "external/graph/types.h"
#include "framework/common/debug/ge_log.h"
#include "framework/common/debug/log.h"
#include "framework/common/fmk_error_codes.h"
#include "framework/common/ge_inner_error_codes.h"
#include "framework/common/op/attr_value_util.h"
#include "framework/common/util.h"
#include "graph/anchor.h"
#include "graph/debug/ge_attr_define.h"
#include "graph/utils/op_desc_utils.h"
#include "graph/utils/tensor_utils.h"
#include "graph/utils/type_utils.h"
#include "mmpa/mmpa_api.h"

#define RETURN_IF_TRUE(cond, errcode, ...) \
  do {                                     \
    if (cond) {                            \
      GELOGE(errcode, __VA_ARGS__);        \
      return errcode;                      \
    }                                      \
  } while (0);

using domi::DOMI_TENSOR_NCHW;
using std::vector;

namespace ge {
// General constant
const int32_t kDimMaxSize = 8;
const float DEFAULT_ALPHA_VALUE = 1.0;
const float DEFAULT_BETA_VALUE = 0.0;
const int NORMAL_TENSOR_SIZE = 4;
const int32_t kDimSizeZero = 0;
const int32_t kDimSizeOne = 1;
const int32_t kDimSizeTwo = 2;
const int32_t kDimSizeThree = 3;
const uint32_t kSliceDataNum = 2;

// Add Sub Mul
const uint32_t ADD_INPUT_NUM = 2;
const uint32_t SUB_INPUT_NUM = 2;
const uint32_t MUL_INPUT_NUM = 2;

// Permute
const int32_t PERMUTE_ORDER_NUM = 4;
// Ssd PriroBox
const double SSD_PRIORBOX_ASPECT_RATIO_VALUE = 1.0;
// Switch
const uint32_t SWITCH_INPUT_NUM = 2;
const uint32_t SWITCH_OUTPUT_NUM = 2;
const uint32_t SWITCH_FALSE_OUTPUT = 0;
const uint32_t SWITCH_TRUE_OUTPUT = 1;
const uint32_t SWITCH_DATA_INPUT = 0;
const uint32_t SWITCH_PRED_INPUT = 1;

// Get the value of key from attr
#define AIPP_GET_ATTR_VALUE(KEY, ATTR_TYPE)                          \
  if (aipp_attr.GetItem(#KEY).GetValue<ATTR_TYPE>(KEY) != SUCCESS) { \
    GELOGI("Attr %s will take default value.", #KEY);                \
    break;                                                           \
  }

// Converting aippparams and attrdefmap
#define AIPP_CONVERT_FORMAT_EX(KEY, ORG_TYPE, SAVE_TYPE, ATTR_TYPE) \
  do {                                                              \
    SAVE_TYPE KEY = static_cast<SAVE_TYPE>(0);                      \
    AIPP_GET_ATTR_VALUE(KEY, ATTR_TYPE)                             \
    aipp_params->set_##KEY(ORG_TYPE(KEY));                          \
  } while (0)

// Converting aippparams and attrdefmap
#define AIPP_CONVERT_FORMAT(KEY, KEY_TYPE, ATTR_TYPE) AIPP_CONVERT_FORMAT_EX(KEY, KEY_TYPE, KEY_TYPE, ATTR_TYPE)

#define AIPP_CONVERT_INT(KEY) AIPP_CONVERT_FORMAT(KEY, int64_t, GeAttrValue::INT)

#define AIPP_CONVERT_BOOL(KEY) AIPP_CONVERT_FORMAT(KEY, bool, GeAttrValue::BOOL)

#define AIPP_CONVERT_FLOAT(KEY) AIPP_CONVERT_FORMAT(KEY, float, GeAttrValue::FLOAT)

// Transform aippparams (with repeated decoration) and attrdefmap
#define AIPP_CONVERT_LIST_FORMAT(KEY, KEY_TYPE, REQUIRED, ATTR_TYPE) \
  do {                                                               \
    if (REQUIRED) {                                                  \
      KEY_TYPE KEY;                                                  \
      AIPP_GET_ATTR_VALUE(KEY, ATTR_TYPE)                            \
      aipp_params->add_##KEY(KEY);                                   \
    }                                                                \
  } while (0)

#define AIPP_CONVERT_LIST_INT(KEY, REQUIRED) AIPP_CONVERT_LIST_FORMAT(KEY, int64_t, REQUIRED, GeAttrValue::INT)

#define AIPP_CONVERT_LIST_BOOL(KEY, REQUIRED) AIPP_CONVERT_LIST_FORMAT(KEY, bool, REQUIRED, GeAttrValue::BOOL)

#define AIPP_CONVERT_LIST_FLOAT(KEY, REQUIRED) AIPP_CONVERT_LIST_FORMAT(KEY, float, REQUIRED, GeAttrValue::FLOAT)

FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY Status
OpUtils::ConvertAippParams(const GeAttrValue::NamedAttrs &aipp_attr, domi::AippOpParams *aipp_params) {
  GE_CHECK_NOTNULL(aipp_params);
  AIPP_CONVERT_FORMAT_EX(aipp_mode, domi::AippOpParams::AippMode, int32_t, GeAttrValue::INT);

  if (aipp_params->aipp_mode() == domi::AippOpParams::dynamic) {
    AIPP_CONVERT_INT(max_src_image_size);
    AIPP_CONVERT_BOOL(support_rotation);
  } else {
    AIPP_CONVERT_FORMAT_EX(input_format, domi::AippOpParams::InputFormat, int32_t, GeAttrValue::INT);
    AIPP_CONVERT_BOOL(csc_switch);
    AIPP_CONVERT_BOOL(crop);
    AIPP_CONVERT_INT(load_start_pos_w);
    AIPP_CONVERT_INT(load_start_pos_h);
    AIPP_CONVERT_INT(crop_size_w);
    AIPP_CONVERT_INT(crop_size_h);
    AIPP_CONVERT_BOOL(resize);
    AIPP_CONVERT_INT(resize_output_w);
    AIPP_CONVERT_INT(resize_output_h);
    AIPP_CONVERT_BOOL(padding);
    AIPP_CONVERT_INT(left_padding_size);
    AIPP_CONVERT_INT(right_padding_size);
    AIPP_CONVERT_INT(top_padding_size);
    AIPP_CONVERT_INT(bottom_padding_size);
    AIPP_CONVERT_INT(src_image_size_w);
    AIPP_CONVERT_INT(src_image_size_h);
    AIPP_CONVERT_FLOAT(cpadding_value);
    AIPP_CONVERT_BOOL(rbuv_swap_switch);
    AIPP_CONVERT_BOOL(ax_swap_switch);
    AIPP_CONVERT_BOOL(single_line_mode);
    AIPP_CONVERT_INT(mean_chn_0);
    AIPP_CONVERT_INT(mean_chn_1);
    AIPP_CONVERT_INT(mean_chn_2);
    AIPP_CONVERT_FLOAT(min_chn_0);
    AIPP_CONVERT_FLOAT(min_chn_1);
    AIPP_CONVERT_FLOAT(min_chn_2);
    AIPP_CONVERT_LIST_FLOAT(var_reci_chn_0, true);
    AIPP_CONVERT_LIST_FLOAT(var_reci_chn_1, true);
    AIPP_CONVERT_LIST_FLOAT(var_reci_chn_2, true);

    const bool csc_switch = aipp_params->csc_switch();
    AIPP_CONVERT_LIST_INT(matrix_r0c0, csc_switch);
    AIPP_CONVERT_LIST_INT(matrix_r0c1, csc_switch);
    AIPP_CONVERT_LIST_INT(matrix_r0c2, csc_switch);
    AIPP_CONVERT_LIST_INT(matrix_r1c0, csc_switch);
    AIPP_CONVERT_LIST_INT(matrix_r1c1, csc_switch);
    AIPP_CONVERT_LIST_INT(matrix_r1c2, csc_switch);
    AIPP_CONVERT_LIST_INT(matrix_r2c0, csc_switch);
    AIPP_CONVERT_LIST_INT(matrix_r2c1, csc_switch);
    AIPP_CONVERT_LIST_INT(matrix_r2c2, csc_switch);
    AIPP_CONVERT_LIST_INT(output_bias_0, csc_switch);
    AIPP_CONVERT_LIST_INT(output_bias_1, csc_switch);
    AIPP_CONVERT_LIST_INT(output_bias_2, csc_switch);
    AIPP_CONVERT_LIST_INT(input_bias_0, csc_switch);
    AIPP_CONVERT_LIST_INT(input_bias_1, csc_switch);
    AIPP_CONVERT_LIST_INT(input_bias_2, csc_switch);
  }

  return SUCCESS;
}

FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY Status OpUtils::TransferDim(const std::vector<int64_t> &dim,
                                                                             std::vector<int64_t> &dim_vector) {
  size_t input_shape_size = dim.size();
  std::list<uint32_t> new_dim_list;
  for (auto dim_temp : dim) {
    new_dim_list.push_back(dim_temp);
  }
  if (input_shape_size > DIM_DEFAULT_SIZE) {
    dim_vector = dim;
    GELOGI("Dim_vector size is %zu, do not to transfer dim", input_shape_size);
    return SUCCESS;
  }
  switch (input_shape_size) {
    case kDimSizeZero: {
      new_dim_list.push_back(1);
      new_dim_list.push_back(1);
      new_dim_list.push_back(1);
      new_dim_list.push_back(1);
      break;
    }
    case kDimSizeOne: {
      new_dim_list.push_front(1);
      new_dim_list.push_back(1);
      new_dim_list.push_back(1);
      break;
    }
    case kDimSizeTwo: {
      new_dim_list.push_front(1);
      new_dim_list.push_back(1);
      break;
    }
    case kDimSizeThree: {
      new_dim_list.push_front(1);
      break;
    }
    default:
      GELOGI("Invalid input_shape_size.");
      break;
  }

  dim_vector.clear();
  for (auto dims : new_dim_list) {
    dim_vector.push_back(dims);
  }
  return SUCCESS;
}

void OpUtils::SliceData(std::vector<char *> &input, int64_t chunk_size, std::vector<char *> &output, int64_t begin,
                        int64_t out_dim, int64_t stride) {
  char *slice = nullptr;
  for (size_t j = 0; j < input.size(); j++) {
    slice = input[j] + sizeof(int32_t) * begin * chunk_size;
    for (int64_t i = 0; i < out_dim; i++) {
      output.push_back(slice + sizeof(int32_t) * i * chunk_size * stride);
    }
  }
}

FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY Status OpUtils::SetOutputSliceData(
  void *data, int64_t data_size, int32_t data_type, std::vector<int64_t> &input_dims, std::vector<int64_t> &begin,
  std::vector<int64_t> &output_dims, GeTensor *output, std::vector<int64_t> &stride) {
  GE_CHECK_NOTNULL(data);
  GE_CHECK_NOTNULL(output);
  std::vector<char *> chunk_input;
  std::vector<char *> chunk_output;
  chunk_input.push_back(reinterpret_cast<char *>(data));
  int64_t chunk_size = data_size;
  int dim_size = static_cast<int>(input_dims.size());
  for (int i = 0; i < dim_size; i++) {
    int64_t begin_i = begin[i];
    int64_t size_i = output_dims[i];
    int64_t dim_i = input_dims[i];
    int64_t stride_i = stride[i];
    GE_CHK_BOOL_EXEC((dim_i != 0), return PARAM_INVALID, "Dim_i can't be 0.");
    chunk_size = chunk_size / dim_i;

    if (i % kSliceDataNum == 0) {
      SliceData(chunk_input, chunk_size, chunk_output, begin_i, size_i, stride_i);
      chunk_input.clear();
    } else {
      SliceData(chunk_output, chunk_size, chunk_input, begin_i, size_i, stride_i);
      chunk_output.clear();
    }
  }

  size_t out_size = chunk_input.size() + chunk_output.size();
  GE_CHK_BOOL_RET_STATUS(out_size > 0, FAILED, "Out_size <= 0");

  if (data_type == DT_FLOAT) {
    float *output_data = new (std::nothrow) float[out_size]();
    GE_CHECK_NOTNULL(output_data);
    if (!chunk_input.empty()) {
      for (size_t j = 0; j < out_size; j++) {
        float *value = reinterpret_cast<float *>(chunk_input[j]);
        output_data[j] = *value;
      }
    } else {
      for (size_t j = 0; j < out_size; j++) {
        float *value = reinterpret_cast<float *>(chunk_output[j]);
        output_data[j] = *value;
      }
    }
    (void)output->SetData(reinterpret_cast<uint8_t *>(output_data), out_size * sizeof(float));
    // output_data != nullptr and out_size > 0, SetData always return success, no need to check value
    GE_DELETE_NEW_ARRAY(output_data);
  } else if (data_type == DT_INT32) {
    int *output_data = new (std::nothrow) int[out_size]();
    GE_CHECK_NOTNULL(output_data);

    if (!chunk_input.empty()) {
      for (size_t j = 0; j < out_size; j++) {
        int *value = reinterpret_cast<int *>(chunk_input[j]);
        output_data[j] = *value;
      }
    } else {
      for (size_t j = 0; j < out_size; j++) {
        int *value = reinterpret_cast<int *>(chunk_output[j]);
        output_data[j] = *value;
      }
    }
    (void)output->SetData(reinterpret_cast<uint8_t *>(output_data), out_size * sizeof(int));
    // output_data != nullptr and out_size > 0, SetData always return success, no need to check value
    GE_DELETE_NEW_ARRAY(output_data);
  } else {
    GELOGE(FAILED, "Data type of Slice OP must be float or int32.");
    return FAILED;
  }

  return SUCCESS;
}

FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY void OpUtils::TransDataHWCK2KCHW(const void *input, int64_t h,
                                                                                  int64_t w, int64_t c, int64_t k,
                                                                                  void **output) {
  if (input == nullptr) {
    return;
  }
  if (output == nullptr) {
    return;
  }
  const char *w_data = (const char *)input;

  int64_t count = h * w * c * k;
  GE_IF_BOOL_EXEC(count <= 0, GELOGW("Count value must be greater than 0, but count = %ld", count); return );
  float *buf = new (std::nothrow) float[count]();
  GE_RT_VOID_CHECK_NOTNULL(buf);
  float *src_buff = nullptr;
  float *dst_buff = nullptr;
  for (int h_i = 0; h_i < h; ++h_i) {
    for (int w_i = 0; w_i < w; ++w_i) {
      for (int c_i = 0; c_i < c; ++c_i) {
        for (int k_i = 0; k_i < k; ++k_i) {
          src_buff = reinterpret_cast<float *>(const_cast<char *>(w_data)) +
                     ((h_i * w * c * k) + (w_i * c * k) + (c_i * k) + (k_i));

          dst_buff = buf + ((k_i * c * h * w) + (c_i * h * w) + (h_i * w) + (w_i));

          *dst_buff = *src_buff;
        }
      }
    }
  }
  *output = buf;
}

FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY void OpUtils::TransDataKCHW2HWCK(const void *input, int64_t k,
                                                                                  int64_t c, int64_t h, int64_t w,
                                                                                  void *output) {
  if ((input == nullptr) || (output == nullptr)) {
    GELOGD("%s[%d]: input param is nullptr.", __FILE__, __LINE__);
    return;
  }

  const char *w_data = (const char *)input;

  float *buf = reinterpret_cast<float *>(output);
  float *src_buff = nullptr;
  float *dst_buff = nullptr;
  for (int k_i = 0; k_i < k; ++k_i) {
    for (int c_i = 0; c_i < c; ++c_i) {
      for (int h_i = 0; h_i < h; ++h_i) {
        for (int w_i = 0; w_i < w; ++w_i) {
          src_buff = reinterpret_cast<float *>(const_cast<char *>(w_data)) +
                     ((k_i * c * h * w) + (c_i * h * w) + (h_i * w) + (w_i));

          dst_buff = buf + ((h_i * w * c * k) + (w_i * c * k) + (c_i * k) + (k_i));

          *dst_buff = *src_buff;
        }
      }
    }
  }
}

vector<ConstGeTensorPtr> OpUtils::GetWeights(const ge::Node &node) { return OpDescUtils::GetWeights(node); }

FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY vector<ConstGeTensorPtr> OpUtils::GetWeights(ge::ConstNodePtr node) {
  return OpDescUtils::GetWeights(node);
}

FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY vector<GeTensorPtr> OpUtils::MutableWeights(const ge::Node &node) {
  return OpDescUtils::MutableWeights(node);
}

FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY vector<GeTensorPtr> OpUtils::MutableWeights(const ge::NodePtr node) {
  return OpDescUtils::MutableWeights(node);
}

FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY Status OpUtils::SetWeights(ge::Node &node,
                                                                            const vector<ge::GeTensorPtr> &weights) {
  return OpDescUtils::SetWeights(node, weights);
}

FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY Status OpUtils::SetWeights(ge::NodePtr node,
                                                                            const vector<ge::GeTensorPtr> &weights) {
  return OpDescUtils::SetWeights(node, weights);
}

// The caller guarantees that the input sensor is constant
FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY Status
OpUtils::GetShapeDataFromConstTensor(const ConstGeTensorPtr &tensor, DataType type, std::vector<int64_t> &dims) {
  if (tensor == nullptr) {
    GELOGE(PARAM_INVALID, "Input tensor is nullptr");
    return PARAM_INVALID;
  }

  // If the tensor data is a vector, the shape dimension must be 1
  if (tensor->GetTensorDesc().GetShape().GetDims().size() > 1) {
    GELOGE(PARAM_INVALID, "The dimension of the input tensor shape cannot be more than 1, it is %zu",
           tensor->GetTensorDesc().GetShape().GetDims().size());
    return PARAM_INVALID;
  }

  if (type == DT_INT32) {
    int32_t *shape_data = const_cast<int32_t *>(reinterpret_cast<const int32_t *>(tensor->GetData().GetData()));
    GE_CHECK_NOTNULL(shape_data);
    size_t dims_num = tensor->GetData().size() / sizeof(int32_t);
    for (size_t i = 0; i < dims_num; i++) {
      dims.push_back(static_cast<int64_t>(shape_data[i]));
    }
  } else if (type == DT_INT64) {
    int64_t *shape_data = const_cast<int64_t *>(reinterpret_cast<const int64_t *>(tensor->GetData().GetData()));
    GE_CHECK_NOTNULL(shape_data);
    size_t dims_num = tensor->GetData().size() / sizeof(int64_t);
    for (size_t i = 0; i < dims_num; i++) {
      dims.push_back(shape_data[i]);
    }
  } else {
    GELOGE(PARAM_INVALID, "Data type only can be DT_INT32 or DT_INT64. type is %s",
           TypeUtils::DataTypeToSerialString(type).c_str());
    return PARAM_INVALID;
  }

  return SUCCESS;
}

uint32_t OpUtils::GetRealDimCnt(const GeTensorDesc &tensor_desc) {
  uint32_t real_dim_cnt = 0;
  domi::Status ret = TensorUtils::GetRealDimCnt(tensor_desc, real_dim_cnt);
  return (ret == domi::SUCCESS) ? real_dim_cnt : 0;
}
}  // namespace ge
