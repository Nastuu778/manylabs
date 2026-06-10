#include "ICounter.h"
#include <fstream>

LineCounter::LineCounter(std::shared_ptr<ILogger> log) : logger(log) {}

CountResult LineCounter::count(const std::string &path)
{
    CountResult res;
    std::ifstream file(path);
    std::string line;
    bool inBlockComment = false;

    if (!file.is_open())
    {
        if (logger)
            logger->log("Error: File not found - " + path);
        return res;
    }

    if (logger)
        logger->log("Processing: " + path);

    while (std::getline(file, line))
    {
        // Исправление #1: Удаляем символ \r для совместимости с Windows (CRLF)
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        res.physical++;

        // === ДОБАВИТЬ ЭТУ ПРОВЕРКУ ===
        // Если строка пустая (была только \r\n), пропускаем её
        if (line.find_first_not_of(" \t\r\n") == std::string::npos)
        {
            continue; // Пропускаем пустую строку
        }
        // =================================

        bool hasCode = false;
        bool isComment = false;

        std::string trimmedLine = line;

        // === ДОБАВИТЬ ПРОВЕРКУ: если строка пустая после удаления \r ===
        if (trimmedLine.empty())
        {
            // Пропускаем пустую строку
            continue;
        }
        // ==================================================================

        // Удаляем ведущие пробелы
        size_t start = trimmedLine.find_first_not_of(" \t");
        if (start != std::string::npos)
        {
            trimmedLine = trimmedLine.substr(start);
        }
        else
        {
            trimmedLine = "";
        }

        // Если внутри блочного комментария
        if (inBlockComment)
        {
            size_t endBlockPos = trimmedLine.find("*/");
            if (endBlockPos != std::string::npos)
            {
                // Найден конец комментария
                inBlockComment = false;

                // Проверяем, есть ли код ПОСЛЕ */
                std::string afterBlock = trimmedLine.substr(endBlockPos + 2);

                // Удаляем строчные комментарии после */
                size_t lineCommentPos = afterBlock.find("//");
                if (lineCommentPos != std::string::npos)
                {
                    afterBlock = afterBlock.substr(0, lineCommentPos);
                }

                // Удаляем пробелы
                size_t firstNonSpace = afterBlock.find_first_not_of(" \t");
                if (firstNonSpace != std::string::npos)
                {
                    size_t lastNonSpace = afterBlock.find_last_not_of(" \t");
                    afterBlock = afterBlock.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1);
                }
                else
                {
                    afterBlock = "";
                }

                if (!afterBlock.empty())
                {
                    hasCode = true;
                }
                else
                {
                    isComment = true;
                }
            }
            else
            {
                // Всё ещё внутри комментария, кода нет
                isComment = true;
            }
        }
        else
        {
            // Проверяем на строковый комментарий
            size_t commentPos = trimmedLine.find("//");
            if (commentPos != std::string::npos)
            {
                // Есть комментарий, проверяем есть ли код до него
                std::string codePart = trimmedLine.substr(0, commentPos);
                codePart.erase(0, codePart.find_first_not_of(" \t"));
                if (!codePart.empty())
                {
                    size_t lastNonSpace = codePart.find_last_not_of(" \t");
                    codePart = codePart.substr(0, lastNonSpace + 1);
                }
                if (!codePart.empty())
                {
                    hasCode = true;
                }
                else
                {
                    isComment = true;
                }
            }
            // Исправление #2: Проверяем на блочный комментарий
            else if (trimmedLine.find("/*") != std::string::npos)
            {
                size_t blockStart = trimmedLine.find("/*");
                std::string codeBefore = trimmedLine.substr(0, blockStart);

                // Проверяем, есть ли код до /*
                codeBefore.erase(0, codeBefore.find_first_not_of(" \t"));
                if (!codeBefore.empty())
                {
                    size_t lastNonSpace = codeBefore.find_last_not_of(" \t");
                    codeBefore = codeBefore.substr(0, lastNonSpace + 1);
                }

                if (!codeBefore.empty())
                {
                    hasCode = true;
                }

                // Проверяем, закрылся ли комментарий в этой же строке
                if (trimmedLine.find("*/") == std::string::npos)
                {
                    inBlockComment = true;
                }

                if (!hasCode)
                {
                    isComment = true;
                }
            }
            // Обычная строка (без комментариев)
            else
            {
                if (!trimmedLine.empty())
                {
                    hasCode = true;
                }
            }
        }

        // Подсчитываем
        if (hasCode)
        {
            res.logical++;
        }
        else if (isComment)
        {
            res.comments++;
        }
    }

    if (logger)
        logger->log("Counting finished.");
    return res;
}

std::shared_ptr<ICounter> LineCounterCreator::create(std::shared_ptr<ILogger> log)
{
    return std::make_shared<LineCounter>(log);
}