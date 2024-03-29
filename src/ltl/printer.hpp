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

  virtual void _visit(Negation* node) override;

  virtual void _visit(Conjunction* node) override;

  virtual void _visit(Next* node) override;

  virtual void _visit(Until* node) override;

  virtual void _visit(AtomicProposition* node) override;

  virtual void _visit(True* node) override;
};

std::string toString(LTLFormula* formula);

}  // namespace ltl

}  // namespace alice

#endif  // LTL_PRINTER_H_
