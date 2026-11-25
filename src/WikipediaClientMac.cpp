#include "WikipediaClient.h"

#include <curl/curl.h>

#include <memory>
#include <stdexcept>

namespace {
class CurlRuntime {
public:
    CurlRuntime() {
        if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
            throw std::runtime_error("Не удалось запустить HTTP-клиент.");
        }
    }
    ~CurlRuntime() {
        curl_global_cleanup();
    }
};

std::size_t receiveData(char* data, std::size_t size, std::size_t count, void* context) noexcept {
    auto& response = *static_cast<std::string*>(context);
    constexpr std::size_t limit = 1024 * 1024;
    if (size != 0 && count > (limit - response.size()) / size) {
        return 0;
    }
    const auto bytes = size * count;
    try {
        response.append(data, bytes);
        return bytes;
    } catch (...) {
        return 0;
    }
}
}

std::string WikipediaClient::search(const std::string& query) const {
    static const CurlRuntime runtime;
    using CurlHandle = std::unique_ptr<CURL, decltype(&curl_easy_cleanup)>;
    CurlHandle request(curl_easy_init(), curl_easy_cleanup);
    if (!request) {
        throw std::runtime_error("Не удалось создать HTTP-запрос.");
    }
    using EncodedString = std::unique_ptr<char, decltype(&curl_free)>;
    EncodedString encoded(curl_easy_escape(request.get(), query.c_str(), 0), curl_free);
    if (!encoded) {
        throw std::runtime_error("Не удалось закодировать поисковый запрос.");
    }
    const std::string url = std::string("https://ru.wikipedia.org/w/api.php?action=query&list=search"
        "&format=json&utf8=1&srprop=&srlimit=10&srsearch=") + encoded.get();
    std::string response;
    auto setOption = [&](CURLoption option, auto value) {
        if (curl_easy_setopt(request.get(), option, value) != CURLE_OK) {
            throw std::runtime_error("Не удалось настроить HTTP-запрос.");
        }
    };
    setOption(CURLOPT_URL, url.c_str());
    setOption(CURLOPT_USERAGENT, "Lab2WikiSearch/1.0 (educational console client)");
    setOption(CURLOPT_CONNECTTIMEOUT, 8L);
    setOption(CURLOPT_TIMEOUT, 30L);
    setOption(CURLOPT_NOSIGNAL, 1L);
    setOption(CURLOPT_WRITEFUNCTION, &receiveData);
    setOption(CURLOPT_WRITEDATA, &response);
    const auto result = curl_easy_perform(request.get());
    if (result != CURLE_OK) {
        throw std::runtime_error(std::string("Ошибка получения ответа Википедии: ") + curl_easy_strerror(result));
    }
    long status = 0;
    if (curl_easy_getinfo(request.get(), CURLINFO_RESPONSE_CODE, &status) != CURLE_OK) {
        throw std::runtime_error("Не удалось прочитать статус ответа Википедии.");
    }
    if (status != 200) {
        throw std::runtime_error("Википедия вернула HTTP " + std::to_string(status) + ".");
    }
    return response;
}
