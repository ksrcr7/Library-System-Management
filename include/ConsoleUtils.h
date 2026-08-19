#pragma once
#include <vector>
#include <sqlite3.h>
#include "Book.h"
#include "BorrowRecord.h"



void printMenu();
int getCommandInput();
sqlite3_int64 getIdInput();
sqlite3_int64 getValidIdInput(const std::vector<Book>& books);
Book getBookbyId(const std::vector<Book>& books, sqlite3_int64 id);
void editBookData(Book& book);
void printSearchResults(const std::vector<Book>& books);
void printBorrowHistory(const std::vector<BorrowRecord>& history);
void printAllBorrowRecords(const std::vector<BorrowRecord>& records);

