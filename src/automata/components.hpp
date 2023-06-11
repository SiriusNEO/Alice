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

namespace automata {

class Symbols {
 public:
  // for 2^AP
  std::set<std::string> A_;

  explicit Symbols(const std::set<std::string> &A) : A_(A) {}

  std::string toString() const {
    std::string ret = "{";
    for (const auto &symbol : A_) {
      ret += symbol + ", ";
    }
    return ret + "}";
  }
};

class State {
 public:
  bool is_init_;

  std::string name_;

  // for the elementary set
  // don't delete it in destructor because it's managed in the closure analyzer.
  std::set<ltl::LTLFormula *> *B_;

  // for the converted NBA
  int index_;

  std::map<Symbols *, std::vector<State *>> delta_;

  explicit State(const std::string &name, std::set<ltl::LTLFormula *> *B,
                 int index = 0)
      : is_init_(false), name_(name), index_(index), delta_() {}
};

}  // namespace automata

}  // namespace alice

#endif  // AUTOMATA_COMPONENTS_HPP_
