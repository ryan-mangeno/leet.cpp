#include <iostream>
#include "submission.h"

using namespace leet_cpp;

void log_result(const char* name, bool passed, const char* msg = "") {
    if (passed) {
        std::cout << "✅ [PASS] " << name << std::endl;
    } else {
        std::cout << "❌ [FAIL] " << name << ": " << msg << std::endl;
        exit(1);
    }
}

void Test_BasicOps() {
    SkipList list;
    list.insert(1);
    list.insert(2);
    list.insert(3);
    
    log_result("BasicSearch", list.search(2), "Should find 2");
    log_result("NotFound", !list.search(4), "Should not find 4");
}

void Test_Erase() {
    SkipList list;
    list.insert(1);
    list.insert(2);
    list.insert(3);
    
    log_result("EraseExists", list.erase(2), "Should erase 2");
    log_result("AfterErase", !list.search(2), "Should not find 2 after erase");
    log_result("EraseNonExist", !list.erase(4), "Should return false");
}

void Test_DuplicateInsert() {
    SkipList list;
    list.insert(1);
    list.insert(1);
    list.insert(1);
    
    log_result("Duplicates", list.search(1), "Should handle duplicates");
}

void Test_LargeDataset() {
    SkipList list;
    
    for (int i = 0; i < 1000; i += 2) {
        list.insert(i);
    }
    
    bool all_found = true;
    for (int i = 0; i < 1000; i += 2) {
        if (!list.search(i)) {
            all_found = false;
            break;
        }
    }
    
    log_result("LargeInsert", all_found, "Should find all elements");
}

int main() {
    std::cout << "--- ⚡ SKIP LIST TESTS ⚡ ---" << std::endl;
    Test_BasicOps();
    Test_Erase();
    Test_DuplicateInsert();
    Test_LargeDataset();
    std::cout << "--- 🏆 ALL TESTS PASSED 🏆 ---" << std::endl;
    return 0;
}
