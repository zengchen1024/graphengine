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

#ifndef INC_EXTERNAL_REGISTER_REGISTER_H_
#define INC_EXTERNAL_REGISTER_REGISTER_H_

#include <functional>
#include <initializer_list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <unordered_map>
#include <vector>

#include "graph/operator.h"
#include "register/register_error_codes.h"
#include "register/register_fmk_types.h"
#include "register/register_types.h"

using std::make_shared;
using std::map;
using std::pair;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::vector;

namespace ge {
class Operator;
class TensorDesc;
class Tensor;
class TBEPluginManager;
}  // namespace ge

namespace domi {
Status AutoMappingFn(const google::protobuf::Message *op_src, ge::Operator &op);
Status AutoMappingFnDynamic(const google::protobuf::Message *op_src, ge::Operator &op,
                            std::map<std::string, std::pair<std::string, std::string>> dynamic_name_attr_value,
                            int in_pos = -1, int out_pos = -1);
using google::protobuf::Message;
class OpRegistrationDataImpl;

using ParseParamFunc = std::function<domi::Status(const google::protobuf::Message *, ge::Operator &)>;

class FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY OpRegistrationData {
 public:
  OpRegistrationData(const std::string &om_optype);

  ~OpRegistrationData();

  OpRegistrationData &FrameworkType(const domi::FrameworkType &fmk_type);

  OpRegistrationData &OriginOpType(const std::initializer_list<std::string> &ori_optype_list);

  OpRegistrationData &OriginOpType(const std::string &ori_optype);

  OpRegistrationData &ParseParamsFn(const ParseParamFunc &parseParamFn);

  OpRegistrationData &ImplyType(const domi::ImplyType &imply_type);

  OpRegistrationData &DelInputWithCond(int inputIdx, const std::string &attrName, bool attrValue);

  domi::ImplyType GetImplyType() const;
  std::string GetOmOptype() const;
  std::set<std::string> GetOriginOpTypeSet() const;
  domi::FrameworkType GetFrameworkType() const;
  ParseParamFunc GetParseParamFn() const;

 private:
  std::shared_ptr<OpRegistrationDataImpl> impl_;
  friend class OpRegistry;
  friend class OpRegistrationTbe;
  friend class ge::TBEPluginManager;
};

class FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY OpReceiver {
 public:
  OpReceiver(OpRegistrationData &reg_data);
  ~OpReceiver() {}
};

#define REGISTER_CUSTOM_OP(name) REGISTER_CUSTOM_OP_UNIQ_HELPER(__COUNTER__, name)
#define REGISTER_CUSTOM_OP_UNIQ_HELPER(ctr, name) REGISTER_CUSTOM_OP_UNIQ(ctr, name)
#define REGISTER_CUSTOM_OP_UNIQ(ctr, name) \
  static OpReceiver register_op##ctr __attribute__((unused)) = OpRegistrationData(name)
}  // namespace domi

namespace ge {
using OpRegistrationData = domi::OpRegistrationData;
using OpReceiver = domi::OpReceiver;
}  // namespace ge
#endif  // INC_EXTERNAL_REGISTER_REGISTER_H_
