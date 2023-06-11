#include "closure_analyzer.hpp"

#include <algorithm>

#include "../utils/instanceof.hpp"
#include "printer.hpp"

namespace alice {

namespace ltl {

void ClosureAnalyzer::putIntoClosureNormal(LTLFormula* formula) {
  if (!in(formula, closure_)) {
    closure_.push_back(formula);
    auto neg = new Negation(formula);
    closure_.push_back(neg);
    created_.insert(neg);
  }
}

bool ClosureAnalyzer::inEnumerated(LTLFormula* formula) {
  return in(formula, enumerated_);
}

void ClosureAnalyzer::show(std::ostream& os) const {
  os << "[Closure Analyzer]" << std::endl;

  // show has true
  os << "\tHas True: ";
  if (has_true_) {
    os << "True\n";
  } else {
    os << "False\n";
  }

  // show closure
  os << "\tClosure: {\n";
  for (const auto& formula : closure_) {
    os << "\t\t" << toString(formula) << ", \n";
  }
  os << "\t}" << std::endl;

  // show elementary
  os << "\tElementary Sets: \n";
  int counter = 1;
  for (const auto& elem_set : elementary_sets_) {
    os << "\t\tB" << counter << "={";
    for (const auto& formula : *elem_set) {
      os << toString(formula) << ", ";
    }
    os << "}" << std::endl;
    counter++;
  }
}

void ClosureAnalyzer::getElementarySet() { enumerateAndCheckElementarySet(0); }

void ClosureAnalyzer::enumerateAndCheckElementarySet(int depth) {
  if (depth < closure_.size()) {
    // without this element
    enumerateAndCheckElementarySet(depth + 1);
    // with this element
    enumerated_.push_back(closure_[depth]);
    enumerateAndCheckElementarySet(depth + 1);
    enumerated_.pop_back();
  } else {
    // start check
    bool check_flag = true;

    // 1. propositional logic: True
    if (has_true_) {
      check_flag = false;
      for (const auto& f : enumerated_) {
        if (has_true_ && utils:: instanceof <True>(f)) {
          check_flag = true;
          break;
        }
      }
    }

    for (const auto& f : closure_) {
      // 2. propositional logic: Conjunction
      if (utils:: instanceof <Conjunction>(f)) {
        bool cond1 = inEnumerated(f);
        bool cond2 = inEnumerated(f->left_son_);
        bool cond3 = inEnumerated(f->right_son_);
        // a /\ b in B, then a and b all in B.
        if (cond1 && (!cond2 || !cond3)) {
          CHECK_FAILED;
        }
        // a and b all in B, then a /\ b in B.
        if ((cond2 && cond3) && !cond1) {
          CHECK_FAILED;
        }
      }

      if (utils:: instanceof <Negation>(f)) {
        bool cond1 = inEnumerated(f->left_son_);
        bool cond2 = inEnumerated(f);
        // 3. propositional logic: Negation
        // 4. maximality
        if (cond1 == cond2) {
          CHECK_FAILED;
        }
      }

      // 5. local until consistency
      if (utils:: instanceof <Until>(f)) {
        bool cond1 = inEnumerated(f);
        bool cond2 = inEnumerated(f->right_son_);
        // b in B, then a U b in B
        if (cond2 && !cond1) {
          CHECK_FAILED;
        }
        // a U b in B and b not in B, then a in B.
        if (cond1 && !cond2) {
          if (!inEnumerated(f->left_son_)) {
            CHECK_FAILED;
          }
        }
      }
    }

    if (check_flag) {
      elementary_sets_.push_back(
          new std::set<LTLFormula*>(enumerated_.begin(), enumerated_.end()));
    }
  }
}

void ClosureAnalyzer::_visit(Negation* node) {
  // negation of negation is node->left_son_
  // don't push this negation into it because it will created in the son node.
  this->visit(node->left_son_);
}

void ClosureAnalyzer::_visit(Conjunction* node) {
  putIntoClosureNormal(node);
  this->visit(node->left_son_);
  this->visit(node->right_son_);
}

void ClosureAnalyzer::_visit(Next* node) {
  putIntoClosureNormal(node);
  this->visit(node->left_son_);
}

void ClosureAnalyzer::_visit(Until* node) {
  putIntoClosureNormal(node);
  this->visit(node->left_son_);
  this->visit(node->right_son_);
}

void ClosureAnalyzer::_visit(AtomicProposition* node) {
  putIntoClosureNormal(node);
  AP_.insert(*node->ap_name_);
}

void ClosureAnalyzer::_visit(True* node) {
  putIntoClosureNormal(node);
  has_true_ = true;
}

}  // namespace ltl

}  // namespace alice
