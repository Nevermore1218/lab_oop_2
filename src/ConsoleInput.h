#pragma once

#include <cstddef>
#include <optional>
#include <string>

class ConsoleInput {
public:
    std::optional<std::string> readQuery() const;
    // Ноль означает новый поиск, конец ввода завершает программу.
    std::optional<std::size_t> readChoice(std::size_t articleCount) const;

private:
    std::optional<std::string> readLine() const;
};
