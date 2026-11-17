#pragma once

#include "BrowserLauncher.h"
#include "ConsoleInput.h"
#include "ResultPrinter.h"
#include "SearchResponseParser.h"
#include "WikipediaClient.h"

class Application {
public:
    void run() const;

private:
    ConsoleInput input;
    WikipediaClient client;
    SearchResponseParser parser;
    ResultPrinter printer;
    BrowserLauncher browser;
};
