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

/*!
 * \brief Convert LTL Formula to GNBA.
 * Rerference: Principles of Model Checking, P278, Theorem 5.37
 */
automata::GeneralizedBuechiAutomata* fromLTL(ltl::LTLFormula* formula);

/*!
 * \brief Convert GNBA to NBA.
 * Rerference: Principles of Model Checking, P195, Theorem 4.56
 */
automata::NondeterministicBuechiAutomata* fromGNBA(
    automata::GeneralizedBuechiAutomata* gnba);

/*!
 * \brief TS x NBA -> producted TS.
 * Rerference: Principles of Model Checking, P200, Definition 4.62
 */
ts::TransitionSystem* productFromTSAndNBA(
    ts::TransitionSystem* ts, automata::NondeterministicBuechiAutomata* nba);

/*!
 * \brief Persistence checking using nested DFS search algorithm.
 * Rerference: Principles of Model Checking, P211, Algorithm 8
 */
bool persistenceChecking(ts::TransitionSystem* ts);

}  // namespace algo

}  // namespace alice

#endif  // ALGO_ALGO_HPP_
