/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/expressions/ScalarLiteral.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#include "expressions/scalar/ScalarLiteral.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "types/Type.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace expressions {

const Type& ScalarLiteral::getValueType() const {
  return value_type_;
}

ExpressionPtr ScalarLiteral::copyWithNewChildren(
    const std::vector<ExpressionPtr> &new_children) const {
  DCHECK_EQ(new_children.size(), children().size());
  return ScalarLiteral::Create(value_, value_type_);
}

::quickstep::Scalar *ScalarLiteral::concretize(
    const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const {
  return new ::quickstep::ScalarLiteral(value_, value_type_);
}

void ScalarLiteral::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  inline_field_names->push_back("value");
  if (value_.isNull()) {
    inline_field_values->push_back("NULL");
  } else {
    inline_field_values->push_back(value_type_.printValueToString(value_));
  }

  inline_field_names->push_back("type");
  inline_field_values->push_back(getValueType().getName());
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep