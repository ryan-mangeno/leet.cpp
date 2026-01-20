#include <iostream>
#include <string>
#include "submission.h"

using namespace getcracked;

void check(std::string actual, std::string expected, const char* name) {
    if (actual == expected) {
        std::cout << "✅ " << name << ": Correct" << std::endl;
    } else {
        std::cout << "❌ " << name << ": Expected '" << expected << "', got '" << actual << "'" << std::endl;
        exit(1);
    }
}

int main() {
    std::cout << "--- LINUX & GIT KNOWLEDGE CHECK ---" << std::endl;
    check(LinuxQuiz::git_stage_command(), "git add", "Git Stage");
    check(LinuxQuiz::bash_redirect_stderr_to_stdout(), "2>&1", "Bash Redirect");
    check(LinuxQuiz::process_memory_sharing(), "no", "Process Memory");
    
    // Accept ps or top
    std::string p = LinuxQuiz::list_processes_cmd();
    if (p == "ps" || p == "top") std::cout << "✅ List Processes: Correct" << std::endl;
    else {
        std::cout << "❌ List Processes: Expected 'ps' or 'top', got '" << p << "'" << std::endl;
        exit(1);
    }
    return 0;
}
