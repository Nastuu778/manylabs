cmake_minimum_required(VERSION 3.10)
project(LineCounter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем папку include в путь поиска заголовков
include_directories(include)

# Собираем все .cpp файлы из папки src
file(GLOB SOURCES "src/*.cpp")

add_executable(LineCounter ${SOURCES})