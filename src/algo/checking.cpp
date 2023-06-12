#include <stack>

#include "../utils/logging.hpp"
#include "algo.hpp"
#include "utils.hpp"

namespace alice {

namespace algo {

std::set<ts::State*> I, R, T;
std::stack<ts::State*> U, V;
bool cycle_found;

void init() {
  cycle_found = false;
  I.clear();
  R.clear();
  T.clear();
  while (!U.empty()) {
    U.pop();
  }
  while (!V.empty()) {
    V.pop();
  }
}

bool cycleCheck(ts::State* s) {
  bool found = false;
  V.push(s);
  T.insert(s);
  do {
    auto s_prime = V.top();
    for (const auto& t : s_prime->out_edges_) {
      if (t->to_ == s) {
        found = true;
        V.push(s);
        break;
      }
    }
    if (!found) {
      // check_flag for: Post(s_prime) \ T != \emptyset
      bool check_flag = false;
      for (const auto& t : s_prime->out_edges_) {
        if (T.find(t->to_) == T.end()) {
          V.push(t->to_);
          T.insert(t->to_);
          check_flag = true;
          break;
        }
      }
      if (!check_flag) {
        V.pop();
      }
    }
  } while (!V.empty() && !found);
  return found;
}

void reachableCycle(ts::State* s) {
  U.push(s);
  R.insert(s);
  if (I.find(s) != I.end()) {
    I.erase(s);
  }

  do {
    auto s_prime = U.top();
    // check_flag for: Post(s_prime) \ R != \emptyset
    bool check_flag = false;
    for (const auto& t : s_prime->out_edges_) {
      if (R.find(t->to_) == R.end()) {
        U.push(t->to_);
        R.insert(t->to_);
        if (I.find(t->to_) != I.end()) {
          I.erase(t->to_);
        }
        check_flag = true;
        break;
      }
    }

    if (!check_flag) {
      U.pop();
      // check whether s_prime \vdash \neg F
      if (s_prime->L_[0]->name_[0] == '@') {
        cycle_found = cycleCheck(s_prime);
      }
    }
  } while (!U.empty() && !cycle_found);
}

void reportCycleInLog() {
  std::string info_str = "Cycle Found in Check.\n\tPath: ";

  std::vector<std::string> path_buffer;
  while (!U.empty()) {
    auto s = U.top();
    U.pop();
    path_buffer.push_back(s->name_);
  }
  for (auto it = path_buffer.rbegin(); it != path_buffer.rend();) {
    info_str += *it;
    ++it;
    if (it != path_buffer.rend()) {
      info_str += " -> ";
    }
  }

  info_str += "\n\tCycle: ";
  while (!V.empty()) {
    auto s = V.top();
    V.pop();
    info_str += s->name_;
    if (!V.empty()) info_str += " - ";
  }

  LOG(INFO) << info_str;
}

bool persistenceChecking(ts::TransitionSystem* ts) {
  init();
  I = std::set<ts::State*>(ts->init_states_.begin(), ts->init_states_.end());

  while (!I.empty() && !cycle_found) {
    auto s = *I.begin();
    reachableCycle(s);
  }

  if (cycle_found) reportCycleInLog();

  return !cycle_found;
}

bool persistenceChecking(ts::TransitionSystem* ts, ts::State* start,
                         automata::NondeterministicBuechiAutomata* nba) {
  init();
  for (const auto& q0 : nba->init_states_) {
    auto q_set = std::move(getOutStatesByL(nba, q0, start));
    for (const auto& q : q_set) {
      I.insert(ts->sq_map_[std::make_pair(start, q)]);
    }
  }

  while (!I.empty() && !cycle_found) {
    auto s = *I.begin();
    reachableCycle(s);
  }

  if (cycle_found) reportCycleInLog();

  return !cycle_found;
}

}  // namespace algo

}  // namespace alice
