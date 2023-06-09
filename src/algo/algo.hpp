/*!
 * \file src/algo/algo.hpp
 * \brief Core algorithms declaration.
 * \author SiriusNEO
 */

#ifndef ALGO_ALGO_HPP_
#define ALGO_ALGO_HPP_

#include "../automata/ba.hpp"
#include "../ltl/ast.hpp"
#include "../ts/ts.hpp"

namespace alice {

namespace algo {

/*! \brief Convert LTL Formula to GNBA. */
ba::GeneralizedBuechiAutomata* fromLTL(ltl::LTLFormula* formula);

/*! \brief Convert GNBA to NBA. */
ba::NondeterministicBuechiAutomata* fromGNBA(
    ba::GeneralizedBuechiAutomata* gnba);

/*! \brief TS x NBA -> producted TS. */
ts::TransitionSystem* productFromTSAndNBA(
    ts::TransitionSystem* ts, ba::NondeterministicBuechiAutomata* nba);

void nestedDFS();

}  // namespace algo

}  // namespace alice

#endif  // ALGO_ALGO_HPP_
