#pragma once

#include "_pch.h"

// Example Token struct
namespace Expression {

enum class TokenType {
    Number,   // e.g. "3.14"
    Variable, // e.g. "x"
    Operator  // e.g. "+", "-", "*", "/", "^", "=="
};

struct Token {
    TokenType type;
    std::string text;   // the raw text, e.g. "x", "+", "=="
    double value;       // if it's a Number, store the numeric value here.

    Token(TokenType t, const std::string& s, double v = 0.0)
        : type(t), text(s), value(v) {}
};

} // namespace Expression
