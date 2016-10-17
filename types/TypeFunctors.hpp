/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#ifndef QUICKSTEP_TYPES_TYPE_FUNCTORS_HPP_
#define QUICKSTEP_TYPES_TYPE_FUNCTORS_HPP_

#include <cstddef>
#include <functional>
#include <vector>

namespace quickstep {

class Type;

/** \addtogroup Types
 *  @{
 */

typedef std::function<std::size_t (const void *)> UntypedHashFunctor;
UntypedHashFunctor MakeUntypedHashFunctor(const Type *type);
UntypedHashFunctor MakeUntypedHashFunctor(const std::vector<const Type *> &types);

typedef std::function<bool (const void *, const void *)> UntypedEqualityFunctor;
UntypedEqualityFunctor MakeUntypedEqualityFunctor(const Type *type);
UntypedEqualityFunctor MakeUntypedEqualityFunctor(const std::vector<const Type *> &types);

typedef std::function<void (void *, const void *)> UntypedCopyFunctor;
UntypedCopyFunctor MakeUntypedCopyFunctor(const Type *type);

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_TYPE_FUNCTORS_HPP_