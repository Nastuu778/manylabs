#include "ICounter.h"
#include <fstream>

LineCounter::LineCounter(std::shared_ptr<ILogger> log) : logger(log) {}

CountResult LineCounter::count(const std::string& path) {
    CountResult res;
    std::ifstream file(path);
    std::string line;
    bool inBlockComment = false;

    if (!file.is_open()) {
        if (logger) logger->log("Error: File not found - " + path);
        return res;
    }

    if (logger) logger->log("Processing: " + path);

    while (std::getline(file, line)) {
        res.physical++;
        
        // Проверяем, есть ли в строке код (не только комментарии)
        bool hasCode = false;
        bool isComment = false;
        bool wasInBlock = inBlockComment;
        
        // Простая проверка: ищем код до комментариев
        size_t pos = 0;
        std::string trimmedLine = line;
        
        // Удаляем ведущие пробелы
        size_t start = trimmedLine.find_first_not_of(" \t");
        if (start != std::string::npos) {
            trimmedLine = trimmedLine.substr(start);
        } else {
            trimmedLine = "";
        }
        
        // Если внутри блочного комментария
        if (inBlockComment) {
            isComment = true;
            if (trimmedLine.find("*/") != std::string::npos) {
                inBlockComment = false;
                // Проверяем, есть ли код после */
                size_t codePos = trimmedLine.find("*/") + 2;
                std::string afterBlock = trimmedLine.substr(codePos);
                size_t commentPos = afterBlock.find("//");
                if (commentPos != std::string::npos) {
                    afterBlock = afterBlock.substr(0, commentPos);
                }
                // Удаляем пробелы
                afterBlock.erase(0, afterBlock.find_first_not_of(" \t"));
                afterBlock.erase(afterBlock.find_last_not_of(" \t") + 1);
                if (!afterBlock.empty()) {
                    hasCode = true;
                    isComment = false;
                }
            }
        } else {
            // Проверяем на строковый комментарий
            size_t commentPos = trimmedLine.find("//");
            if (commentPos != std::string::npos) {
                // Есть комментарий, проверяем есть ли код до него
                std::string codePart = trimmedLine.substr(0, commentPos);
                codePart.erase(0, codePart.find_first_not_of(" \t"));
                codePart.erase(codePart.find_last_not_of(" \t") + 1);
                if (!codePart.empty()) {
                    hasCode = true;  // Есть код до комментария
                } else {
                    isComment = true;  // Только комментарий
                }
            }
            // Проверяем на блочный комментарий
            else if (trimmedLine.find("/*") != std::string::npos) {
                isComment = true;
                if (trimmedLine.find("*/") == std::string::npos) {
                    inBlockComment = true;  // Не закрылся в этой строке
                }
            }
            // Обычная строка (без комментариев)
            else {
                if (!trimmedLine.empty()) {
                    hasCode = true;
                }
            }
        }
        
        // Подсчитываем
        if (hasCode) {
            res.logical++;
        } else if (isComment) {
            res.comments++;
        }
    }
    
    if (logger) logger->log("Counting finished.");
    return res;
}

std::shared_ptr<ICounter> LineCounterCreator::create(std::shared_ptr<ILogger> log) {
    return std::make_shared<LineCounter>(log);
}