/*
All modification made by Cambricon Corporation: © 2022 Cambricon Corporation
All rights reserved.
All other contributions:
Copyright (c) 2014--2022, the respective contributors
All rights reserved.
For the list of contributors go to https://github.com/pytorch/pytorch/graphs/contributors
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "jit/codegen/convertion/convert.h"

namespace torch_mlu {
namespace jit {
namespace codegen {
namespace convertion {

static auto registry = Registerer()
    .op("aten::size.int(Tensor self, int dim) -> int",
        [](codegen::MagicmindHandle* handle, const torch::jit::Node* node,
           torch::jit::Stack& params) -> bool {
          auto input_tensor = codegen::getOrCreateITensor(handle, params[0]);
          auto dim = codegen::getOrCreateITensor(handle, params[1]);
          auto output_dtype = input_tensor->GetDataType();
          auto sizeop = handle->network->AddIShapeNode(input_tensor, dim);
          MM_CHECK(sizeop->SetOutputType(0, magicmind::DataType::INT32));
          auto output_tensor = sizeop->GetOutput(0);
          handle->bindingValueAndIvalue(
              node->outputs()[0], codegen::bindITensor(output_tensor));
          return true;
        });

}  // namespace convertion
}  // namespace codegen
}  // namespace jit
}  // namespace torch_mlu