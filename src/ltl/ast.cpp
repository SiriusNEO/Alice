#include "ast.hpp"

namespace alice {

namespace ltl {

void LTLFormula::accept(LTLVisitor* visitor) {
  std::cerr << "Unreachable." << std::endl;
  exit(1);
}

void Negation::accept(LTLVisitor* visitor) { visitor->_visit(this); }

void Conjunction::accept(LTLVisitor* visitor) { visitor->_visit(this); }

void Disjunction::accept(LTLVisitor* visitor) { visitor->_visit(this); }

void Implication::accept(LTLVisitor* visitor) { visitor->_visit(this); }

void Next::accept(LTLVisitor* visitor) { visitor->_visit(this); }

void Always::accept(LTLVisitor* visitor) { visitor->_visit(this); }

void Eventually::accept(LTLVisitor* visitor) { visitor->_visit(this); }

void Until::accept(LTLVisitor* visitor) { visitor->_visit(this); }

void AtomicProposition::accept(LTLVisitor* visitor) { visitor->_visit(this); }

void True::accept(LTLVisitor* visitor) { visitor->_visit(this); }

}  // namespace ltl

}  // namespace alice
