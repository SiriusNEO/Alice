/*!
 * \file src/frontend/ts_builder.hpp
 * \brief Build the transition system from file.
 * \author SiriusNEO
 */

#include <fstream>

#include "../ts/ts.hpp"

#ifndef FRONTEND_TS_BUILDER_HPP_
#define FRONTEND_TS_BUILDER_HPP_

namespace alice {

namespace frontend {

class TSBuilder {
 private:
  std::ifstream file_in_;
  ts::TransitionSystem* ts_;

 public:
  explicit TSBuilder(const char* file_path);

  ~TSBuilder();

  ts::TransitionSystem* get() const;
};

}  // namespace frontend

}  // namespace alice

#endif  // FRONTEND_TS_BUILDER_HPP_
