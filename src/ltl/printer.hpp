/*!
 * \file src/ltl/printer.hpp
 * \brief Printer for LTL formulas.
 * \author SiriusNEO
 */

#include "ast.hpp"

#ifndef LTL_PRINTER_H_
#define LTL_PRINTER_H_

namespace alice {

namespace ltl {

class LTLPrinter : public LTLVisitor {
 public:
  std::string str_;

  LTLPrinter() : str_(), LTLVisitor() {}

  virtual void _visit(Negation* node);

  virtual void _visit(Conjunction* node);

  virtual void _visit(Disjunction* node);

  virtual void _visit(Implication* node);

  virtual void _visit(Next* node);

  virtual void _visit(Always* node);

  virtual void _visit(Eventually* node);

  virtual void _visit(Until* node);

  virtual void _visit(AtomicProposition* node);

  virtual void _visit(True* node);
};

}  // namespace ltl

}  // namespace alice

#endif  // LTL_PRINTER_H_
