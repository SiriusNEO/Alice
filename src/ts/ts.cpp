#include "ts.hpp"

#include <iostream>

namespace alice {

namespace ts {

TransitionSystem::~TransitionSystem() {
  for (const auto& s : states_) delete s;
  for (const auto& t : transitions_) delete t;
  for (const auto& a : actions_) delete a;
  for (const auto& p : AP_) delete p;
}

void TransitionSystem::show(std::ostream& os) const {
  os << "[Transition System]" << std::endl;

  // print states
  os << "\tStates: {";
  for (const auto& s : states_) {
    os << s->name_ << ", ";
  }
  os << "}" << std::endl;

  // print transitions
  os << "\tTransitions: {";
  for (const auto& t : transitions_) {
    os << t->name_ << ", ";
  }
  os << "}" << std::endl;

  // print init states
  os << "\tInitial States: {";
  for (const auto& s : init_states_) {
    os << s->name_ << ", ";
  }
  os << "}" << std::endl;

  // print actions
  os << "\tActions: {";
  for (const auto& a : actions_) {
    os << a->name_ << ", ";
  }
  os << "}" << std::endl;

  // print AP
  os << "\tAP: {";
  for (const auto& ap : AP_) {
    os << ap->name_ << ", ";
  }
  os << "}" << std::endl;

  // print edges
  os << "\tEdges: " << std::endl;
  for (const auto& s : states_) {
    for (const auto& t : s->out_edges_) {
      os << "\t\t" << s->name_ << " -> " << t->to_->name_
         << " (transition: " << t->name_ << ", action: " << t->action_->name_
         << ")";
      os << std::endl;
    }
  }

  // print L
  os << "\tL: " << std::endl;
  for (const auto& s : states_) {
    os << "\t\t"
       << "L(" << s->name_ << ") = {";
    for (const auto& ap : s->L_) {
      os << ap->name_ << ", ";
    }
    os << "}" << std::endl;
  }
}

}  // namespace ts

}  // namespace alice
