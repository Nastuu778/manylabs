#include "MainWindow.h"
#include "QtLogger.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>

MainWindow::MainWindow(std::shared_ptr<ICounter> counter,
                       std::shared_ptr<ILogger> logger,
                       QWidget *parent)
    : QMainWindow(parent), counter(counter), logger(logger)
{
    setWindowTitle("Счётчик строк кода");
    resize(600, 500);

    setupUI();

    // Подключаем сигнал логгера к слоту отображения логов
    QtLogger *qtLogger = dynamic_cast<QtLogger *>(logger.get());
    if (qtLogger)
    {
        connect(qtLogger, &QtLogger::newLogMessage,
                this, &MainWindow::onLogMessageReceived);
    }

    clearResults();
}

void MainWindow::setupUI()
{
    // Центральный виджет
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // === Группа: Выбор файла ===
    QGroupBox *fileGroup = new QGroupBox("Выбор файла");
    QHBoxLayout *fileLayout = new QHBoxLayout(fileGroup);

    filePathEdit = new QLineEdit();
    filePathEdit->setPlaceholderText("Путь к файлу...");
    filePathEdit->setReadOnly(true);

    browseButton = new QPushButton("Обзор...");
    browseButton->setFixedWidth(100);

    fileLayout->addWidget(filePathEdit);
    fileLayout->addWidget(browseButton);

    // === Кнопка подсчёта ===
    countButton = new QPushButton("Подсчитать строки");
    countButton->setStyleSheet(
        "QPushButton { "
        "  background-color: #4CAF50; "
        "  color: white; "
        "  font-weight: bold; "
        "  padding: 10px; "
        "  border: none; "
        "  border-radius: 5px; "
        "} "
        "QPushButton:hover { background-color: #45a049; } "
        "QPushButton:pressed { background-color: #3d8b40; }");

    // === Группа: Результаты ===
    QGroupBox *resultsGroup = new QGroupBox("Результаты подсчёта");
    QGridLayout *resultsLayout = new QGridLayout(resultsGroup);

    physicalLinesLabel = new QLabel("0");
    logicalLinesLabel = new QLabel("0");
    commentsLabel = new QLabel("0");

    physicalLinesLabel->setStyleSheet("color: green; font-weight: bold; font-size: 14px;");
    logicalLinesLabel->setStyleSheet("color: blue; font-weight: bold; font-size: 14px;");
    commentsLabel->setStyleSheet("color: orange; font-weight: bold; font-size: 14px;");

    physicalLinesLabel->setAlignment(Qt::AlignRight);
    logicalLinesLabel->setAlignment(Qt::AlignRight);
    commentsLabel->setAlignment(Qt::AlignRight);

    resultsLayout->addWidget(new QLabel("Физические строки:"), 0, 0);
    resultsLayout->addWidget(physicalLinesLabel, 0, 1);
    resultsLayout->addWidget(new QLabel("Логические строки:"), 1, 0);
    resultsLayout->addWidget(logicalLinesLabel, 1, 1);
    resultsLayout->addWidget(new QLabel("Комментарии:"), 2, 0);
    resultsLayout->addWidget(commentsLabel, 2, 1);

    // === Группа: Логи ===
    QGroupBox *logGroup = new QGroupBox("Журнал событий");
    QVBoxLayout *logLayout = new QVBoxLayout(logGroup);

    logTextEdit = new QTextEdit();
    logTextEdit->setReadOnly(true);
    logTextEdit->setStyleSheet(
        "QTextEdit { "
        "  background-color: #f5f5f5; "
        "  font-family: 'Courier New', monospace; "
        "  font-size: 11px; "
        "  border: 1px solid #ddd; "
        "}");
    logLayout->addWidget(logTextEdit);

    // === Собираем всё вместе ===
    mainLayout->addWidget(fileGroup);
    mainLayout->addWidget(countButton);
    mainLayout->addWidget(resultsGroup);
    mainLayout->addWidget(logGroup);

    // === Подключаем сигналы кнопок ===
    connect(browseButton, &QPushButton::clicked,
            this, &MainWindow::onBrowseButtonClicked);
    connect(countButton, &QPushButton::clicked,
            this, &MainWindow::onCountButtonClicked);
}

void MainWindow::onBrowseButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Выберите файл",
        QString(),
        "Все файлы (*);;C++ файлы (*.cpp *.h);;Текстовые файлы (*.txt)");

    if (!fileName.isEmpty())
    {
        filePathEdit->setText(fileName);
        if (logger)
        {
            logger->log("Выбран файл: " + fileName.toStdString());
        }
    }
}

void MainWindow::onCountButtonClicked()
{
    QString filePath = filePathEdit->text();

    if (filePath.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, выберите файл!");
        return;
    }

    QFile file(filePath);
    if (!file.exists())
    {
        QMessageBox::critical(this, "Ошибка", "Файл не найден!");
        return;
    }

    if (logger)
    {
        logger->log("Начало подсчёта для файла: " + filePath.toStdString());
    }

    CountResult result = counter->count(filePath.toStdString());
    displayResults(result);

    if (logger)
    {
        logger->log("Подсчёт завершён.");
    }
}

void MainWindow::onLogMessageReceived(const QString &message)
{
    logTextEdit->append("[LOG]: " + message);
}

void MainWindow::displayResults(const CountResult &result)
{
    physicalLinesLabel->setText(QString::number(result.physical));
    logicalLinesLabel->setText(QString::number(result.logical));
    commentsLabel->setText(QString::number(result.comments));
}

void MainWindow::clearResults()
{
    physicalLinesLabel->setText("0");
    logicalLinesLabel->setText("0");
    commentsLabel->setText("0");
    logTextEdit->clear();
}