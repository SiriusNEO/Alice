#include "frontend/benchmark.hpp"
#include "frontend/ts_builder.hpp"
#include "ltl/ast.hpp"
#include "ltl/parser.hpp"
#include "ltl/printer.hpp"
#include "utils/logging.hpp"

using namespace alice;

int main() {
  LOG(INFO) << "test ltl parser";
  std::string input = "G(a \\/ b)";

  ltl::LTLPrinter printer;
  auto root = ltl::parseLTLFromString(input);
  printer.visit(root);
  std::cout << "parse result: " << printer.str_ << std::endl;
  delete root;

  LOG(INFO) << "test ts builder";
  frontend::TSBuilder builder("../ref/example_TS.txt");
  builder.get()->print();

  LOG(INFO) << "test ts benchmark";
  frontend::Benchmark benchmark("../ref/example_LTL.txt");
  benchmark.print();

  return 0;
}
