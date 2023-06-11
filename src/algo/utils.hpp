/*!
 * \file src/algo/utils.hpp
 * \brief Some tool functions in algorithm implementation.
 * \author SiriusNEO
 */

#ifndef ALGO_UTILS_HPP_
#define ALGO_UTILS_HPP_

#include "../automata/ba.hpp"
#include "../ts/components.hpp"
#include "../utils/logging.hpp"

namespace alice {

namespace algo {

/*! \brief Get the set {q | p -- L(s) --> q} in the NBA. */
std::set<automata::State*> getOutStatesByL(
    automata::NondeterministicBuechiAutomata* nba, automata::State* p,
    ts::State* s) {
  auto L = std::set<std::string>();
  for (auto ap : s->L_) {
    L.insert(ap->name_);
  }

  for (auto word : nba->alphabet_) {
    if (word->A_ == L) {
      return std::set<automata::State*>(p->delta_[word].begin(),
                                        p->delta_[word].end());
    }
  }

  LOG(FATAL) << "unreachable";
  return std::set<automata::State*>();
}

}  // namespace algo

}  // namespace alice

#endif  // ALGO_UTILS_HPP_
