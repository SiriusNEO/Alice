#include "parser.hpp"

#include <memory>

#include "../utils/logging.hpp"

extern std::shared_ptr<alice::ltl::LTLFormula> root;
extern int yyparse();
extern void* set_input(const char* in);
extern void destory_buffer(void* buffer);

namespace alice {

namespace ltl {

alice::ltl::LTLFormula* parseLTLFromString(const std::string& input) {
  auto buffer = set_input(input.c_str());
  yyparse();
  destory_buffer(buffer);
  return root.get();
}

}  // namespace ltl

}  // namespace alice
