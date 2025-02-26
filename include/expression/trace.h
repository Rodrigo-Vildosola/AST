#ifndef TRACE_HPP
#define TRACE_HPP

#include <string>
#include <sstream>

namespace Expression {

// A simple tracing utility to log evaluation steps.
class Trace {
public:
    static void add(const std::string& message);
    static void clear();
    static std::string getTrace();
private:
    static std::ostringstream oss;
};

} // namespace Expression

#endif
