#pragma once
#include "ICounter.h"
#include "ILogger.h"
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <memory>

// Главное окно графического интерфейса (создаётся программно, без .ui файла)
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(std::shared_ptr<ICounter> counter,
                        std::shared_ptr<ILogger> logger,
                        QWidget *parent = nullptr);
    ~MainWindow() = default;

private slots:
    void onBrowseButtonClicked();
    void onCountButtonClicked();
    void onLogMessageReceived(const QString &message);

private:
    // Виджеты (создаются программно в setupUI)
    QLineEdit *filePathEdit;
    QPushButton *browseButton;
    QPushButton *countButton;
    QLabel *physicalLinesLabel;
    QLabel *logicalLinesLabel;
    QLabel *commentsLabel;
    QTextEdit *logTextEdit;

    std::shared_ptr<ICounter> counter;
    std::shared_ptr<ILogger> logger;

    void setupUI();
    void displayResults(const CountResult &result);
    void clearResults();
};