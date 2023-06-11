/*!
 * \file src/ts/ts.hpp
 * \brief Transition system.
 * \author SiriusNEO
 */

#include <map>
#include <vector>

#include "../automata/components.hpp"
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

  std::set<State*> init_states_;

  // <s, q> -> true state in product ts.
  std::map<std::pair<State*, automata::State*>, State*> sq_map_;

  TransitionSystem()
      : states_(), transitions_(), AP_(), init_states_(), sq_map_() {}

  ~TransitionSystem();

  void show(std::ostream& os, const std::string& name) const;

  static void link(State* from, State* to, Transition* trans, Action* act);
};

}  // namespace ts

}  // namespace alice

#endif  // TS_TS_HPP_
