#include "ICounter.h"
#include <fstream>

// Конструктор счетчика
LineCounter::LineCounter(std::shared_ptr<ILogger> log) : logger(log) {}

// Логика подсчета (Правило: Логика отдельно от UI)
CountResult LineCounter::count(const std::string& path) {
    CountResult res;
    std::ifstream file(path);
    std::string line;
    bool inComment = false;

    if (!file.is_open()) return res;

    while (std::getline(file, line)) {
        res.physical++;
        bool isComment = false;
        bool isEmpty = line.find_first_not_of(" \t\r\n") == std::string::npos;

        if (inComment) {
            isComment = true;
            if (line.find("*/") != std::string::npos) inComment = false;
        } else {
            if (line.find("/*") != std::string::npos) {
                inComment = true; isComment = true;
            } else if (line.find("//") != std::string::npos) {
                isComment = true;
            }
        }

        if (isComment) res.comments++;
        else if (!isEmpty) res.logical++;
    }
    return res;
}

// Фабричный метод создания счетчика
std::shared_ptr<ICounter> LineCounterCreator::create(std::shared_ptr<ILogger> log) {
    return std::make_shared<LineCounter>(log);
}