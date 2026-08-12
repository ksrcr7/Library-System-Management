cmake_minimum_required(VERSION 3.16)

project(LibraryManagementSystem)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(LibraryManagementSystem
    src/main.cpp
    src/Book.cpp
    src/Database.cpp
    src/StringUtils.cpp
    src/ConsoleUtils.cpp
)

target_include_directories(LibraryManagementSystem PRIVATE include)
target_link_directories(LibraryManagementSystem PRIVATE /ucrt64/lib)
target_link_libraries(LibraryManagementSystem PRIVATE sqlite3)