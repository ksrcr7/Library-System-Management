cmake_minimum_required(VERSION 3.16)

project(LibraryManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(LibraryManagementSystem
    src/main.cpp
    src/Book.cpp
    src/Database.cpp
)

target_include_directories(LibraryManagementSystem PRIVATE include)