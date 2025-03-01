#include "solver/token.h"
#include "node_factory.h"

// forward declare NodeFactory from your code
namespace Expression {
    class NodeFactory;
    class Node;        // your base AST node
}

namespace Expression {

inline Node* buildAstFromPostfix(const std::vector<Token> &tokens, NodeFactory &factory) 
{
    std::stack<Node*> st;

    for (const auto &tk : tokens) {
        switch (tk.type) {
        case TokenType::Number: {
            // create a NumberNode
            Node* numNode = factory.num(tk.value);
            st.push(numNode);
            break;
        }
        case TokenType::Variable: {
            // create a VariableNode
            Node* varNode = factory.var(tk.text);
            st.push(varNode);
            break;
        }
        case TokenType::Operator: {
            // We handle one of "+", "-", "*", "/", "^", "=="
            if (tk.text == "+" || tk.text == "-" || tk.text == "*" || 
                tk.text == "/" || tk.text == "^" || tk.text == "==")
            {
                // these are binary operators: pop two nodes
                if (st.size() < 2) {
                    throw std::runtime_error("Not enough operands for operator " + tk.text);
                }
                Node* right = st.top(); st.pop();
                Node* left  = st.top(); st.pop();

                Node* result = nullptr;
                if (tk.text == "+") {
                    result = factory.add(left, right);
                } else if (tk.text == "-") {
                    result = factory.sub(left, right);
                } else if (tk.text == "*") {
                    result = factory.mul(left, right);
                } else if (tk.text == "/") {
                    result = factory.div(left, right);
                } else if (tk.text == "^") {
                    result = factory.exp(left, right);
                } else if (tk.text == "==") {
                    result = factory.eq(left, right);
                }

                st.push(result);
            }
            else {
                // Possibly handle unary operators like "sin", "cos" if used in postfix,
                // or other operators. For now, we assume it's an unrecognized operator.
                throw std::runtime_error("Unrecognized operator: " + tk.text);
            }
            break;
        }
        } // end switch
    } // end for tokens

    // At the end, we expect exactly one node on the stack
    if (st.size() != 1) {
        throw std::runtime_error("Invalid postfix expression: stack size != 1");
    }
    return st.top();
}

} // namespace Expression
