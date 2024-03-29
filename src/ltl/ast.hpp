/*!
 * \file src/ltl/ast.hpp
 * \brief Abstract Syntax Tree for LTL formulas.
 * \note Here we adopt a minimal operators set of LTL formulas.
 *       For operators like disjunction, implication, always, eventually,
 *       we will parse them to the combination of other operators.
 * \author SiriusNEO
 */

#include <iostream>
#include <set>
#include <vector>

#ifndef LTL_AST_H_
#define LTL_AST_H_

namespace alice {

namespace ltl {

class LTLVisitor;

class LTLFormula {
 public:
  LTLFormula *left_son_, *right_son_;

  LTLFormula(LTLFormula* left_son, LTLFormula* right_son)
      : left_son_(left_son), right_son_(right_son) {}

  virtual void accept(LTLVisitor* visitor);
};

class Negation : public LTLFormula {
 public:
  explicit Negation(LTLFormula* phi) : LTLFormula(phi, nullptr) {}

  virtual void accept(LTLVisitor* visitor);
};

class Conjunction : public LTLFormula {
 public:
  Conjunction(LTLFormula* phi1, LTLFormula* phi2) : LTLFormula(phi1, phi2) {}

  virtual void accept(LTLVisitor* visitor);
};

// class Disjunction : public LTLFormula {
//  public:
//   Disjunction(LTLFormula* phi1, LTLFormula* phi2) : LTLFormula(phi1, phi2) {}

//   virtual void accept(LTLVisitor* visitor);
// };

// class Implication : public LTLFormula {
//  public:
//   Implication(LTLFormula* phi1, LTLFormula* phi2) : LTLFormula(phi1, phi2) {}

//   virtual void accept(LTLVisitor* visitor);
// };

class Next : public LTLFormula {
 public:
  explicit Next(LTLFormula* phi) : LTLFormula(phi, nullptr) {}

  virtual void accept(LTLVisitor* visitor);
};

// class Always : public LTLFormula {
//  public:
//   explicit Always(LTLFormula* phi) : LTLFormula(phi, nullptr) {}

//   virtual void accept(LTLVisitor* visitor);
// };

// class Eventually : public LTLFormula {
//  public:
//   explicit Eventually(LTLFormula* phi) : LTLFormula(phi, nullptr) {}

//   virtual void accept(LTLVisitor* visitor);
// };

class Until : public LTLFormula {
 public:
  Until(LTLFormula* phi1, LTLFormula* phi2) : LTLFormula(phi1, phi2) {}

  virtual void accept(LTLVisitor* visitor);
};

class AtomicProposition : public LTLFormula {
 public:
  std::string* ap_name_;

  AtomicProposition(std::string* ap_name)
      : ap_name_(ap_name), LTLFormula(nullptr, nullptr) {}

  virtual void accept(LTLVisitor* visitor);
};

class True : public LTLFormula {
 public:
  True() : LTLFormula(nullptr, nullptr) {}

  virtual void accept(LTLVisitor* visitor);
};

class LTLVisitor {
 public:
  void visit(LTLFormula* node);

  // The following methods are public for overriding.
  // For users (who use the pass), just use visit instead of the following
  // methods.

  virtual void _visit(Negation* node) = 0;

  virtual void _visit(Conjunction* node) = 0;

  virtual void _visit(Next* node) = 0;

  virtual void _visit(Until* node) = 0;

  virtual void _visit(AtomicProposition* node) = 0;

  virtual void _visit(True* node) = 0;
};

bool equal(LTLFormula* lhs, LTLFormula* rhs);

bool in(LTLFormula* formula, const std::vector<LTLFormula*>& container);

bool in(LTLFormula* formula, const std::set<LTLFormula*>& container);

}  // namespace ltl

}  // namespace alice

#endif  // LTL_AST_H_
