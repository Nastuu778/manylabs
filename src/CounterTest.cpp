#include "ICounter.h"
#include "ILogger.h"
#include <iostream>
#include <fstream>
#include <memory>

class MockLogger : public ILogger
{
public:
    void log(const std::string &msg) override {}
};

// ТЕСТ 1: CRLF окончания
bool testCRLF()
{
    std::ofstream out("test_crlf.cpp");
    out << "int main() {\r\n";
    out << "\r\n"; // Пустая строка только с \r
    out << "    return 0;\r\n";
    out << "}\r\n";
    out.close();

    auto logger = std::make_shared<MockLogger>();
    LineCounter counter(logger);
    CountResult res = counter.count("test_crlf.cpp");

    std::cout << "--- TEST 1: CRLF окончания ---" << std::endl;
    std::cout << "Физические: " << res.physical << " (Ожидается 4)" << std::endl;
    std::cout << "Логические: " << res.logical << " (Ожидается 3)" << std::endl;
    std::cout << "Комментарии: " << res.comments << " (Ожидается 0)" << std::endl;

    remove("test_crlf.cpp");
    return res.logical == 3;
}

// ТЕСТ 2: Код с комментарием на одной строке
bool testCodeWithComment()
{
    std::ofstream out("test_comment.cpp");
    out << "int a = 5; // комментарий в конце\n";
    out << "int b = 10;\n";
    out.close();

    auto logger = std::make_shared<MockLogger>();
    LineCounter counter(logger);
    CountResult res = counter.count("test_comment.cpp");

    std::cout << "\n--- TEST 2: Код с комментарием ---" << std::endl;
    std::cout << "Физические: " << res.physical << " (Ожидается 2)" << std::endl;
    std::cout << "Логические: " << res.logical << " (Ожидается 2)" << std::endl;
    std::cout << "Комментарии: " << res.comments << " (Ожидается 0)" << std::endl;

    remove("test_comment.cpp");
    return res.logical == 2;
}

int main()
{
    std::cout << "=== ЗАПУСК UNIT-ТЕСТОВ ===" << std::endl
              << std::endl;

    bool pass1 = testCRLF();
    bool pass2 = testCodeWithComment();

    std::cout << "\n=== РЕЗУЛЬТАТЫ ===" << std::endl;
    if (!pass1)
        std::cout << "[ОШИБКА] Тест 1 (CRLF) провален!" << std::endl;
    else
        std::cout << "[ОК] Тест 1 пройден." << std::endl;

    if (!pass2)
        std::cout << "[ОШИБКА] Тест 2 (Код с комментарием) провален!" << std::endl;
    else
        std::cout << "[ОК] Тест 2 пройден." << std::endl;

    return 0;
}