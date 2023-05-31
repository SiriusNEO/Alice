/*!
 * \file src/ts/components.hpp
 * \brief Components in TS.
 * \author SiriusNEO
 */

#ifndef TS_COMPONENTS_HPP_
#define TS_COMPONENTS_HPP_

#include <string>
#include <vector>

namespace alice {

namespace ts {

class Transition;
class AtomicProposition;

class State {
 public:
  bool is_init_;
  std::string name_;
  std::vector<Transition *> out_edges_;
  std::vector<AtomicProposition *> L_;

  explicit State(const std::string &name)
      : is_init_(false), name_(name), out_edges_(), L_() {}
};

class Action {
 public:
  std::string name_;

  explicit Action(const std::string &name) : name_(name) {}
};

class Transition {
 public:
  std::string name_;
  Action *action_;
  State *from_, *to_;

  explicit Transition(const std::string &name)
      : name_(name), action_(nullptr), from_(nullptr), to_(nullptr) {}
};

class AtomicProposition {
 public:
  std::string name_;

  explicit AtomicProposition(const std::string &name) : name_(name) {}
};

}  // namespace ts

}  // namespace alice

#endif  // TS_COMPONENTS_HPP_
