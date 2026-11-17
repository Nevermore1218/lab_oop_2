#include "ResultPrinter.h"

#include <iostream>

void ResultPrinter::print(const std::vector<Article>& articles) const {
    if (articles.empty()) {
        std::cout << "Ничего не найдено. Попробуйте другой запрос.\n";
        return;
    }
    std::cout << "Найденные статьи:\n";
    for (std::size_t i = 0; i < articles.size(); ++i) {
        std::cout << i + 1 << ". " << articles[i].title << '\n';
    }
}
