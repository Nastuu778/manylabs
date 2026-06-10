cmake_minimum_required(VERSION 3.10)
project(LineCounter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем автоматическую обработку MOC (ОБЯЗАТЕЛЬНО перед add_executable)
set(CMAKE_AUTOMOC ON)

# Поиск Qt5
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# Добавляем папку include в путь поиска заголовков
include_directories(include)

# ============================================
# Консольная версия (оригинальная)
# ============================================
add_executable(LineCounter 
    src/main.cpp 
    src/Counter.cpp 
    src/Loggers.cpp 
    src/ConsoleUI.cpp
)
target_include_directories(LineCounter PRIVATE include)

# ============================================
# Графическая версия (Qt5)
# ============================================
add_executable(LineCounterQt
    src/main_qt.cpp
    src/Counter.cpp
    src/Loggers.cpp
    src/QtLogger.cpp
    src/MainWindow.cpp
    # ВАЖНО: Явно указываем заголовки с Q_OBJECT, чтобы CMake AUTOMOC их обработал
    include/QtLogger.h
    include/MainWindow.h
)
target_link_libraries(LineCounterQt Qt5::Core Qt5::Gui Qt5::Widgets)
target_include_directories(LineCounterQt PRIVATE include)
# Сборка unit-тестов
add_executable(CounterTest
    src/CounterTest.cpp
    src/Counter.cpp
    src/Loggers.cpp
)
target_include_directories(CounterTest PRIVATE include)