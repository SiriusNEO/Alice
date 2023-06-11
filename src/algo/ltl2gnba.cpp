#include <string>

#include "../utils/instanceof.hpp"
#include "../utils/logging.hpp"
#include "algo.hpp"

namespace alice {

namespace algo {

void enumerateAlphabet(std::set<std::string>::iterator it,
                       std::set<std::string>::iterator end,
                       std::vector<automata::Symbols*>& alphabet) {
  static std::vector<std::string> enumerated;
  if (it == end) {
    alphabet.push_back(new automata::Symbols(
        std::set<std::string>(enumerated.begin(), enumerated.end())));
    return;
  }

  enumerated.push_back(*it);
  ++it;
  enumerateAlphabet(it, end, alphabet);
  enumerated.pop_back();
  enumerateAlphabet(it, end, alphabet);
}

automata::GeneralizedBuechiAutomata* fromLTL(
    ltl::LTLFormula* formula, const ltl::ClosureAnalyzer& analyzer,
    const std::set<std::string>& AP) {
  auto gnba = new automata::GeneralizedBuechiAutomata();

  // Step 1. Construct the alphabet (2^AP).
  enumerateAlphabet(AP.begin(), AP.end(), gnba->alphabet_);

  // Step 2. Construct Q, Q0 and delta.
  std::map<std::set<alice::ltl::LTLFormula*>*, automata::State*>
      elem_set_to_state;
  for (size_t i = 0; i < analyzer.elementary_sets_.size(); ++i) {
    auto B = analyzer.elementary_sets_[i];

    // Q and Q0
    auto state = new automata::State("q" + std::to_string(i), B);

    // LOG(INFO) << "created state: " << state->name_;

    if (ltl::in(formula, *B)) {
      gnba->init_states_.push_back(state);
      state->is_init_ = true;
    }
    gnba->states_.push_back(state);
    elem_set_to_state[B] = state;

    // init delta
    for (const auto& symbols : gnba->alphabet_) {
      state->delta_[symbols] = std::vector<automata::State*>();
    }
  }

  // Step 3. Compute delta function
  for (const auto& B : analyzer.elementary_sets_) {
    auto state = elem_set_to_state[B];

    std::set<std::string> B_and_AP;
    for (auto it = B->begin(); it != B->end(); ++it) {
      auto ap = dynamic_cast<ltl::AtomicProposition*>(*it);
      if (ap != nullptr && AP.find(*ap->ap_name_) != AP.end()) {
        B_and_AP.insert(*ap->ap_name_);
      }
    }

    automata::Symbols* A = nullptr;
    for (const auto& symbols : gnba->alphabet_) {
      if (symbols->A_ == B_and_AP) {
        A = symbols;
      }
    }
    CHECK(A != NULL);

    for (const auto& B_prime : analyzer.elementary_sets_) {
      bool check_flag = true;
      for (const auto& f : analyzer.closure_) {
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

  // Step 4. Construct F.
  for (const auto& f : analyzer.closure_) {
    if (utils:: instanceof <ltl::Until>(f)) {
      std::set<automata::State*> F;
      for (const auto& elem_set : analyzer.elementary_sets_) {
        if (!in(f, *elem_set) || in(f->right_son_, *elem_set)) {
          F.insert(elem_set_to_state[elem_set]);
        }
      }
      if (!F.empty()) {
        gnba->F_.push_back(std::move(F));
      }
    }
  }

  // Step 5. Turn to Non-Blocking.
  if (!gnba->isNonBlocking()) {
    gnba->trap_ = new automata::State("trap", new std::set<ltl::LTLFormula*>());
    gnba->states_.push_back(gnba->trap_);
    for (const auto& state : gnba->states_) {
      for (const auto& symbols : gnba->alphabet_) {
        if (state->delta_[symbols].empty()) {
          state->delta_[symbols].push_back(gnba->trap_);
        }
      }
    }
  }

  return gnba;
}

}  // namespace algo

}  // namespace alice
