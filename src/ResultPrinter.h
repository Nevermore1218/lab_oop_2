#pragma once

#include "Article.h"

#include <vector>

class ResultPrinter {
public:
    void print(const std::vector<Article>& articles) const;
};
