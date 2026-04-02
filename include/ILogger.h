#pragma once
#include <string>
#include <memory>

// 1. Интерфейс логгера
class ILogger {
public:
    virtual void log(const std::string& msg) = 0;
    virtual ~ILogger() = default;
};

// 2. Фабрика логгеров (Factory Method)
class LoggerFactory {
public:
    virtual std::shared_ptr<ILogger> create() = 0;
    virtual ~LoggerFactory() = default;
};

class ConsoleLoggerFactory : public LoggerFactory {
public:
    std::shared_ptr<ILogger> create() override;
};

class FileLoggerFactory : public LoggerFactory {
public:
    std::shared_ptr<ILogger> create() override;
};