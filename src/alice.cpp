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
  LOG(INFO) << "Loading Transition System ...";
  frontend::TSBuilder builder("../data/TS.txt");
  std::ofstream file_ts("../run/TS.txt");
  builder.get()->show(file_ts);

  LOG(INFO) << "Loading LTL Benchmark ...";
  frontend::Benchmark benchmark("../data/LTL.txt");
  std::ofstream file_ltl("../run/LTL.txt");
  benchmark.show(file_ltl);

  return 0;
}
