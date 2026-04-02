#include "ILogger.h"
#include <iostream>
#include <fstream>

// Реализация консольного логгера
class ConsoleLogger : public ILogger {
public:
    void log(const std::string& msg) override {
        std::cout << "[LOG]: " << msg << std::endl;
    }
};

// Реализация файлового логгера
class FileLogger : public ILogger {
public:
    void log(const std::string& msg) override {
        std::ofstream out("log.txt", std::ios::app);
        if (out) out << "[LOG]: " << msg << std::endl;
    }
};

// Фабричные методы
std::shared_ptr<ILogger> ConsoleLoggerFactory::create() {
    return std::make_shared<ConsoleLogger>();
}

std::shared_ptr<ILogger> FileLoggerFactory::create() {
    return std::make_shared<FileLogger>();
}