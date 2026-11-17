#include "WikipediaClient.h"

#include <windows.h>
#include <winhttp.h>

#include <memory>
#include <stdexcept>

namespace {
using HttpHandle = std::unique_ptr<void, decltype(&WinHttpCloseHandle)>;

HttpHandle checkedHandle(HINTERNET handle, const char* message) {
    if (!handle) {
        throw std::runtime_error(message);
    }
    return HttpHandle(handle, WinHttpCloseHandle);
}

std::wstring encodeQuery(const std::string& utf8) {
    constexpr wchar_t hex[] = L"0123456789ABCDEF";
    std::wstring encoded;
    for (unsigned char byte : utf8) {
        if ((byte >= 'a' && byte <= 'z') || (byte >= 'A' && byte <= 'Z') ||
            (byte >= '0' && byte <= '9') || byte == '-' || byte == '_' ||
            byte == '.' || byte == '~') {
            encoded += static_cast<wchar_t>(byte);
        } else {
            encoded += L'%';
            encoded += hex[byte >> 4];
            encoded += hex[byte & 0x0F];
        }
    }
    return encoded;
}
}

std::string WikipediaClient::search(const std::string& query) const {
    auto session = checkedHandle(WinHttpOpen(L"Lab2WikiSearch/1.0 (educational console client)",
        WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0),
        "Не удалось запустить HTTP-клиент.");
    if (!WinHttpSetTimeouts(session.get(), 5000, 8000, 8000, 15000)) {
        throw std::runtime_error("Не удалось настроить сетевые тайм-ауты.");
    }
    auto connection = checkedHandle(WinHttpConnect(session.get(), L"ru.wikipedia.org",
        INTERNET_DEFAULT_HTTPS_PORT, 0), "Не удалось подключиться к Википедии.");
    const auto path = L"/w/api.php?action=query&list=search&format=json&utf8=1&srprop=&srlimit=10&srsearch="
                    + encodeQuery(query);
    auto request = checkedHandle(WinHttpOpenRequest(connection.get(), L"GET", path.c_str(), nullptr,
        WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE | WINHTTP_FLAG_ESCAPE_DISABLE),
        "Не удалось создать запрос к Википедии.");
    if (!WinHttpSendRequest(request.get(), WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) ||
        !WinHttpReceiveResponse(request.get(), nullptr)) {
        throw std::runtime_error("Ошибка сети. Проверьте соединение и повторите поиск.");
    }

    DWORD status = 0;
    DWORD size = sizeof(status);
    if (!WinHttpQueryHeaders(request.get(), WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                             WINHTTP_HEADER_NAME_BY_INDEX, &status, &size, WINHTTP_NO_HEADER_INDEX)) {
        throw std::runtime_error("Не удалось прочитать статус ответа Википедии.");
    }
    if (status != 200) {
        throw std::runtime_error("Википедия вернула HTTP " + std::to_string(status) + ".");
    }

    std::string response;
    char buffer[8192];
    DWORD count = 0;
    do {
        if (!WinHttpReadData(request.get(), buffer, sizeof(buffer), &count)) {
            throw std::runtime_error("Соединение прервано при получении результатов поиска.");
        }
        if (response.size() + count > 1024 * 1024) {
            throw std::runtime_error("Ответ Википедии слишком велик.");
        }
        response.append(buffer, count);
    } while (count > 0);
    return response;
}
