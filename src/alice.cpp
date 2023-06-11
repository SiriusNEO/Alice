#include "algo/algo.hpp"
#include "frontend/benchmark.hpp"
#include "frontend/ts_builder.hpp"
#include "ltl/ast.hpp"
#include "ltl/closure_analyzer.hpp"
#include "ltl/parser.hpp"
#include "ltl/printer.hpp"
#include "ltl/tree_deleter.hpp"
#include "utils/instanceof.hpp"
#include "utils/logging.hpp"

using namespace alice;

std::pair<ts::TransitionSystem*, automata::NondeterministicBuechiAutomata*>
preprocess(ts::TransitionSystem* ts, ltl::LTLFormula* formula,
           std::ostream& log_stream) {
  // Step 1. Take the negation.
  ltl::LTLFormula* neg = nullptr;
  bool delete_neg = false;
  if (utils:: instanceof <ltl::Negation>(formula)) {
    neg = formula->left_son_;
  } else {
    neg = new ltl::Negation(formula);
    delete_neg = true;
  }

  // Step 2. Collect AP.
  std::set<std::string> AP;
  for (auto prop : ts->AP_) {
    AP.insert(prop->name_);
  }

  // Step 3. Analyze the closure.
  auto analyzer = ltl::ClosureAnalyzer(&AP);
  analyzer.visit(neg);
  analyzer.getElementarySet();
  analyzer.show(log_stream);
  log_stream << std::endl;

  // Step 4. LTL to GNBA.
  auto gnba = algo::fromLTL(neg, analyzer, AP);
  gnba->show(log_stream);
  log_stream << std::endl;

  // Step 5. GNBA to NBA.
  auto nba = algo::fromGNBA(gnba);
  nba->show(log_stream);
  log_stream << std::endl;

  // Step 6. TS \times NBA.
  auto product = algo::productTSAndNBA(ts, nba);
  product->show(log_stream, "Producted TS");

  delete gnba;
  if (delete_neg) delete neg;

  return std::make_pair(product, nba);
}

int main() {
  LOG(INFO) << "Loading Transition System ...";
  frontend::TSBuilder builder("../data/TS.txt");
  std::ofstream file_ts("../run/TS.txt");
  builder.get()->show(file_ts, "Input TS");

  LOG(INFO) << "Loading LTL Benchmark ...";
  frontend::Benchmark benchmark("../data/benchmark.txt");
  std::ofstream file_ltl("../run/LTL.txt");
  benchmark.show(file_ltl);

  // Global Formulas Check
  while (true) {
    auto formula = benchmark.next_glb();
    if (formula == nullptr) {
      break;
    }

    LOG(INFO) << "Checking Global Formulas ... [" << benchmark.glb_cursor_
              << "/" << benchmark.glb_num_ << "]";
    std::ofstream log_file("../run/Check_Log_Global_Formula_" +
                           std::to_string(benchmark.glb_cursor_) + ".txt");

    auto processed = preprocess(builder.get(), formula, log_file);
    std::cout << int(algo::persistenceChecking(processed.first)) << std::endl;

    delete processed.first;
    delete processed.second;
  }

  // Local Formulas Check
  while (true) {
    auto pair = benchmark.next_loc();
    if (pair.second == nullptr) {
      break;
    }

    LOG(INFO) << "Checking Local Formulas ... [" << benchmark.loc_cursor_ << "/"
              << benchmark.loc_num_ << "]";
    std::ofstream log_file("../run/Check_Log_Local_Formula_" +
                           std::to_string(benchmark.loc_cursor_) + ".txt");

    auto processed = preprocess(builder.get(), pair.second, log_file);
    auto start_state = builder.get()->states_[pair.first];
    std::cout << int(algo::persistenceChecking(processed.first, start_state,
                                               processed.second))
              << std::endl;

    delete processed.first;
    delete processed.second;
  }

  return 0;
}
