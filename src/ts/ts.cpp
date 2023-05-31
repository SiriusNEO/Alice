#include "ts.hpp"

#include <iostream>

namespace alice {

namespace ts {

TransitionSystem::~TransitionSystem() {
  for (auto s : states_) delete s;
  for (auto t : transitions_) delete t;
  for (auto a : actions_) delete a;
  for (auto p : AP_) delete p;
}

void TransitionSystem::print() const {
  std::cout << "[Transition System]" << std::endl;

  // print states
  std::cout << "\tStates: {";
  for (auto s : states_) {
    std::cout << s->name_ << ", ";
  }
  std::cout << "}" << std::endl;

  // print transitions
  std::cout << "\tTransitions: {";
  for (auto t : transitions_) {
    std::cout << t->name_ << ", ";
  }
  std::cout << "}" << std::endl;

  // print init states
  std::cout << "\tInitial States: {";
  for (auto s : init_states_) {
    std::cout << s->name_ << ", ";
  }
  std::cout << "}" << std::endl;

  // print actions
  std::cout << "\tActions: {";
  for (auto a : actions_) {
    std::cout << a->name_ << ", ";
  }
  std::cout << "}" << std::endl;

  // print AP
  std::cout << "\tAP: {";
  for (auto ap : AP_) {
    std::cout << ap->name_ << ", ";
  }
  std::cout << "}" << std::endl;

  // print edges
  std::cout << "\tEdges: " << std::endl;
  for (auto s : states_) {
    std::cout << "\t\t" << s->name_ << " -> ";
    for (auto t : s->out_edges_) {
      std::cout << t->to_->name_ << ", ";
    }
    std::cout << std::endl;
  }

  // print L
  std::cout << "\tL: " << std::endl;
  for (auto s : states_) {
    std::cout << "\t\t"
              << "L(" << s->name_ << ") = {";
    for (auto ap : s->L_) {
      std::cout << ap->name_ << ", ";
    }
    std::cout << "}" << std::endl;
  }
}

}  // namespace ts

}  // namespace alice
