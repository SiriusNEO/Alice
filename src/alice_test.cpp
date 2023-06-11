#include "algo/algo.hpp"
#include "frontend/benchmark.hpp"
#include "frontend/ts_builder.hpp"
#include "ltl/ast.hpp"
#include "ltl/closure_analyzer.hpp"
#include "ltl/parser.hpp"
#include "ltl/printer.hpp"
#include "ltl/tree_deleter.hpp"
#include "utils/logging.hpp"

using namespace alice;

int main() {
  // **********************************************************
  LOG(INFO) << "Test LTL parser";
  std::string input = "G(a \\/ b)";
  ltl::TreeDeleter deleter;
  auto root = ltl::parseLTLFromString(input);
  std::cout << "parse result: " << ltl::toString(root) << std::endl;
  deleter.visit(root);
  // **********************************************************

  // **********************************************************
  LOG(INFO) << "Test TS builder";
  frontend::TSBuilder builder("../data/TS.txt");
  builder.get()->show(std::cout, "Example");
  // **********************************************************

  // **********************************************************
  LOG(INFO) << "Test TS benchmark";
  frontend::Benchmark benchmark("../data/benchmark.txt");
  benchmark.show(std::cout);
  // **********************************************************

  // **********************************************************
  // P276. Example 5.36.
  LOG(INFO) << "Test closure analyzer";
  std::string input1 = "a U ((!a) /\\ b)";
  auto root1 = ltl::parseLTLFromString(input1);
  std::cout << "parse result: " << ltl::toString(root1) << std::endl;

  std::set<std::string> AP = {"a", "b"};

  ltl::ClosureAnalyzer analyzer(&AP);
  analyzer.visit(root1);
  analyzer.getElementarySet();
  analyzer.show(std::cout);
  deleter.visit(root1);
  // **********************************************************

  // **********************************************************
  LOG(INFO) << "Test LTL to GNBA";
  // P282, Figure 5.21.
  std::string input2 = "X a";
  auto root2 = ltl::parseLTLFromString(input2);
  std::cout << "parse result: " << ltl::toString(root2) << std::endl;

  std::set<std::string> AP1 = {"a"};

  ltl::ClosureAnalyzer analyzer1(&AP1);
  analyzer1.visit(root2);
  analyzer1.getElementarySet();
  analyzer1.show(std::cout);

  auto gnba = algo::fromLTL(root2, analyzer1, AP1);
  gnba->show(std::cout);
  deleter.visit(root2);
  delete gnba;

  // P283, Example 5.39. (a U b)
  std::string input3 = "a U b";
  auto root3 = ltl::parseLTLFromString(input3);
  std::cout << "parse result: " << ltl::toString(root3) << std::endl;

  std::set<std::string> AP2 = {"a", "b"};

  ltl::ClosureAnalyzer analyzer2(&AP2);
  analyzer2.visit(root3);
  analyzer2.getElementarySet();
  analyzer2.show(std::cout);

  auto gnba1 = algo::fromLTL(root3, analyzer2, AP2);
  gnba1->show(std::cout);
  deleter.visit(root3);
  delete gnba1;
  // **********************************************************

  // **********************************************************
  LOG(INFO) << "Test GNBA to NBA";
  // Construct the GNBA in P193, Example 4.53.
  automata::GeneralizedBuechiAutomata *gnba2 =
      new automata::GeneralizedBuechiAutomata();
  {
    auto q0 = new automata::State("q0", nullptr);
    auto q1 = new automata::State("q1", nullptr);
    auto q2 = new automata::State("q2", nullptr);
    gnba2->states_.push_back(q0);
    gnba2->states_.push_back(q1);
    gnba2->states_.push_back(q2);

    q0->is_init_ = true;
    gnba2->init_states_.push_back(q0);

    gnba2->alphabet_.push_back(new automata::Symbols({}));
    gnba2->alphabet_.push_back(new automata::Symbols({"crit1"}));
    gnba2->alphabet_.push_back(new automata::Symbols({"crit2"}));
    gnba2->alphabet_.push_back(new automata::Symbols({"crit1", "crit2"}));

    q0->delta_[gnba2->alphabet_[0]] = std::vector<automata::State *>({q0});
    q0->delta_[gnba2->alphabet_[1]] = std::vector<automata::State *>({q0, q1});
    q0->delta_[gnba2->alphabet_[2]] = std::vector<automata::State *>({q0, q2});
    q0->delta_[gnba2->alphabet_[3]] = std::vector<automata::State *>({q0});

    q1->delta_[gnba2->alphabet_[0]] = std::vector<automata::State *>({q0});
    q1->delta_[gnba2->alphabet_[1]] = std::vector<automata::State *>({q0});
    q1->delta_[gnba2->alphabet_[2]] = std::vector<automata::State *>({q0});
    q1->delta_[gnba2->alphabet_[3]] = std::vector<automata::State *>({q0});

    q2->delta_[gnba2->alphabet_[0]] = std::vector<automata::State *>({q0});
    q2->delta_[gnba2->alphabet_[1]] = std::vector<automata::State *>({q0});
    q2->delta_[gnba2->alphabet_[2]] = std::vector<automata::State *>({q0});
    q2->delta_[gnba2->alphabet_[3]] = std::vector<automata::State *>({q0});

    gnba2->F_.push_back({q1});
    gnba2->F_.push_back({q2});
  }

  gnba2->show(std::cout);

  auto nba = algo::fromGNBA(gnba2);
  nba->show(std::cout);
  delete nba;

  delete gnba2;
  // **********************************************************

  // **********************************************************
  LOG(INFO) << "Test TS NBA Product";
  // Construct the Example in P202, Figure 4.22.

  // Construct the traffic light TS.
  auto traffic_light = new ts::TransitionSystem();
  {
    auto s0 = new ts::State("s0");
    auto s1 = new ts::State("s1");
    traffic_light->states_.push_back(s0);
    traffic_light->states_.push_back(s1);

    s0->is_init_ = true;
    traffic_light->init_states_.insert(s0);

    auto red = new ts::AtomicProposition("red");
    auto green = new ts::AtomicProposition("green");
    traffic_light->AP_.push_back(red);
    traffic_light->AP_.push_back(green);

    s0->L_.push_back(red);
    s1->L_.push_back(green);

    auto a0 = new ts::Action("a0");
    auto a1 = new ts::Action("a1");
    traffic_light->actions_.push_back(a0);
    traffic_light->actions_.push_back(a1);

    auto t0 = new ts::Transition("t0");
    auto t1 = new ts::Transition("t1");
    traffic_light->transitions_.push_back(t0);
    traffic_light->transitions_.push_back(t1);
    traffic_light->link(s0, s1, t0, a0);
    traffic_light->link(s1, s0, t1, a1);
  }
  traffic_light->show(std::cout, "Traffic Light");

  // Construct the NBA.
  auto nba1 = new automata::NondeterministicBuechiAutomata();
  {
    auto q0 = new automata::State("q0", nullptr);
    auto q1 = new automata::State("q1", nullptr);
    auto q2 = new automata::State("q2", nullptr);
    nba1->states_.push_back(q0);
    nba1->states_.push_back(q1);
    nba1->states_.push_back(q2);

    q0->is_init_ = true;
    nba1->init_states_.push_back(q0);

    nba1->alphabet_.push_back(new automata::Symbols({}));
    nba1->alphabet_.push_back(new automata::Symbols({"red"}));
    nba1->alphabet_.push_back(new automata::Symbols({"green"}));
    nba1->alphabet_.push_back(new automata::Symbols({"red", "green"}));

    q0->delta_[nba1->alphabet_[0]] = std::vector<automata::State *>({q0, q1});
    q0->delta_[nba1->alphabet_[1]] = std::vector<automata::State *>({q0, q1});
    q0->delta_[nba1->alphabet_[2]] = std::vector<automata::State *>({q0});
    q0->delta_[nba1->alphabet_[3]] = std::vector<automata::State *>({q0});

    q1->delta_[nba1->alphabet_[0]] = std::vector<automata::State *>({q1});
    q1->delta_[nba1->alphabet_[1]] = std::vector<automata::State *>({q1});
    q1->delta_[nba1->alphabet_[2]] = std::vector<automata::State *>({q2});
    q1->delta_[nba1->alphabet_[3]] = std::vector<automata::State *>({q2});

    q2->delta_[nba1->alphabet_[0]] = std::vector<automata::State *>({q2});
    q2->delta_[nba1->alphabet_[1]] = std::vector<automata::State *>({q2});
    q2->delta_[nba1->alphabet_[2]] = std::vector<automata::State *>({q2});
    q2->delta_[nba1->alphabet_[3]] = std::vector<automata::State *>({q2});

    nba1->F_.insert(q1);
  }

  nba1->show(std::cout);

  auto product = algo::productTSAndNBA(traffic_light, nba1);
  product->show(std::cout, "Producted");

  delete product;
  delete traffic_light;
  delete nba1;
  // **********************************************************

  return 0;
}
