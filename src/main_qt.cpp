#include "ILogger.h"
#include "ICounter.h"
#include "MainWindow.h"
#include "QtLogger.h"
#include <QApplication>
#include <memory>

// Точка входа для графической версии приложения
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 1. Создание фабрик
    auto logFactory = std::make_shared<QtLoggerFactory>();
    auto counterFactory = std::make_shared<LineCounterCreator>();

    // 2. Создание объектов через Factory Method
    auto logger = logFactory->create();
    auto counter = counterFactory->create(logger);

    // 3. Создание и отображение главного окна
    MainWindow window(counter, logger);
    window.show();

    return app.exec();
}