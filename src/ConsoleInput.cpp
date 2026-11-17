#include "ConsoleInput.h"

#include <windows.h>

#include <charconv>
#include <iostream>
#include <stdexcept>

namespace {
std::string trim(const std::string& line) {
    constexpr char spaces[] = " \t\r\n\v\f";
    const auto first = line.find_first_not_of(spaces);
    if (first == std::string::npos) {
        return {};
    }
    return line.substr(first, line.find_last_not_of(spaces) - first + 1);
}
}

std::optional<std::string> ConsoleInput::readLine() const {
    const HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    if (!GetConsoleMode(input, &mode)) {
        std::string line;
        if (std::getline(std::cin, line)) {
            return trim(line);
        }
        return std::nullopt;
    }

    // В консоли Windows читаем UTF-16 напрямую, независимо от кодовой страницы ввода.
    std::wstring line;
    wchar_t buffer[256];
    while (line.empty() || line.back() != L'\n') {
        DWORD count = 0;
        if (!ReadConsoleW(input, buffer, 256, &count, nullptr)) {
            throw std::runtime_error("Не удалось прочитать ввод из консоли.");
        }
        if (count == 0 || buffer[0] == L'\x1a') {
            return std::nullopt;
        }
        line.append(buffer, count);
    }
    const int length = static_cast<int>(line.size());
    const int bytes = WideCharToMultiByte(CP_UTF8, 0, line.data(), length, nullptr, 0, nullptr, nullptr);
    if (!bytes) {
        throw std::runtime_error("Не удалось декодировать введённую строку.");
    }
    std::string utf8(bytes, '\0');
    WideCharToMultiByte(CP_UTF8, 0, line.data(), length, utf8.data(), bytes, nullptr, nullptr);
    return trim(utf8);
}

std::optional<std::string> ConsoleInput::readQuery() const {
    while (true) {
        std::cout << "Введите поисковый запрос (:q для выхода): " << std::flush;
        auto line = readLine();
        if (!line || *line == ":q") {
            return std::nullopt;
        }
        if (!line->empty()) {
            return line;
        }
        std::cout << "Пустой запрос. Введите слово или фразу.\n";
    }
}

std::optional<std::size_t> ConsoleInput::readChoice(std::size_t articleCount) const {
    while (true) {
        std::cout << "Номер статьи (0 для нового поиска, :q для выхода): " << std::flush;
        const auto line = readLine();
        if (!line || *line == ":q") {
            return std::nullopt;
        }
        std::size_t number = 0;
        const auto end = line->data() + line->size();
        const auto result = std::from_chars(line->data(), end, number);
        if (result.ec == std::errc{} && result.ptr == end && number <= articleCount) {
            return number;
        }
        std::cout << "Введите целое число от 0 до " << articleCount << ".\n";
    }
}
