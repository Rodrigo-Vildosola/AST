#pragma once

#include "rewriting/rewrite_rule.h"

namespace Expression {

class MultiplicationConstantFoldingRule : public RewriteRule {
public:
    virtual bool matches(const Node* node) const override {
        if (auto mul = dynamic_cast<const MultiplicationNode*>(node)) {
            return dynamic_cast<const NumberNode*>(mul->left) &&
                   dynamic_cast<const NumberNode*>(mul->right);
        }
        return false;
    }
    virtual Node* apply(const Node* node, NodeFactory &factory) const override {
        auto mul = static_cast<const MultiplicationNode*>(node);
        auto leftNum = dynamic_cast<const NumberNode*>(mul->left);
        auto rightNum = dynamic_cast<const NumberNode*>(mul->right);
        if (leftNum && rightNum) {
            double prod = leftNum->getValue() * rightNum->getValue();
            return factory.num(prod);
        }
        return nullptr;
    }
};

} // namespace Expression
