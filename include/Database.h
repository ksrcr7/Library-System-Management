#include <sqlite3.h>
#include <string>
#include <vector>
#include "Book.h"

class Database {
    private:
    sqlite3* db = nullptr;
    std::string databasePath;
    void createTables();
    [[noreturn]] void throwSqliteError(sqlite3_stmt* stmt) const;

    public:
    Database(const std::string& path);
    void addBook(Book& book);
    void deleteBook(sqlite3_int64 id);
    std::vector<Book> getAllBooks();
    void updateBook(Book& book);
    std::vector<Book> searchBooks(const std::string& keyword);





    ~Database();


};


