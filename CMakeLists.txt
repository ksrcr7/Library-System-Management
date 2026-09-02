cmake_minimum_required(VERSION 3.16)

project(LibraryManagementSystem)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_COVERAGE "Enable code coverage" OFF)

include (FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)
enable_testing()

find_package(SQLite3 REQUIRED)


add_library(
    LibraryCore
    src/Book.cpp
    src/Database.cpp
    src/StringUtils.cpp
    src/ConsoleUtils.cpp
)

    

add_executable(
    LibraryManagementSystem
    src/main.cpp
)



target_include_directories(LibraryCore PUBLIC include)
target_link_libraries(LibraryCore PUBLIC SQLite::SQLite3)
target_link_libraries(LibraryManagementSystem PRIVATE LibraryCore)



add_executable(
    LibraryManagementTests
    tests/BookTests.cpp
    tests/DatabaseTests.cpp
)

target_link_libraries (
    LibraryManagementTests
    PRIVATE 
    LibraryCore
    GTest::gtest_main
)

if (ENABLE_COVERAGE)
    target_compile_options (LibraryCore PRIVATE --coverage -O0 -g)
    target_link_options (LibraryCore PRIVATE --coverage)

    target_compile_options(LibraryManagementTests PRIVATE --coverage -O0 -g)
    target_link_options(LibraryManagementTests PRIVATE --coverage)
endif()

include (GoogleTest)
gtest_discover_tests(LibraryManagementTests)