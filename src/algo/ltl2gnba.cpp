#include <string>

#include "../ltl/closure_analyzer.hpp"
#include "../utils/instanceof.hpp"
#include "../utils/logging.hpp"
#include "algo.hpp"

namespace alice {

namespace algo {

void enumerateAlphabet(std::set<std::string>::iterator it,
                       std::set<std::string>::iterator end,
                       std::vector<ba::Word*>& alphabet) {
  static std::vector<std::string> enumerated;
  if (it == end) {
    alphabet.push_back(new ba::Word(
        new std::set<std::string>(enumerated.begin(), enumerated.end())));
    return;
  }

  enumerated.push_back(*it);
  ++it;
  enumerateAlphabet(it, end, alphabet);
  enumerated.pop_back();
  enumerateAlphabet(it, end, alphabet);
}

ba::GeneralizedBuechiAutomata* fromLTL(ltl::LTLFormula* formula) {
  auto gnba = new ba::GeneralizedBuechiAutomata();

  auto closure_analyzer = ltl::ClosureAnalyzer();
  closure_analyzer.visit(formula);
  closure_analyzer.getElementarySet();

  // Step 1. Construct the alphabet (2^AP).
  enumerateAlphabet(closure_analyzer.AP_.begin(), closure_analyzer.AP_.end(),
                    gnba->alphabet_);

  // Step 2. Construct Q, Q0 and delta.
  std::map<std::set<alice::ltl::LTLFormula*>*, ba::State*> elem_set_to_state;
  for (auto B : closure_analyzer.elementary_sets_) {
    // Q and Q0
    auto state = new ba::State(B);
    if (B->find(formula) != B->end()) {
      gnba->init_states_.push_back(state);
    }
    gnba->states_.push_back(state);
    elem_set_to_state[B] = state;

    for (auto word : gnba->alphabet_) {
      state->delta_[word] = std::vector<ba::State*>();
    }
  }

  // delta function
  for (auto B : closure_analyzer.elementary_sets_) {
    auto state = elem_set_to_state[B];

    std::set<std::string> B_and_AP;
    for (auto it = B->begin(); it != B->end(); ++it) {
      auto ap = dynamic_cast<ltl::AtomicProposition*>(*it);
      if (ap != nullptr && closure_analyzer.AP_.find(*ap->ap_name_) !=
                               closure_analyzer.AP_.end()) {
        B_and_AP.insert(*ap->ap_name_);
      }
    }

    ba::Word* A = nullptr;
    for (auto word : gnba->alphabet_) {
      if (*word->A_ == B_and_AP) {
        A = word;
      }
    }
    CHECK(A != NULL);

    for (auto B_prime : closure_analyzer.elementary_sets_) {
      bool check_flag = true;
      for (auto f : closure_analyzer.closure_) {
        // (1) Next \psi in B <=> \psi in B'
        if (utils:: instanceof <ltl::Next>(f)) {
          bool cond1 = ltl::in(f, *B);
          bool cond2 = ltl::in(f->left_son_, *B_prime);
          if (cond1 ^ cond2) {
            CHECK_FAILED;
          }
        }
        // (2) Until
        if (utils:: instanceof <ltl::Until>(f)) {
          bool cond1 = ltl::in(f, *B);
          bool cond2 = ltl::in(f->right_son_, *B);
          bool cond3 = ltl::in(f->left_son_, *B);
          bool cond4 = ltl::in(f, *B_prime);
          if (cond1 ^ (cond2 || (cond3 && cond4))) {
            CHECK_FAILED;
          }
        }
      }
      if (check_flag) {
        state->delta_[A].push_back(elem_set_to_state[B_prime]);
      }
    }
  }

  // Step 3. Construct F.
  for (auto f : closure_analyzer.closure_) {
    if (utils:: instanceof <ltl::Until>(f)) {
      std::set<ba::State*> F;
      for (auto elem_set : closure_analyzer.elementary_sets_) {
        if (!in(f, *elem_set) || in(f->right_son_, *elem_set)) {
          F.insert(elem_set_to_state[elem_set]);
        }
      }
      if (!F.empty()) {
        gnba->F_.push_back(std::move(F));
      }
    }
  }

  return gnba;
}

}  // namespace algo

}  // namespace alice
