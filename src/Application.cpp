#include "Application.h"

#include <iostream>
#include <stdexcept>

void Application::run() const {
    std::cout << "Поиск по русской Википедии\n";
    while (const auto query = input.readQuery()) {
        try {
            const auto articles = parser.parse(client.search(*query));
            printer.print(articles);
            if (articles.empty()) {
                continue;
            }
            const auto choice = input.readChoice(articles.size());
            if (!choice) {
                return;
            }
            if (*choice > 0) {
                const auto& article = articles[*choice - 1];
                browser.open(article);
                std::cout << "Открыта статья «" << article.title << "».\n";
            }
        } catch (const std::runtime_error& error) {
            std::cerr << error.what() << '\n';
        }
    }
}
