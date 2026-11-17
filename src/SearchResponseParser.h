#pragma once

#include "Article.h"

#include <string>
#include <vector>

class SearchResponseParser {
public:
    std::vector<Article> parse(const std::string& response) const;
};
