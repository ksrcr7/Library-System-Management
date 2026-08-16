#pragma once
#include <string>
#include <optional>
#include <sqlite3.h>

struct BorrowRecord {
    sqlite3_int64 id;
    std::string bookTitle;
    sqlite3_int64 bookId;
    std::string borrowerName;
    std::string borrowDate;
    std::optional<std::string> returnDate;
    
};
