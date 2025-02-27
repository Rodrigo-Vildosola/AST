// File: rewriting/Rewriter.h
#pragma once

#include "rewriting/factorization_rule.h"
#include "rewriting/exp_product_rule.h"
#include "rewriting/exp_division_rule.h"
#include "rewriting/ln_difference_rule.h"
#include "rewriting/ln_addition_rule.h"
#include "rewriting/log_difference_rule.h"
#include "rewriting/log_addition_rule.h"


#include "rewriting/rewrite_rule.h"
#include "helpers/node_factory.h"

namespace Expression {

class Rewriter {
private:
    std::vector<std::unique_ptr<RewriteRule>> rules;
public:
    Rewriter() {
        // Register default rules.
        rules.push_back(std::make_unique<ExpProductRule>());
        rules.push_back(std::make_unique<ExpDivisionRule>());
        rules.push_back(std::make_unique<LnDifferenceRule>());
        rules.push_back(std::make_unique<LnAdditionRule>());
        rules.push_back(std::make_unique<LogDifferenceRule>());
        rules.push_back(std::make_unique<LogAdditionRule>());
        rules.push_back(std::make_unique<FactorizationRule>());
        // Add more rules as needed.
    }

    // Recursively apply rewriting rules to a node until no more changes occur.
    Node* rewrite(const Node* node, NodeFactory &factory) const {
        Node* current = node->clone(factory); // Start with a clone.
        bool changed = true;
        while (changed) {
            changed = false;
            for (const auto &rule : rules) {
                if (rule->matches(current)) {
                    Node* newNode = rule->apply(current, factory);
                    if (!newNode->toString().empty() && newNode->toString() != current->toString()) {
                        Trace::addTransformation("Rewrite", current->toString(), newNode->toString());
                        // Replace current with newNode.
                        delete current;
                        current = newNode;
                        changed = true;
                        break; // Restart rules on new node.
                    }
                }
            }
            // Optionally, traverse children and rewrite recursively.
            // For brevity, we assume our rules are applied at the top level.
        }
        return current;
    }
};

} // namespace Expression
