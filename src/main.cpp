#include "ILogger.h"
#include "ICounter.h"
#include "ConsoleUI.h"
#include <memory>

int main() {
    // 1. Создание фабрик
    auto logFactory = std::make_shared<ConsoleLoggerFactory>();
    auto counterFactory = std::make_shared<LineCounterCreator>();

    // 2. Создание объектов через Factory Method
    auto logger = logFactory->create();
    auto counter = counterFactory->create(logger);

    // 3. Запуск UI (main только делегирует)
    ConsoleUI ui(counter, logger);
    ui.run();

    return 0;
}