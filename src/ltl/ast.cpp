#include "ast.hpp"

#include "../utils/instanceof.hpp"
#include "../utils/logging.hpp"

namespace alice {

namespace ltl {

void LTLFormula::accept(LTLVisitor* visitor) {
  std::cerr << "Unreachable." << std::endl;
  exit(1);
}

void Negation::accept(LTLVisitor* visitor) { visitor->_visit(this); }

void Conjunction::accept(LTLVisitor* visitor) { visitor->_visit(this); }

void Next::accept(LTLVisitor* visitor) { visitor->_visit(this); }

void Until::accept(LTLVisitor* visitor) { visitor->_visit(this); }

void AtomicProposition::accept(LTLVisitor* visitor) { visitor->_visit(this); }

void True::accept(LTLVisitor* visitor) { visitor->_visit(this); }

void LTLVisitor::visit(LTLFormula* node) { node->accept(this); }

bool equal(LTLFormula* lhs, LTLFormula* rhs) {
  if (utils:: instanceof <Negation>(lhs)) {
    if (!utils:: instanceof <Negation>(rhs)) return false;
    return equal(lhs->left_son_, rhs->left_son_);
  }

  if (utils:: instanceof <Conjunction>(lhs)) {
    if (!utils:: instanceof <Conjunction>(rhs)) return false;
    return (equal(lhs->left_son_, rhs->left_son_) &&
            equal(lhs->right_son_, rhs->right_son_)) ||
           (equal(lhs->left_son_, rhs->right_son_) &&
            equal(lhs->right_son_, rhs->left_son_));
  }

  if (utils:: instanceof <Next>(lhs)) {
    if (!utils:: instanceof <Next>(rhs)) return false;
    return equal(lhs->left_son_, rhs->left_son_);
  }

  if (utils:: instanceof <Until>(lhs)) {
    if (!utils:: instanceof <Until>(rhs)) return false;
    return equal(lhs->left_son_, rhs->left_son_) &&
           equal(lhs->right_son_, rhs->right_son_);
  }

  if (utils:: instanceof <AtomicProposition>(lhs)) {
    if (!utils:: instanceof <AtomicProposition>(rhs)) return false;
    return (*dynamic_cast<AtomicProposition*>(lhs)->ap_name_) ==
           (*dynamic_cast<AtomicProposition*>(rhs)->ap_name_);
  }

  if (utils:: instanceof <True>(lhs)) {
    if (!utils:: instanceof <True>(rhs)) return false;
    return true;
  }

  LOG(FATAL) << "unreached";
  return false;
}

bool in(LTLFormula* formula, const std::vector<LTLFormula*>& container) {
  for (auto f : container) {
    if (equal(f, formula)) {
      return true;
    }
  }
  return false;
}

bool in(LTLFormula* formula, const std::set<LTLFormula*>& container) {
  for (auto f : container) {
    if (equal(f, formula)) {
      return true;
    }
  }
  return false;
}

}  // namespace ltl

}  // namespace alice
