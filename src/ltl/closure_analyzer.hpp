/*!
 * \file src/ltl/closure_analyzer.hpp
 * \brief Get the closure of a LTL formula and do some extra analysis.
 * \author SiriusNEO
 */

#include <set>

#include "ast.hpp"

#ifndef LTL_CLOSURE_ANALYZER_HPP_
#define LTL_CLOSURE_ANALYZER_HPP_

namespace alice {

namespace ltl {

#define CHECK_FAILED  \
  check_flag = false; \
  break

class ClosureAnalyzer : public LTLVisitor {
 public:
  std::vector<LTLFormula*> closure_;
  std::vector<std::set<LTLFormula*>*> elementary_sets_;

  explicit ClosureAnalyzer(std::set<std::string>* AP) : AP_(AP) {}

  ~ClosureAnalyzer();

  void getElementarySet();

  void show(std::ostream& os) const;

  virtual void _visit(Negation* node) override;

  virtual void _visit(Conjunction* node) override;

  virtual void _visit(Next* node) override;

  virtual void _visit(Until* node) override;

  virtual void _visit(AtomicProposition* node) override;

  virtual void _visit(True* node) override;

 private:
  bool has_true_ = false;
  bool set_ap_ = false;

  std::set<std::string>* AP_;

  // for memory manage
  std::set<LTLFormula*> created_;

  // for enumrating
  std::vector<LTLFormula*> enumerated_;

  void putIntoClosureNormal(LTLFormula* formula);

  void enumerateAndCheckElementarySet(int depth);

  bool inEnumerated(LTLFormula* formula);
};

}  // namespace ltl

}  // namespace alice

#endif  // LTL_CLOSURE_ANALYZER_HPP_
