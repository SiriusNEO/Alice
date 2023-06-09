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
  LOG(INFO) << "test ltl parser";
  std::string input = "G(a \\/ b)";
  ltl::TreeDeleter deleter;
  auto root = ltl::parseLTLFromString(input);
  std::cout << "parse result: " << ltl::toString(root) << std::endl;
  deleter.visit(root);

  LOG(INFO) << "test ts builder";
  frontend::TSBuilder builder("../ref/example_TS.txt");
  builder.get()->print();

  LOG(INFO) << "test ts benchmark";
  frontend::Benchmark benchmark("../ref/example_LTL.txt");
  benchmark.print();

  LOG(INFO) << "test closure analyzer";
  std::string input1 = "a U ((!a) /\\ b)";
  auto root1 = ltl::parseLTLFromString(input1);
  std::cout << "parse result: " << ltl::toString(root1) << std::endl;
  ltl::ClosureAnalyzer analyzer;
  analyzer.visit(root1);
  analyzer.getElementarySet();
  analyzer.show();

  return 0;
}
