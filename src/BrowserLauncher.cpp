#include "BrowserLauncher.h"

#include <windows.h>
#include <shellapi.h>

#include <stdexcept>

void BrowserLauncher::open(const Article& article) const {
    const std::wstring url = L"https://ru.wikipedia.org/w/index.php?curid=" + std::to_wstring(article.pageId);
    const auto result = ShellExecuteW(nullptr, L"open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    if (reinterpret_cast<INT_PTR>(result) <= 32) {
        throw std::runtime_error("Не удалось открыть браузер. Ссылка на статью: "
            "https://ru.wikipedia.org/w/index.php?curid=" + std::to_string(article.pageId));
    }
}
