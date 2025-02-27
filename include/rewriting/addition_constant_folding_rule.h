#pragma once

#include "rewriting/rewrite_rule.h"

namespace Expression {

class AdditionConstantFoldingRule : public RewriteRule {
public:
    virtual bool matches(const Node* node) const override {
        if (auto add = dynamic_cast<const AdditionNode*>(node)) {
            return dynamic_cast<const NumberNode*>(add->left) &&
                   dynamic_cast<const NumberNode*>(add->right);
        }
        return false;
    }
    virtual Node* apply(const Node* node, NodeFactory &factory) const override {
        auto add = static_cast<const AdditionNode*>(node);
        auto leftNum = dynamic_cast<const NumberNode*>(add->left);
        auto rightNum = dynamic_cast<const NumberNode*>(add->right);
        if (leftNum && rightNum) {
            double sum = leftNum->getValue() + rightNum->getValue();
            return factory.num(sum);
        }
        return nullptr;
    }
};

} // namespace Expression
