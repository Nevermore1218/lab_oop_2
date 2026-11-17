#include "BrowserLauncher.h"

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#else
#include <spawn.h>
#include <sys/wait.h>
#include <cerrno>

extern char** environ;
#endif

#include <stdexcept>

void BrowserLauncher::open(const Article& article) const {
#ifdef _WIN32
    const std::wstring url = L"https://ru.wikipedia.org/w/index.php?curid=" + std::to_wstring(article.pageId);
    const auto result = ShellExecuteW(nullptr, L"open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    if (reinterpret_cast<INT_PTR>(result) <= 32) {
#else
    const std::string url = "https://ru.wikipedia.org/w/index.php?curid=" + std::to_string(article.pageId);
    char command[] = "/usr/bin/open";
    char* arguments[] = {command, const_cast<char*>(url.c_str()), nullptr};
    pid_t process;
    if (posix_spawn(&process, command, nullptr, nullptr, arguments, environ) != 0) {
        throw std::runtime_error("Не удалось запустить браузер.");
    }
    int status = 0;
    pid_t result;
    do {
        result = waitpid(process, &status, 0);
    } while (result == -1 && errno == EINTR);
    if (result == -1 || !WIFEXITED(status) || WEXITSTATUS(status) != 0) {
#endif
        throw std::runtime_error("Не удалось открыть браузер. Ссылка на статью: "
            "https://ru.wikipedia.org/w/index.php?curid=" + std::to_string(article.pageId));
    }
}
