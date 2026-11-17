#include "Application.h"

#include <windows.h>

#include <exception>
#include <iostream>

int main() {
    const UINT previousCodePage = GetConsoleOutputCP();
    SetConsoleOutputCP(CP_UTF8);
    int exitCode = 0;
    try {
        Application{}.run();
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        exitCode = 1;
    }
    std::cout.flush();
    if (previousCodePage != 0) {
        SetConsoleOutputCP(previousCodePage);
    }
    return exitCode;
}
