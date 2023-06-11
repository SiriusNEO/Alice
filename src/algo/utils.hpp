/*!
 * \file src/algo/utils.hpp
 * \brief Some tool functions in algorithm implementation.
 * \author SiriusNEO
 */

#ifndef ALGO_UTILS_HPP_
#define ALGO_UTILS_HPP_

#include "../automata/ba.hpp"
#include "../ts/components.hpp"

namespace alice {

namespace algo {

/*! \brief Get the set {q | p -- L(s) --> q} in the NBA. */
std::set<automata::State*> getOutStatesByL(
    automata::NondeterministicBuechiAutomata* nba, automata::State* p,
    ts::State* s);

}  // namespace algo

}  // namespace alice

#endif  // ALGO_UTILS_HPP_
