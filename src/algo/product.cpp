#include <algorithm>

#include "../ts/components.hpp"
#include "../utils/logging.hpp"
#include "algo.hpp"
#include "utils.hpp"

namespace alice {

namespace algo {

ts::TransitionSystem* productTSAndNBA(
    ts::TransitionSystem* ts, automata::NondeterministicBuechiAutomata* nba) {
  ts::TransitionSystem* product = new ts::TransitionSystem();

  std::map<ts::Action*, ts::Action*> actions_map;

  // Step 1. Construct the S x Q.
  // Step 2. Construct AP' and L'.
  // Naming rule:
  //  (1) Using the name of q (e.g. q0) directly as the AP name.
  //  (2) If q is a finish state, add a speical prefix "@".
  for (size_t i = 0; i < ts->states_.size(); ++i) {
    for (size_t j = 0; j < nba->states_.size(); ++j) {
      auto s = ts->states_[i];
      auto q = nba->states_[j];

      // flatten nested product for better display
      auto q_name = q->name_;
      if (q_name[0] == '<' && q_name.back() == '>') {
        q_name = q_name.substr(1, q_name.length() - 2);
      }

      auto new_state = new ts::State("<" + s->name_ + ", " + q_name + ">");
      product->states_.push_back(new_state);
      product->sq_map_[std::make_pair(s, q)] = new_state;

      if (i == 0) {  // the first loop in Q
        std::string ap_name = q->name_;
        if (nba->F_.find(q) != nba->F_.end()) {
          ap_name = "@" + ap_name;
        }
        product->AP_.push_back(new ts::AtomicProposition(ap_name));
      }

      new_state->L_.push_back(product->AP_[j]);
    }
  }

  // Step 3. Construct I'.
  for (const auto& s0 : ts->init_states_) {
    for (const auto& q0 : nba->init_states_) {
      auto q_set = std::move(getOutStatesByL(nba, q0, s0));
      // LOG(INFO) << "q_set size: " << q_set.size();
      for (const auto& q : q_set) {
        auto init_state = product->sq_map_[std::make_pair(s0, q)];
        product->init_states_.insert(init_state);
        init_state->is_init_ = true;
      }
    }
  }

  // Step 4. Copy the Act.
  for (const auto& a : ts->actions_) {
    auto new_action = new ts::Action(a->name_);
    product->actions_.push_back(new_action);
    actions_map[a] = new_action;
  }

  // Step 5. Construct the new transition.
  int counter = 0;
  for (const auto& trans : ts->transitions_) {
    auto s = trans->from_, t = trans->to_;
    for (const auto& q : nba->states_) {
      auto p_set = std::move(getOutStatesByL(nba, q, t));
      for (const auto& p : p_set) {
        auto sq = product->sq_map_[std::make_pair(s, q)];
        auto tp = product->sq_map_[std::make_pair(t, p)];
        auto new_trans = new ts::Transition("t" + std::to_string(counter++));
        product->transitions_.push_back(new_trans);
        product->link(sq, tp, new_trans, actions_map[trans->action_]);
      }
    }
  }

  return product;
}

}  // namespace algo

}  // namespace alice
