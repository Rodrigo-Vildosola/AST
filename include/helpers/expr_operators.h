#pragma once

#include "helpers/expr.h"

namespace Expression {

/// This macro expands to 6 overloads for (T OP Expr) and (Expr OP T)
/// covering T = int, float, double.  We call NodeFactory::FACTORY_FN(...) internally.
#define MAKE_BINARY_OP(OP, FACTORY_FN)                                             \
/* int OP Expr */                                                                  \
inline Expr operator OP (int lhs, const Expr &rhs) {                               \
    NodeFactory &f = rhs.factory;                                                  \
    return Expr(f.FACTORY_FN(f.num(lhs), rhs.node), f);                            \
}                                                                                  \
/* float OP Expr */                                                                \
inline Expr operator OP (float lhs, const Expr &rhs) {                             \
    NodeFactory &f = rhs.factory;                                                  \
    return Expr(f.FACTORY_FN(f.num(lhs), rhs.node), f);                            \
}                                                                                  \
/* double OP Expr */                                                               \
inline Expr operator OP (double lhs, const Expr &rhs) {                            \
    NodeFactory &f = rhs.factory;                                                  \
    return Expr(f.FACTORY_FN(f.num(lhs), rhs.node), f);                            \
}                                                                                  \
/* Expr OP int */                                                                  \
inline Expr operator OP (const Expr &lhs, int rhs) {                               \
    NodeFactory &f = lhs.factory;                                                  \
    return Expr(f.FACTORY_FN(lhs.node, f.num(rhs)), f);                            \
}                                                                                  \
/* Expr OP float */                                                                \
inline Expr operator OP (const Expr &lhs, float rhs) {                             \
    NodeFactory &f = lhs.factory;                                                  \
    return Expr(f.FACTORY_FN(lhs.node, f.num(rhs)), f);                            \
}                                                                                  \
/* Expr OP double */                                                               \
inline Expr operator OP (const Expr &lhs, double rhs) {                            \
    NodeFactory &f = lhs.factory;                                                  \
    return Expr(f.FACTORY_FN(lhs.node, f.num(rhs)), f);                            \
}

// Now we instantiate the macro for +, -, *, /, and ^

MAKE_BINARY_OP(+, add)
MAKE_BINARY_OP(-, sub)
MAKE_BINARY_OP(*, mul)
MAKE_BINARY_OP(/, div)
MAKE_BINARY_OP(^, exp)
MAKE_BINARY_OP(==, eq)


// We do NOT define operator==(int, Expr) because that's normally a boolean, etc.

} // namespace Expression
