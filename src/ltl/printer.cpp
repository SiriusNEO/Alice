#include "printer.hpp"

namespace alice {

namespace ltl {

void LTLPrinter::_visit(Negation* node) {
  this->str_ += "(!";
  this->visit(node->left_son_);
  this->str_ += ")";
}

void LTLPrinter::_visit(Conjunction* node) {
  this->str_ += "(";
  this->visit(node->left_son_);
  this->str_ += " /\\ ";
  this->visit(node->right_son_);
  this->str_ += ")";
}

void LTLPrinter::_visit(Disjunction* node) {
  this->str_ += "(";
  this->visit(node->left_son_);
  this->str_ += " \\/ ";
  this->visit(node->right_son_);
  this->str_ += ")";
}

void LTLPrinter::_visit(Implication* node) {
  this->str_ += "(";
  this->visit(node->left_son_);
  this->str_ += " -> ";
  this->visit(node->right_son_);
  this->str_ += ")";
}

void LTLPrinter::_visit(Next* node) {
  this->str_ += "(X";
  this->visit(node->left_son_);
  this->str_ += ")";
}

void LTLPrinter::_visit(Always* node) {
  this->str_ += "(G";
  this->visit(node->left_son_);
  this->str_ += ")";
}

void LTLPrinter::_visit(Eventually* node) {
  this->str_ += "(F";
  this->visit(node->left_son_);
  this->str_ += ")";
}

void LTLPrinter::_visit(Until* node) {
  this->str_ += "(";
  this->visit(node->left_son_);
  this->str_ += " U ";
  this->visit(node->right_son_);
  this->str_ += ")";
}

void LTLPrinter::_visit(AtomicProposition* node) {
  this->str_ += *node->ap_name_;
}

void LTLPrinter::_visit(True* node) { this->str_ += "true"; }

}  // namespace ltl

}  // namespace alice
