#include "Application.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <exception>
#include <iostream>

int main() {
#ifdef _WIN32
    const UINT previousCodePage = GetConsoleOutputCP();
    SetConsoleOutputCP(CP_UTF8);
#endif
    int exitCode = 0;
    try {
        Application{}.run();
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        exitCode = 1;
    }
    std::cout.flush();
#ifdef _WIN32
    if (previousCodePage != 0) {
        SetConsoleOutputCP(previousCodePage);
    }
#endif
    return exitCode;
}
