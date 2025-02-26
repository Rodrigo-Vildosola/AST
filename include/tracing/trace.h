#ifndef TRACE_HPP
#define TRACE_HPP

#include "_pch.h"
#include <nlohmann/json.hpp>

namespace Expression {

struct TransformationStep {
    std::string description;
    std::string before;
    std::string after;
};

class Trace {
public:
    // Add a plain evaluation or informational message.
    static void add(const std::string& message);
    
    // Add a transformation step with a description and before/after expressions.
    static void addTransformation(const std::string& description, const std::string& before, const std::string& after);
    
    // Clear all stored messages and transformation steps.
    static void clear();
    
    // Get a plain text trace (for quick logging).
    static std::string getTrace();
    
    // Export the complete trace (messages and transformation steps) as a JSON string.
    static std::string exportToJson();

private:
    // For legacy plain text logging.
    static std::ostringstream oss;
    
    // Structured storage for messages.
    static std::vector<std::string> messages;
    
    // Structured storage for transformation steps.
    static std::vector<TransformationStep> transformationSteps;
};

} // namespace Expression

#endif
