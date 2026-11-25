#include "Application.h"

#include <exception>
#include <iostream>

int main() {
    int exitCode = 0;
    try {
        Application{}.run();
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        exitCode = 1;
    }
    return exitCode;
}
