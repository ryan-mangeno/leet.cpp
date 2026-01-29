#ifndef PROFILER_H
#define PROFILER_H

#include <vector>
#include <string>

namespace leet_cpp {

enum class Kind {
    Begin,
    End,
};

struct Event {
    Kind Type;
    double Elapsed;
    std::string Method;
};

struct Sample {
    double Elapsed;
    std::vector<std::string> Stack;
};

using Events = std::vector<Event>;
using Samples = std::vector<Sample>;

Events GenerateEvents(const Samples& samples) {
    Events result;
    // TODO: Implement your solution
    // Hint: Track previous stack, compare with current
    return result;
}

} // namespace leet_cpp

#endif
