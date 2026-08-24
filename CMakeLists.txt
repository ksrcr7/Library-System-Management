cmake_minimum_required(VERSION 3.16)

project(LibraryManagementSystem)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include (FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(LibraryManagementSystem
    src/main.cpp
    src/Book.cpp
    src/Database.cpp
    src/StringUtils.cpp
    src/ConsoleUtils.cpp
    include/SQLiteUtils.h
)

target_include_directories(LibraryManagementSystem PRIVATE include)
target_link_directories(LibraryManagementSystem PRIVATE /ucrt64/lib)
target_link_libraries(LibraryManagementSystem PRIVATE sqlite3)

add_executable(
    LibraryManagementTests
    tests/BookTests.cpp
    src/Book.cpp
    src/StringUtils.cpp
)

target_include_directories(
    LibraryManagementTests
    PRIVATE include
)

target_link_libraries (
    LibraryManagementTests
    PRIVATE GTest::gtest_main
)

include (GoogleTest)
gtest_discover_tests(LibraryManagementTests)