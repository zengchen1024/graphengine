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

#ifndef HOST_INNER_INC_DATA_COMMON_H_
#define HOST_INNER_INC_DATA_COMMON_H_

namespace tdt {
#ifndef TDT_DATA_TYPE
#define TDT_DATA_TYPE

/**
 * @ingroup  Tdt data.
 *
 * Tdt data type.
 */
enum TdtDataType {
  TDT_IMAGE_LABEL = 0, /**< Image label*/
  TDT_T_R,
  TDT_DATA_LABEL,      /**< Data label*/
  TDT_END_OF_SEQUENCE, /**< End of Sequence*/
  TDT_TENSOR,          /**< Tensor*/
  TDT_ABNORMAL,        /**< ABNORMAL*/
  TDT_DATATYPE_MAX     /**< Max*/
};
#endif

/**
 * @ingroup  Tdt data.
 *
 * Tdt push data between host and device.
 */
struct TdtDataItem {
  TdtDataType dataType_;          /**< Input data type*/
  uint64_t label_;                /**< Input data label*/
  uint64_t dataLen_;              /**< Input data type length*/
  std::string tensorShape_;       /**< Tensor shape*/
  std::string tensorType_;        /**< Tensor type*/
  uint32_t cnt_;                  /**< Data  count*/
  uint32_t currentCnt_;           /**< Data  current count*/
  uint64_t index_;                /**< Data  inde*/
  std::string tensorName_;        /**< Tensor  name*/
  uint64_t md5ValueHead_;         /**< Data  md5*/
  uint64_t md5ValueTail_;         /**< Data  md5*/
  std::shared_ptr<void> dataPtr_; /**< Data  pointer*/
  std::string headMD5_;           /**< MD5 header, 8byte*/
  std::string tailMD5_;           /**< MD5 tail, 8byte*/
};

/**
 * @ingroup  Tdt data.
 *
 * Tdt push data for queuedataset ort mind-data.
 */
struct DataItem {
  TdtDataType dataType_;          /**< Input data type*/
  std::string tensorName_;        /**< Tensor  name*/
  std::string tensorShape_;       /**< Tensor shape*/
  std::string tensorType_;        /**< Tensor type*/
  uint64_t dataLen_;              /**< Input data type length*/
  std::shared_ptr<void> dataPtr_; /**< Data  pointer*/
};
}  // namespace tdt
#endif  // HOST_INNER_INC_DATA_COMMON_H_
