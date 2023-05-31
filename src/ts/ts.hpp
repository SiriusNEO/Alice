/*!
 * \file src/ts/ts.hpp
 * \brief Transition system.
 * \author SiriusNEO
 */

#include <vector>

#include "components.hpp"

#ifndef TS_TS_HPP_
#define TS_TS_HPP_

namespace alice {

namespace ts {

class TransitionSystem {
 public:
  std::vector<State*> states_;
  std::vector<Transition*> transitions_;
  std::vector<Action*> actions_;
  std::vector<AtomicProposition*> AP_;

  std::vector<State*> init_states_;

  TransitionSystem() : states_(), transitions_(), init_states_(), AP_() {}

  ~TransitionSystem();

  void print() const;
};

}  // namespace ts

}  // namespace alice

#endif  // TS_TS_HPP_
