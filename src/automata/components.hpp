/*!
 * \file src/automata/components.hpp
 * \brief Components of BA.
 * \author SiriusNEO
 */

#ifndef AUTOMATA_COMPONENTS_HPP_
#define AUTOMATA_COMPONENTS_HPP_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "../ltl/ast.hpp"

namespace alice {

namespace ba {

class Word {
 public:
  // for 2^AP
  std::set<std::string> *A_;

  explicit Word(std::set<std::string> *A) : A_(A) {}
};

class State {
 public:
  bool is_init_;

  // for the elementary set
  std::set<ltl::LTLFormula *> *B_;

  // for the converted NBA
  int number_;

  std::map<Word *, std::vector<State *>> delta_;

  explicit State(std::set<ltl::LTLFormula *> *B, int number = 0)
      : is_init_(false), number_(number), delta_() {}
};

}  // namespace ba

}  // namespace alice

#endif  // AUTOMATA_COMPONENTS_HPP_
