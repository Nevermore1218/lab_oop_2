#include "SearchResponseParser.h"

#include <nlohmann/json.hpp>
#include <limits>
#include <stdexcept>

std::vector<Article> SearchResponseParser::parse(const std::string& response) const {
    using nlohmann::json;
    try {
        const auto root = json::parse(response);
        if (root.contains("error") || root.contains("errors")) {
            throw std::runtime_error("Википедия вернула ошибку API. Попробуйте изменить запрос или повторить его позже.");
        }
        const auto& results = root.at("query").at("search").get_ref<const json::array_t&>();
        std::vector<Article> articles;
        for (const auto& result : results) {
            const auto& id = result.at("pageid");
            const auto& title = result.at("title").get_ref<const std::string&>();
            if (!id.is_number_unsigned() || id == 0 ||
                id.get<std::uint64_t>() > std::numeric_limits<std::int64_t>::max() || title.empty()) {
                throw std::runtime_error("Википедия прислала некорректные данные статьи.");
            }
            articles.push_back({id.get<std::int64_t>(), title});
        }
        return articles;
    } catch (const json::exception&) {
        throw std::runtime_error("Не удалось разобрать ответ Википедии.");
    }
}
