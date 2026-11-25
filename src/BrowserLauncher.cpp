#include "BrowserLauncher.h"

#include <spawn.h>
#include <sys/wait.h>
#include <cerrno>

extern char** environ;

#include <stdexcept>

void BrowserLauncher::open(const Article& article) const {
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
        throw std::runtime_error("Не удалось открыть браузер. Ссылка на статью: "
            "https://ru.wikipedia.org/w/index.php?curid=" + std::to_string(article.pageId));
    }
}
