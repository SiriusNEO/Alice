#include "benchmark.hpp"

#include <sstream>

#include "../ltl/parser.hpp"
#include "../ltl/printer.hpp"
#include "../ltl/tree_deleter.hpp"
#include "../utils/logging.hpp"

namespace alice {

namespace frontend {

Benchmark::Benchmark(const char* file_path)
    : file_in_(),
      global_formulas_(),
      local_formulas_(),
      glb_cursor_(0),
      loc_cursor_(0) {
  file_in_.open(file_path, std::ios::in);
  CHECK(file_in_.is_open());

  int A, B;
  file_in_ >> A >> B;

  std::string buffer;

  // handle '\n' in the cache
  getline(file_in_, buffer);

  for (int i = 0; i < A; ++i) {
    getline(file_in_, buffer);
    global_formulas_.push_back(ltl::parseLTLFromString(buffer));
    buffer.clear();
  }

  for (int i = 0, si; i < B; ++i) {
    file_in_ >> si;
    getline(file_in_, buffer);
    LOG(INFO) << buffer;
    local_formulas_.push_back(
        std::make_pair(si, ltl::parseLTLFromString(buffer)));
    buffer.clear();
  }
}

Benchmark::~Benchmark() {
  auto deleter = ltl::TreeDeleter();
  for (auto formula : global_formulas_) deleter.visit(formula);
  for (auto formula : local_formulas_) deleter.visit(formula.second);
}

ltl::LTLFormula* Benchmark::next_glb() {
  return global_formulas_[glb_cursor_++];
}

std::pair<int, ltl::LTLFormula*> Benchmark::next_loc() {
  return local_formulas_[loc_cursor_++];
}

void Benchmark::print() const {
  std::cout << "[Benchmark]" << std::endl;

  // print global formulas
  std::cout << "\tGlobal Formulas: " << std::endl;
  for (auto f : global_formulas_) {
    std::cout << "\t\t" << toString(f) << std::endl;
  }

  // print local formulas
  std::cout << "\tLocal Formulas: " << std::endl;
  for (auto p : local_formulas_) {
    std::cout << "\t\tsi=" << p.first << "\t" << toString(p.second)
              << std::endl;
  }
}

}  // namespace frontend

}  // namespace alice
