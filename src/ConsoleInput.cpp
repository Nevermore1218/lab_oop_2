#include "ConsoleInput.h"

#include <charconv>
#include <iostream>

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
    std::string line;
    if (std::getline(std::cin, line)) {
        return trim(line);
    }
    return std::nullopt;
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
