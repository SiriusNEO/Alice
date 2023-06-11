#include "algo.hpp"

namespace alice {

namespace algo {

automata::NondeterministicBuechiAutomata* fromGNBA(
    automata::GeneralizedBuechiAutomata* gnba) {
  auto nba = new automata::NondeterministicBuechiAutomata();

  size_t k = gnba->F_.size();

  std::map<automata::State*, automata::State*> nba_states_to_gnba_states;
  std::map<automata::State*, std::vector<automata::State*>>
      gnba_states_to_nba_states;

  // Step 1. Copy Alphabet.
  for (const auto& symbols : gnba->alphabet_) {
    auto new_symbols = new automata::Symbols(symbols->A_);
    nba->alphabet_.push_back(new_symbols);
  }

  // Step 2. Construct Q and Q0.
  for (const auto& state : gnba->init_states_) {
    for (size_t i = 1; i <= k; ++i) {
      auto new_state = new automata::State(
          "<" + state->name_ + ", " + std::to_string(i) + ">", state->B_, i);
      if (i == 1 && state->is_init_) {
        nba->init_states_.push_back(new_state);
        new_state->is_init_ = true;
        gnba_states_to_nba_states[state] = std::vector<automata::State*>();
      }
      gnba_states_to_nba_states[state].push_back(new_state);
      nba_states_to_gnba_states[new_state] = state;
      nba->states_.push_back(new_state);
    }
  }

  // Step 3. Construct F_prime.
  for (const auto& qf : gnba->F_[0]) {
    nba->F_.insert(gnba_states_to_nba_states[qf][0]);
  }

  // Step 4. Compute delta_prime.
  for (const auto& state : nba->states_) {
    auto q = nba_states_to_gnba_states[state];
    auto i = state->index_;
    for (const auto& symbols : nba->alphabet_) {
      state->delta_[symbols] = std::vector<automata::State*>();
      if (gnba->F_[i].find(q) == gnba->F_[i - 1].end()) {
        for (const auto& q_prime : q->delta_[symbols]) {
          state->delta_[symbols].push_back(
              gnba_states_to_nba_states[q_prime][i - 1]);
        }
      } else {
        for (const auto& q_prime : q->delta_[symbols]) {
          state->delta_[symbols].push_back(
              gnba_states_to_nba_states[q_prime][i % k]);
        }
      }
    }
  }

  return nba;
}

}  // namespace algo

}  // namespace alice
