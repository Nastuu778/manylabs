#include "ConsoleUI.h"
#include <iostream>

ConsoleUI::ConsoleUI(std::shared_ptr<ICounter> c, std::shared_ptr<ILogger> l) 
    : counter(c), logger(l) {}

void ConsoleUI::run() {
    std::string path;
    std::cout << "Enter file path: ";
    std::cin >> path;

    if(logger) logger->log("Processing: " + path);

    // Правило: UI вызывает методы логики
    CountResult res = counter->count(path);

    // Правило: Вывод в отдельном классе
    std::cout << "\nPhysical: " << res.physical 
              << "\nLogical:  " << res.logical 
              << "\nComments: " << res.comments << std::endl;
}