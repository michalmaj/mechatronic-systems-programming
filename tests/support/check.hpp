#pragma once

#include <cstdlib>
#include <iostream>
#include <string_view>

inline void psmCheck(bool condition, std::string_view description) {
    if (!condition) {
        std::cerr << "CHECK FAILED: " << description << '\n';
        std::exit(1);
    }
}
