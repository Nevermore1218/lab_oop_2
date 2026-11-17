#pragma once

#include <string>

class WikipediaClient {
public:
    std::string search(const std::string& query) const;
};
