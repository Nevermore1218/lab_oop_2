#pragma once

#include <optional>
#include <string>
#include <cstddef>

class ConsoleInput {
public:
    std::optional<std::string> readQuery() const;

    std::optional<std::size_t> readChoice(std::size_t articleCount) const;

private:
    std::optional<std::string> readLine() const;
};
