#include "ba.hpp"

#include "../ltl/printer.hpp"

namespace alice {

namespace automata {

bool BuechiAutomata::isNonBlocking() const {
  for (const auto& state : states_)
    for (const auto& word : alphabet_)
      if (state->delta_[word].empty()) return false;
  return true;
}

void BuechiAutomata::showMainBody(std::ostream& os) const {
  // print whether it's nonblocking.
  os << "\tIs NonBlocking: ";
  if (isNonBlocking()) {
    os << "True" << std::endl;
  } else {
    os << "False" << std::endl;
  }

  // print states
  os << "\tStates: ";
  for (const auto& s : states_) {
    os << "\t\t" << s->name_ << "={";
    for (const auto& formula : *s->B_) {
      os << ltl::toString(formula) << ", ";
    }
    os << "}" << std::endl;
  }

  // print init states
  os << "\tInitial States: {";
  for (const auto& s : init_states_) {
    os << s->name_ << ", ";
  }
  os << "}" << std::endl;

  // print alphabet
  os << "\tAlphabet: ";
  for (const auto& symbols : alphabet_) {
    os << "\t\t" << symbols->toString() << std::endl;
  }

  // print delta functions
  os << "\tDelta: ";
  for (const auto& s : states_) {
    for (const auto& p : s->delta_) {
      for (const auto& t : p.second) {
        os << "\t\t" << s->name_ << " -> " << t->name_
           << " (symbols: " << p.first->toString() << ")";
        os << std::endl;
      }
    }
  }
}

void BuechiAutomata::show(std::ostream& os) const {
  os << "[Buechi Automata]" << std::endl;
  showMainBody(os);
}

void NondeterministicBuechiAutomata::show(std::ostream& os) const {
  os << "[Nondeterministic Buechi Automata]" << std::endl;
  // print F.
  os << "\tF: {";
  for (const auto& s : F_) {
    os << s->name_ << ", ";
  }
  os << "}" << std::endl;
  showMainBody(os);
}

void GeneralizedBuechiAutomata::show(std::ostream& os) const {
  os << "[Generalized Buechi Automata]" << std::endl;
  // print F.
  os << "\tF: ";
  for (const auto& s_set : F_) {
    os << "\t\t{";
    for (const auto& s : s_set) os << s->name_ << ", ";
    os << "}" << std::endl;
  }
  showMainBody(os);
}

}  // namespace automata

}  // namespace alice
