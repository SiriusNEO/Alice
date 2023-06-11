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

void TransitionSystem::link(State* from, State* to, Transition* trans,
                            Action* act) {
  trans->from_ = from;
  trans->to_ = to;
  trans->action_ = act;
  from->out_edges_.push_back(trans);
}

void TransitionSystem::show(std::ostream& os, const std::string& name) const {
  os << "[Transition System: " << name << "]" << std::endl;

  // print states
  os << "\tStates: " << std::endl;
  for (const auto& s : states_) {
    os << "\t\t" << s->name_ << std::endl;
  }

  // print init states
  os << "\tInitial States: " << std::endl;
  for (const auto& s : init_states_) {
    os << "\t\t" << s->name_ << std::endl;
  }

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
  os << "\tTransitions: " << std::endl;
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
