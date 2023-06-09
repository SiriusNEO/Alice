/*!
 * \file src/ltl/tree_deleter.hpp
 * \brief Recursively delete the LTL AST.
 * \author SiriusNEO
 */

#include "ast.hpp"

#ifndef LTL_TREE_DELETER_HPP_
#define LTL_TREE_DELETER_HPP_

namespace alice {

namespace ltl {

class TreeDeleter : public LTLVisitor {
 public:
  virtual void _visit(Negation* node) override;

  virtual void _visit(Conjunction* node) override;

  virtual void _visit(Next* node) override;

  virtual void _visit(Until* node) override;

  virtual void _visit(AtomicProposition* node) override;

  virtual void _visit(True* node) override;
};

}  // namespace ltl

}  // namespace alice

#endif  // LTL_TREE_DELETER_HPP_
