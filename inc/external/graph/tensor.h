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

#ifndef INC_EXTERNAL_GRAPH_TENSOR_H_
#define INC_EXTERNAL_GRAPH_TENSOR_H_

#include <atomic>
#include <memory>
#include <string>
#include <vector>

#include "external/graph/ge_error_codes.h"
#include "external/graph/types.h"

namespace ge {
class ShapeImpl;
class GE_FUNC_DEV_VISIBILITY GE_FUNC_HOST_VISIBILITY Shape {
 public:
  Shape();
  ~Shape() = default;
  explicit Shape(const std::vector<int64_t> &dims);

  size_t GetDimNum() const;
  // If the idx is invalid, return 0
  int64_t GetDim(size_t idx) const;
  graphStatus SetDim(size_t idx, int64_t value);
  std::vector<int64_t> GetDims() const;
  int64_t GetShapeSize() const;

 private:
  std::shared_ptr<ShapeImpl> impl_;
};

class TensorDescImpl;
class GE_FUNC_DEV_VISIBILITY GE_FUNC_HOST_VISIBILITY TensorDesc {
 public:
  TensorDesc();
  ~TensorDesc() = default;
  explicit TensorDesc(Shape shape, Format format = FORMAT_ND, DataType dt = DT_FLOAT);
  // Copy
  TensorDesc(const TensorDesc &desc);
  // Move
  TensorDesc(TensorDesc &&desc);
  // Copy
  TensorDesc &operator=(const TensorDesc &desc);
  // Move
  TensorDesc &operator=(TensorDesc &&desc);

  void Update(const Shape &shape, Format format = FORMAT_ND, DataType dt = DT_FLOAT);
  Shape GetShape() const;
  void SetShape(const Shape &shape);

  Format GetFormat() const;
  void SetFormat(Format format);

  Shape GetOriginShape() const;
  void SetOriginShape(const Shape &originShape);

  Format GetOriginFormat() const;
  void SetOriginFormat(Format originFormat);

  DataType GetDataType() const;
  void SetDataType(DataType dt);

  std::string GetName() const;
  void SetName(const std::string &name);

  // Attr acess
  void SetSize(int64_t size);
  int64_t GetSize() const;

  int64_t GetRealDimCnt() const;
  void SetRealDimCnt(const int64_t realDimCnt);

 private:
  std::shared_ptr<TensorDescImpl> impl;
};

class TensorImpl;
class GE_FUNC_DEV_VISIBILITY GE_FUNC_HOST_VISIBILITY Tensor {
 public:
  Tensor();
  ~Tensor() = default;
  explicit Tensor(const TensorDesc &tensorDesc);
  Tensor(const TensorDesc &tensorDesc, const std::vector<uint8_t> &data);
  Tensor(const TensorDesc &tensorDesc, const uint8_t *data, size_t size);
  Tensor(TensorDesc &&tensorDesc, std::vector<uint8_t> &&data);

  TensorDesc GetTensorDesc() const;
  graphStatus SetTensorDesc(const TensorDesc &tensorDesc);

  const uint8_t *GetData() const;
  uint8_t *GetData();
  size_t GetSize() const;

  graphStatus SetData(std::vector<uint8_t> &&data);
  graphStatus SetData(const std::vector<uint8_t> &data);
  graphStatus SetData(const uint8_t *data, size_t size);
  graphStatus SetData(const std::string &data);
  graphStatus SetData(const std::vector<std::string> &data);
  graphStatus IsValid();

  Tensor Clone() const;

 private:
  std::shared_ptr<TensorImpl> impl;
  friend class TensorAdapter;
};
}  // namespace ge

#endif  // INC_EXTERNAL_GRAPH_TENSOR_H_
