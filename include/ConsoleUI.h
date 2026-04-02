#pragma once
#include "ICounter.h"
#include "ILogger.h"
#include <memory>

// Слой представления (UI)
class ConsoleUI {
    std::shared_ptr<ICounter> counter;
    std::shared_ptr<ILogger> logger;
public:
    ConsoleUI(std::shared_ptr<ICounter> c, std::shared_ptr<ILogger> l);
    void run();
};