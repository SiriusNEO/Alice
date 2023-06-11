#include "utils.hpp"

#include "../utils/logging.hpp"

namespace alice {

namespace algo {

std::set<automata::State*> getOutStatesByL(
    automata::NondeterministicBuechiAutomata* nba, automata::State* p,
    ts::State* s) {
  auto L = std::set<std::string>();
  std::string L_str = "{";
  for (auto ap : s->L_) {
    L.insert(ap->name_);
    L_str += ap->name_ + ", ";
  }
  L_str += "}";

  for (auto symbols : nba->alphabet_) {
    if (symbols->A_ == L) {
      return std::set<automata::State*>(p->delta_[symbols].begin(),
                                        p->delta_[symbols].end());
    }
  }

  LOG(FATAL) << "This L Mismatch in alphabet: " << L_str;
  return std::set<automata::State*>();
}

}  // namespace algo

}  // namespace alice
