#include <sqlite3.h>
#include <string>
#include <vector>
#include "Book.h"
#include "BorrowRecord.h"

class Database {
    private:
    sqlite3* db = nullptr;
    std::string databasePath;
    void createBookTables();
    void createBorrowHistoryTables();
    [[noreturn]] void throwSqliteError(sqlite3_stmt* stmt) const;

    public:
    Database(const std::string& path);
    void addBook(Book& book);
    void deleteBook(sqlite3_int64 id);
    std::vector<Book> getAllBooks();
    void updateBook(Book& book);
    std::vector<Book> searchBooks(const std::string& keyword);
    void borrowBook(sqlite3_int64 id, const std::string& borrowerName);
    void returnBook(sqlite3_int64 id);
    std::vector<BorrowRecord> getBorrowedBookHistory(sqlite3_int64 bookId);
    std::vector<BorrowRecord> getAllBorrowRecords();





    ~Database();


};


