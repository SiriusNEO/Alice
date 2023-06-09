#include "tree_deleter.hpp"

#include "../utils/logging.hpp"

namespace alice {

namespace ltl {

void TreeDeleter::_visit(Negation* node) {
  this->visit(node->left_son_);
  delete node;
}

void TreeDeleter::_visit(Conjunction* node) {
  this->visit(node->left_son_);
  this->visit(node->right_son_);
  delete node;
}

void TreeDeleter::_visit(Next* node) {
  this->visit(node->left_son_);
  delete node;
}

void TreeDeleter::_visit(Until* node) {
  this->visit(node->left_son_);
  this->visit(node->right_son_);
  delete node;
}

void TreeDeleter::_visit(AtomicProposition* node) {
  delete node->ap_name_;
  delete node;
}

void TreeDeleter::_visit(True* node) { delete node; }

}  // namespace ltl

}  // namespace alice
