/*!
 * \file src/frontend/benchmark.hpp
 * \brief Benchmark: a set of LTL formulas.
 * \author SiriusNEO
 */

#include <fstream>
#include <utility>
#include <vector>

#include "../ltl/ast.hpp"

#ifndef FRONTEND_BENCHMARK_HPP_
#define FRONTEND_BENCHMARK_HPP_

namespace alice {

namespace frontend {

class Benchmark {
 private:
  std::ifstream file_in_;
  std::vector<ltl::LTLFormula*> global_formulas_;
  std::vector<std::pair<int, ltl::LTLFormula*>> local_formulas_;
  int glb_cursor_, loc_cursor_;

 public:
  explicit Benchmark(const char* file_path);

  ltl::LTLFormula* next_glb();

  std::pair<int, ltl::LTLFormula*> next_loc();

  ~Benchmark();

  void show(std::ostream& os) const;
};

}  // namespace frontend

}  // namespace alice

#endif  // FRONTEND_BENCHMARK_HPP_
