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

#ifndef __CCE_RUNTIME_DEVICE_H__
#define __CCE_RUNTIME_DEVICE_H__

#include "base.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef struct tagRTDeviceInfo {
  uint8_t env_type; /* 0: FPGA  1: EMU 2: ESL */
  uint32_t ctrl_cpu_ip;
  uint32_t ctrl_cpu_id;
  uint32_t ctrl_cpu_core_num;
  uint32_t ctrl_cpu_endian_little;
  uint32_t ts_cpu_core_num;
  uint32_t ai_cpu_core_num;
  uint32_t ai_core_num;
  uint32_t ai_cpu_core_id;
  uint32_t ai_core_id;
  uint32_t aicpu_occupy_bitmap;
  uint32_t hardware_version;
#ifdef DRIVER_NEW_API
  uint32_t ts_num;
#endif
} rtDeviceInfo_t;

/**
 * @ingroup dvrt_dev
 * @brief get total device number.
 * @param [in|out] count the device number
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_NO_DEVICE for can not find any device
 */
RTS_API rtError_t rtGetDeviceCount(int32_t *count);
/**
 * @ingroup dvrt_dev
 * @brief get device ids
 * @param [in|out] get details of device ids
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_DRV_ERR for error
 */
RTS_API rtError_t rtGetDeviceIDs(uint32_t *devices, uint32_t len);
/**
 * @ingroup dvrt_dev
 * @brief get total device infomation.
 * @param [in] device   the device id
 * @param [out] info   the device info
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_NO_DEVICE for can not find any device
 */
RTS_API rtError_t rtGetDeviceInfo(int32_t device, rtDeviceInfo_t *info);

/**
 * @ingroup dvrt_dev
 * @brief set target device for current thread
 * @param [int] device   the device id
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_DEVICE for can not match ID and device
 */
RTS_API rtError_t rtSetDevice(int32_t device);

/**
 * @ingroup dvrt_dev
 * @brief set target device for current thread
 * @param [int] device   the device id
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_DEVICE for can not match ID and device
 */
RTS_API rtError_t rtSetDeviceEx(int32_t device);

/**
 * @ingroup dvrt_dev
 * @brief get Index by phyId.
 * @param [in] phyId   the physical device id
 * @param [out] devIndex   the logic device id
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_NO_DEVICE for can not find any device
 */
RTS_API rtError_t rtGetDeviceIndexByPhyId(uint32_t phyId, uint32_t *devIndex);

/**
 * @ingroup dvrt_dev
 * @brief get phyId by Index.
 * @param [in] devIndex   the logic device id
 * @param [out] phyId   the physical device id
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_NO_DEVICE for can not find any device
 */
RTS_API rtError_t rtGetDevicePhyIdByIndex(uint32_t devIndex, uint32_t *phyId);

/**
 * @ingroup dvrt_dev
 * @brief enable direction:devIdDes---->phyIdSrc.
 * @param [in] devIdDes   the logical device id
 * @param [in] phyIdSrc   the physical device id
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_NO_DEVICE for can not find any device
 */
RTS_API rtError_t rtEnableP2P(uint32_t devIdDes, uint32_t phyIdSrc);

/**
 * @ingroup dvrt_dev
 * @brief disable direction:devIdDes---->phyIdSrc.
 * @param [in] devIdDes   the logical device id
 * @param [in] phyIdSrc   the physical device id
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_NO_DEVICE for can not find any device
 */
RTS_API rtError_t rtDisableP2P(uint32_t devIdDes, uint32_t phyIdSrc);

/**
 * @ingroup dvrt_dev
 * @brief get target device of current thread
 * @param [in|out] device   the device id
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtGetDevice(int32_t *device);

/**
 * @ingroup dvrt_dev
 * @brief reset all opened device
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_DEVICE if no device set
 */
RTS_API rtError_t rtDeviceReset(int32_t device);

/**
 * @ingroup dvrt_dev
 * @brief reset opened device
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_DEVICE if no device set
 */
RTS_API rtError_t rtDeviceResetEx(int32_t device);

/**
 * @ingroup dvrt_dev
 * @brief get total device infomation.
 * @param [in] device   the device id
 * @param [in] type     limit type RT_LIMIT_TYPE_LOW_POWER_TIMEOUT=0
 * @param [in] value    limit value
 * @param [out] info   the device info
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_NO_DEVICE for can not find any device
 */
RTS_API rtError_t rtDeviceSetLimit(int32_t device, rtLimitType_t type, uint32_t value);

/**
 * @ingroup dvrt_dev
 * @brief Wait for compute device to finish
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_DEVICE if no device set
 */
RTS_API rtError_t rtDeviceSynchronize(void);

/**
 * @ingroup dvrt_dev
 * @brief get priority range of current device
 * @param [in|out] leastPriority   least priority
 * @param [in|out] greatestPriority   greatest priority
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtDeviceGetStreamPriorityRange(int32_t *leastPriority, int32_t *greatestPriority);

/**
 * @ingroup dvrt_dev
 * @brief Set exception handling callback function
 * @param [in] callback   rtExceptiontype
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtSetExceptCallback(rtErrorCallback callback);

/**
 * @ingroup dvrt_dev
 * @brief Setting Scheduling Type of Graph
 * @param [in] tsId   the ts id
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtSetTSDevice(uint32_t tsId);

#ifdef __cplusplus
}
#endif

#endif  // __CCE_RUNTIME_DEVICE_H__
