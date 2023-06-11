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

  file_in_ >> glb_num_ >> loc_num_;

  std::string buffer;

  // handle '\n' in the cache
  getline(file_in_, buffer);

  for (int i = 0; i < glb_num_; ++i) {
    getline(file_in_, buffer);
    global_formulas_.push_back(ltl::parseLTLFromString(buffer));
    buffer.clear();
  }

  for (int i = 0, si; i < loc_num_; ++i) {
    file_in_ >> si;
    getline(file_in_, buffer);
    // LOG(INFO) << buffer;
    local_formulas_.push_back(
        std::make_pair(si, ltl::parseLTLFromString(buffer)));
    buffer.clear();
  }
}

Benchmark::~Benchmark() {
  auto deleter = ltl::TreeDeleter();
  for (const auto& formula : global_formulas_) deleter.visit(formula);
  for (const auto& formula : local_formulas_) deleter.visit(formula.second);
}

ltl::LTLFormula* Benchmark::next_glb() {
  if (glb_cursor_ == glb_num_) return nullptr;
  return global_formulas_[glb_cursor_++];
}

std::pair<int, ltl::LTLFormula*> Benchmark::next_loc() {
  if (loc_cursor_ == loc_num_) return std::make_pair(0, nullptr);
  return local_formulas_[loc_cursor_++];
}

void Benchmark::show(std::ostream& os) const {
  os << "[Benchmark]" << std::endl;

  // print global formulas
  os << "\tGlobal Formulas: (Total " << glb_num_ << ")" << std::endl;
  for (const auto& f : global_formulas_) {
    os << "\t\t" << toString(f) << std::endl;
  }

  // print local formulas
  os << "\tLocal Formulas: (Total " << loc_num_ << ")" << std::endl;
  for (const auto& p : local_formulas_) {
    os << "\t\tsi=" << p.first << "\t" << toString(p.second) << std::endl;
  }
}

}  // namespace frontend

}  // namespace alice
