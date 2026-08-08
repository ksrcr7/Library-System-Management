#include <sqlite3.h>
#include <string>
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





    ~Database();


};


