#include "QtLogger.h"

QtLogger::QtLogger(QObject *parent) : QObject(parent) {}

void QtLogger::log(const std::string &msg)
{
    // Отправляем сообщение через сигнал Qt
    emit newLogMessage(QString::fromStdString(msg));
}

std::shared_ptr<ILogger> QtLoggerFactory::create()
{
    return std::make_shared<QtLogger>();
}