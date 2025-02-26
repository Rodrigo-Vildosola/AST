#include "tracing/trace.h"

namespace Expression {

// Definition of static members.
std::ostringstream Trace::oss;
std::vector<std::string> Trace::messages;
std::vector<TransformationStep> Trace::transformationSteps;

void Trace::add(const std::string& message) {
    // Add message to plain text stream.
    oss << message << "\n";
    // Also store in the messages vector.
    messages.push_back(message);
}

void Trace::addTransformation(const std::string& description, const std::string& before, const std::string& after) {
    TransformationStep step{description, before, after};
    transformationSteps.push_back(step);
    // Also log to the plain text trace.
    std::ostringstream ossStep;
    ossStep << "Transformation: " << description << "\n"
            << "    Before: " << before << "\n"
            << "    After : " << after;
    add(ossStep.str());
}

void Trace::clear() {
    oss.str("");
    oss.clear();
    messages.clear();
    transformationSteps.clear();
}

std::string Trace::getTrace() {
    return oss.str();
}

std::string Trace::exportToJson() {
    // Create a JSON object.
    nlohmann::json j;
    j["messages"] = messages;
    
    // Build an array of transformation steps.
    j["transformationSteps"] = nlohmann::json::array();
    for (const auto& step : transformationSteps) {
        nlohmann::json jStep;
        jStep["description"] = step.description;
        jStep["before"] = step.before;
        jStep["after"] = step.after;
        j["transformationSteps"].push_back(jStep);
    }
    
    // Return the JSON dump (pretty-printed with indent of 4 spaces).
    return j.dump(4);
}

} // namespace Expression
