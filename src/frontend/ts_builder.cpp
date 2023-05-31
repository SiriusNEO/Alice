#include "ts_builder.hpp"

#include <sstream>

#include "../utils/logging.hpp"

namespace alice {

namespace frontend {

TSBuilder::TSBuilder(const char* file_path) : file_in_() {
  file_in_.open(file_path, std::ios::in);
  CHECK(file_in_.is_open());

  ts_ = new ts::TransitionSystem();

  int states_num, trans_num;

  // 1. read fist line
  file_in_ >> states_num >> trans_num;
  for (int i = 0; i < states_num; ++i) {
    ts_->states_.push_back(new ts::State("s" + std::to_string(i)));
  }
  for (int i = 0; i < trans_num; ++i) {
    ts_->transitions_.push_back(new ts::Transition("t" + std::to_string(i)));
  }

  std::string buffer;

  // 2. read second line (init states)

  // handle '\n' in the cache
  getline(file_in_, buffer);

  getline(file_in_, buffer);
  {
    int init_state_id;
    std::istringstream ss(buffer);
    while (ss >> init_state_id) {
      CHECK(init_state_id >= 0);
      CHECK(init_state_id < ts_->states_.size());
      ts_->init_states_.push_back(ts_->states_[init_state_id]);
      ts_->states_[init_state_id]->is_init_ = true;
    }
    buffer.clear();
  }

  // 3. read third line (actions)
  getline(file_in_, buffer);
  {
    std::string act_name;
    std::istringstream ss(buffer);
    while (ss >> act_name) {
      ts_->actions_.push_back(new ts::Action(act_name));
    }
    buffer.clear();
  }

  // 4. read forth line (AP)
  getline(file_in_, buffer);
  {
    std::string ap_name;
    std::istringstream ss(buffer);
    while (ss >> ap_name) {
      ts_->AP_.push_back(new ts::AtomicProposition(ap_name));
    }
    buffer.clear();
  }

  // 5. next T lines
  for (int l = 0, i, j, k; l < ts_->transitions_.size(); ++l) {
    file_in_ >> i >> k >> j;

    ts::Transition* t = ts_->transitions_[l];
    t->from_ = ts_->states_[i];
    t->to_ = ts_->states_[j];
    t->action_ = ts_->actions_[k];
    t->from_->out_edges_.push_back(t);
  }

  // 6. next S lines
  getline(file_in_, buffer);
  for (int i = 0; i < ts_->states_.size(); ++i) {
    getline(file_in_, buffer);
    std::istringstream ss(buffer);
    ts::State* s = ts_->states_[i];
    int ap_id;
    while (ss >> ap_id) {
      s->L_.push_back(ts_->AP_[ap_id]);
    }
    buffer.clear();
  }
}

TSBuilder::~TSBuilder() { delete ts_; }

ts::TransitionSystem* TSBuilder::get() const { return ts_; }

}  // namespace frontend

}  // namespace alice
