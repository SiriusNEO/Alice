/*!
 * \file src/automata/ba.hpp
 * \brief NBA and GNBA.
 * \author SiriusNEO
 */

#ifndef AUTOMATA_BA_HPP_
#define AUTOMATA_BA_HPP_

#include "components.hpp"

namespace alice {

namespace ba {

class BuechiAutomata {
 public:
  std::vector<State*> states_;
  std::vector<Word*> alphabet_;
  std::vector<State*> init_states_;
};

class NondeterministicBuechiAutomata : public BuechiAutomata {
 public:
  std::set<State*> F_;
};

class GeneralizedBuechiAutomata : public BuechiAutomata {
 public:
  std::vector<std::set<State*>> F_;
};

}  // namespace ba

};  // namespace alice

#endif  // AUTOMATA_BA_HPP_
