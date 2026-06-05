#pragma once
#include "ILogger.h"
#include <QObject>
#include <QString>
#include <string>
#include <memory>

// Qt-реализация логгера, передающая сообщения через сигналы/слоты
class QtLogger : public QObject, public ILogger
{
    Q_OBJECT
public:
    explicit QtLogger(QObject *parent = nullptr);
    void log(const std::string &msg) override;

signals:
    void newLogMessage(const QString &message);
};

// Фабрика для создания QtLogger
class QtLoggerFactory : public LoggerFactory
{
public:
    std::shared_ptr<ILogger> create() override;
};