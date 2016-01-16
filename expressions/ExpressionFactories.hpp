/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_EXPRESSIONS_EXPRESSION_FACTORIES_HPP_
#define QUICKSTEP_EXPRESSIONS_EXPRESSION_FACTORIES_HPP_

#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;
class Predicate;
class Scalar;

namespace serialization {
class Predicate;
class Scalar;
}  // namespace serialization

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief All-static factory object that provides access to Predicates.
 **/
class PredicateFactory {
 public:
  /**
   * @brief Get a pointer to a Predicate from its serialized Protocol Buffer
   *        form.
   *
   * @param proto The Protocol Buffer representation of a Predicate object,
   *        originally generated by getProto().
   * @param database The Database to resolve relation and attribute references
   *        in.
   * @return A new Predicate reconstructed from the supplied Protocol Buffer.
   **/
  static Predicate* ReconstructFromProto(const serialization::Predicate &proto,
                                         const CatalogDatabase &database);

  /**
   * @brief Check whether a serialization::Predicate is fully-formed and
   *        all parts are valid.
   *
   * @param proto A serialized Protocol Buffer representation of a Predicate,
   *        originally generated by getProto().
   * @param database The Database to resolve relation and attribute references
   *        in.
   * @return Whether proto is fully-formed and valid.
   **/
  static bool ProtoIsValid(const serialization::Predicate &proto,
                           const CatalogDatabase &database);

 private:
  // Undefined default constructor. Class is all-static and should not be
  // instantiated.
  PredicateFactory();

  DISALLOW_COPY_AND_ASSIGN(PredicateFactory);
};

/**
 * @brief All-static factory object that provides access to Scalars.
 **/
class ScalarFactory {
 public:
  /**
   * @brief Get a pointer to a Scalar from its serialized Protocol Buffer form.
   *
   * @param proto The Protocol Buffer representation of a Scalar object,
   *        originally generated by getProto().
   * @param database The Database to resolve relation and attribute references
   *        in.
   * @return A new Scalar reconstructed from the supplied Protocol Buffer.
   **/
  static Scalar* ReconstructFromProto(const serialization::Scalar &proto,
                                      const CatalogDatabase &database);

  /**
   * @brief Check whether a serialization::Scalar is fully-formed and
   *        all parts are valid.
   *
   * @param proto A serialized Protocol Buffer representation of a Scalar,
   *        originally generated by getProto().
   * @param database The Database to resolve relation and attribute references
   *        in.
   * @return Whether proto is fully-formed and valid.
   **/
  static bool ProtoIsValid(const serialization::Scalar &proto,
                           const CatalogDatabase &database);

 private:
  // Undefined default constructor. Class is all-static and should not be
  // instantiated.
  ScalarFactory();

  DISALLOW_COPY_AND_ASSIGN(ScalarFactory);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_EXPRESSION_FACTORIES_HPP_