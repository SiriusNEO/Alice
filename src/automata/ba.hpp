/*!
 * \file src/automata/ba.hpp
 * \brief NBA and GNBA.
 * \author SiriusNEO
 */

#ifndef AUTOMATA_BA_HPP_
#define AUTOMATA_BA_HPP_

#include "components.hpp"

namespace alice {

namespace automata {

class BuechiAutomata {
 public:
  std::vector<State*> states_;
  std::vector<Symbols*> alphabet_;
  std::vector<State*> init_states_;

  BuechiAutomata() : states_(), alphabet_(), init_states_() {}

  virtual ~BuechiAutomata() {
    for (auto state : states_) delete state;
    for (auto symbols : alphabet_) delete symbols;
  }

  virtual void show(std::ostream& os) const;

  bool isNonBlocking() const;

 protected:
  void showMainBody(std::ostream& os) const;
};

class NondeterministicBuechiAutomata : public BuechiAutomata {
 public:
  std::set<State*> F_;

  NondeterministicBuechiAutomata() : F_(), BuechiAutomata() {}

  void show(std::ostream& os) const;
};

class GeneralizedBuechiAutomata : public BuechiAutomata {
 public:
  std::vector<std::set<State*>> F_;

  GeneralizedBuechiAutomata() : F_(), BuechiAutomata() {}

  void show(std::ostream& os) const;
};

}  // namespace automata

};  // namespace alice

#endif  // AUTOMATA_BA_HPP_
