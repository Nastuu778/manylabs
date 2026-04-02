#pragma once
#include "ILogger.h"
#include <string>
#include <memory>

// Результат подсчета
struct CountResult {
    int physical = 0;
    int logical = 0;
    int comments = 0;
};

// 1. Интерфейс логики (Домен)
class ICounter {
public:
    virtual CountResult count(const std::string& path) = 0;
    virtual ~ICounter() = default;
};

// 2. Конкретная логика
class LineCounter : public ICounter {
    std::shared_ptr<ILogger> logger;
public:
    LineCounter(std::shared_ptr<ILogger> log);
    CountResult count(const std::string& path) override;
};

// 3. Фабрика логики (Factory Method - Обязательное правило)
class CounterCreator {
public:
    virtual std::shared_ptr<ICounter> create(std::shared_ptr<ILogger> log) = 0;
    virtual ~CounterCreator() = default;
};

class LineCounterCreator : public CounterCreator {
public:
    std::shared_ptr<ICounter> create(std::shared_ptr<ILogger> log) override;
};