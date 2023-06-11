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
  LOG(INFO) << "Test LTL parser";
  std::string input = "G(a \\/ b)";
  ltl::TreeDeleter deleter;
  auto root = ltl::parseLTLFromString(input);
  std::cout << "parse result: " << ltl::toString(root) << std::endl;
  deleter.visit(root);

  LOG(INFO) << "Test TS builder";
  frontend::TSBuilder builder("../data/TS.txt");
  builder.get()->show(std::cout);

  LOG(INFO) << "Test TS benchmark";
  frontend::Benchmark benchmark("../data/LTL.txt");
  benchmark.show(std::cout);

  LOG(INFO) << "Test closure analyzer";
  std::string input1 = "a U ((!a) /\\ b)";
  auto root1 = ltl::parseLTLFromString(input1);
  std::cout << "parse result: " << ltl::toString(root1) << std::endl;
  ltl::ClosureAnalyzer analyzer;
  analyzer.visit(root1);
  analyzer.getElementarySet();
  analyzer.show(std::cout);
  deleter.visit(root1);

  LOG(INFO) << "Test LTL to GNBA";

  return 0;
}
