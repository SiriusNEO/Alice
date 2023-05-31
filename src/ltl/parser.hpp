/*!
 * \file src/ltl/parser.hpp
 * \brief Parser for LTL formulas using Flex/Bison.
 * \author SiriusNEO
 */

#include "ast.hpp"

#ifndef LTL_PARSER_HPP_
#define LTL_PARSER_HPP_

namespace alice {

namespace ltl {

alice::ltl::LTLFormula* parseLTLFromString(const std::string& input);

}  // namespace ltl

}  // namespace alice

#endif  // LTL_PARSER_HPP_
