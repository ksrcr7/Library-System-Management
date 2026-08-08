#include <Database.h>
#include <stdexcept>



Database::Database(const std::string &path) : databasePath(path)
{
    int rc = sqlite3_open(databasePath.c_str(),&db);
    if(rc != SQLITE_OK){
        std::string errormsg = sqlite3_errmsg(db);
        sqlite3_close(db);
        throw std::runtime_error(errormsg);
    }
}

void Database::throwSqliteError(sqlite3_stmt *stmt) const {
    
    sqlite3_finalize(stmt);
    throw std::runtime_error(sqlite3_errmsg(db));
}

void Database::addBook(Book &book) {
    const char* sql = "INSERT INTO books (title, author, publish_year, available) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if(rc != SQLITE_OK){
        throwSqliteError(stmt);
    }
    

    rc = sqlite3_bind_text(stmt, 1, book.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    if(rc != SQLITE_OK){
        throwSqliteError(stmt);
    }
    

    rc = sqlite3_bind_text(stmt, 2, book.getAuthor().c_str(), -1, SQLITE_TRANSIENT);
    if(rc != SQLITE_OK){
        throwSqliteError(stmt);
    }
    

    rc = sqlite3_bind_int(stmt, 3, book.getPublishYear());
    if(rc != SQLITE_OK){
        throwSqliteError(stmt);
    }
    

    rc = sqlite3_bind_int(stmt, 4, book.isAvailable());
    if(rc != SQLITE_OK){
        throwSqliteError(stmt);
    }

    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE){
        throwSqliteError(stmt);
    }

    sqlite3_int64 lastId = sqlite3_last_insert_rowid(db);
    book.setId(lastId);

    sqlite3_finalize(stmt);

}

void Database::deleteBook(sqlite3_int64 id) {
    const char* sql = "DELETE FROM books WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if(rc != SQLITE_OK){
        throwSqliteError(stmt);
    }

    rc = sqlite3_bind_int64(stmt, 1, id);
    if(rc != SQLITE_OK){
        throwSqliteError(stmt);
    }

    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE){
        throwSqliteError(stmt);
    }

    auto changes = sqlite3_changes(db);
    if(changes == 0){
        sqlite3_finalize(stmt);
        throw std::runtime_error("No book found with the given ID.");
    }

    sqlite3_finalize(stmt);
}

std::vector<Book> Database::getAllBooks(){
    std::vector<Book> books;
    const char* sql = "SELECT id, title, author, publish_year, available FROM books;";
    sqlite3_stmt* stmt = nullptr;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if(rc != SQLITE_OK){
        throwSqliteError(stmt);
    }

    while((rc = sqlite3_step(stmt)) == SQLITE_ROW){
        sqlite3_int64 id = sqlite3_column_int64(stmt, 0);
        std::string title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int publishYear = sqlite3_column_int(stmt, 3);
        bool available = sqlite3_column_int(stmt, 4) != 0;

        Book book(title, author, publishYear, available);
        book.setId(id);
        books.emplace_back(book);
    }

    if(rc != SQLITE_DONE){
        throwSqliteError(stmt);
    }

    sqlite3_finalize(stmt);
    return books;


}

Database::~Database() {
    if(db){
        sqlite3_close(db);
    }
    
}

void Database::createTables() {
    const char* sql = "CREATE TABLE IF NOT EXISTS books ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "title TEXT NOT NULL, "
                      "author TEXT NOT NULL, "
                      "publish_year INTEGER NOT NULL, "
                      "available INTEGER NOT NULL DEFAULT 1);";

    
    char* errMsg = nullptr;
    
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if(rc != SQLITE_OK){
        std::string error = errMsg;
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }

}


