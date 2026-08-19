#include <Database.h>
#include <stdexcept>
#include <iostream>



Database::Database(const std::string &path) : databasePath(path)
{
    int rc = sqlite3_open(databasePath.c_str(),&db);
    if(rc != SQLITE_OK){
        std::string errormsg = sqlite3_errmsg(db);
        sqlite3_close(db);
        throw std::runtime_error(errormsg);
    }

    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);

    createBookTables();
    createBorrowHistoryTables();
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

void Database::updateBook(Book &book) {
    const char* sql = "UPDATE books SET title = ?, author = ?, publish_year = ?, available = ? WHERE id = ?;";
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

    rc = sqlite3_bind_int64(stmt, 5, book.getId());
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

std::vector<Book> Database::searchBooks(const std::string &keyword) {
    std::vector<Book> searchResults;
    std::string pattern = "%" + keyword + "%";
    const char* sql = "SELECT id, title, author, publish_year, available FROM books WHERE title LIKE ? OR author LIKE ? ORDER BY title;";

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if(rc != SQLITE_OK){
        throwSqliteError(stmt);
    }

    rc = sqlite3_bind_text(stmt, 1, pattern.c_str(), -1, SQLITE_TRANSIENT);
    if(rc != SQLITE_OK){
        throwSqliteError(stmt);
    }

    rc = sqlite3_bind_text(stmt, 2, pattern.c_str(), -1, SQLITE_TRANSIENT);
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
        searchResults.emplace_back(book);
    }

    if(rc != SQLITE_DONE){
        throwSqliteError(stmt);
    }

    sqlite3_finalize(stmt);
    return searchResults;
}

void Database::borrowBook(sqlite3_int64 id, const std::string& borrowerName) {
    
    int rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    if(rc != SQLITE_OK){
        throw std::runtime_error("Failed to begin transaction.");
    }
    
    try{
        const char* sql1 = "UPDATE books SET available = 0 WHERE id = ? AND available = 1;";
        sqlite3_stmt* stmt = nullptr;
        int rc = sqlite3_prepare_v2(db, sql1, -1, &stmt, nullptr);
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

        if(sqlite3_changes(db) == 0){
            sqlite3_finalize(stmt);
            throw std::runtime_error("Book is either not available or does not exist.");
        }
        
        sqlite3_finalize(stmt);
        stmt = nullptr; 

        const char* sql2 = "INSERT INTO borrow_history (book_id, borrower_name) VALUES (?, ?);";
        rc = sqlite3_prepare_v2(db, sql2, -1, &stmt, nullptr);
        if(rc != SQLITE_OK){
            throwSqliteError(stmt);
        }

        rc = sqlite3_bind_int64(stmt, 1, id);
        if(rc != SQLITE_OK){
            throwSqliteError(stmt);
        }

        rc = sqlite3_bind_text(stmt, 2, borrowerName.c_str(), -1, SQLITE_TRANSIENT);
        if(rc != SQLITE_OK){
            throwSqliteError(stmt);
        }

        rc = sqlite3_step(stmt);
        if(rc != SQLITE_DONE){
            throwSqliteError(stmt);
        }

        if (sqlite3_changes(db) == 0) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to create borrow history.");
    }
        sqlite3_finalize(stmt);
        stmt = nullptr;
        rc = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
        if(rc != SQLITE_OK){
            throw std::runtime_error("Failed to commit transaction.");
        }
    }

    catch(const std::exception& e){
        rc = sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        if(rc != SQLITE_OK){
            throw std::runtime_error("Failed to rollback transaction.");
        }
        throw;
    }
    
}

void Database::returnBook(sqlite3_int64 id) {
   
    int rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    if(rc != SQLITE_OK){
        throw std::runtime_error("Failed to begin transaction.");
    }

    try{
        const char* sql1 = "UPDATE books SET available = 1 WHERE id = ? AND available = 0;";
        sqlite3_stmt* stmt = nullptr;

        int rc = sqlite3_prepare_v2(db, sql1, -1, &stmt, nullptr);
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

        if(sqlite3_changes(db) == 0){
            sqlite3_finalize(stmt);
            throw std::runtime_error("Book is either not borrowed or does not exist.");
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;

        const char* sql2 = "UPDATE borrow_history "
                            "SET return_date = CURRENT_TIMESTAMP "
                            "WHERE book_id = ? AND return_date IS NULL;";
        
        rc = sqlite3_prepare_v2(db, sql2, -1, &stmt, nullptr);
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
        
        if(sqlite3_changes(db) == 0){
            sqlite3_finalize(stmt);
            throw std::runtime_error("No borrow history found for this book.");
        }

        sqlite3_finalize(stmt);
        stmt = nullptr;

        rc = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
        if(rc != SQLITE_OK){
            throw std::runtime_error("Failed to commit transaction.");
        }
    }

    catch(const std::exception& e){
        rc = sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        if(rc != SQLITE_OK){
            throw std::runtime_error("Failed to rollback transaction.");
        }
        throw;
    }
   
    
}

std::vector<BorrowRecord> Database::getBorrowedBookHistory(sqlite3_int64 bookId) {
    std::vector<BorrowRecord> result;

    const char* sql = "SELECT bh.id, bh.book_id, b.title, bh.borrower_name, "
                      "bh.borrow_date, bh.return_date "
                      "FROM borrow_history AS bh "
                      "JOIN books AS b ON bh.book_id = b.id "
                      "WHERE bh.book_id = ? "
                      "ORDER BY bh.borrow_date DESC;";

    
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db,sql,-1, &stmt, nullptr); 
    if(rc != SQLITE_OK){
        throwSqliteError(stmt);
    }  
    
    rc = sqlite3_bind_int64(stmt,1,bookId);
    if(rc != SQLITE_OK){
        throwSqliteError(stmt);
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        BorrowRecord record;
        record.id = sqlite3_column_int64(stmt,0);
        record.bookId = sqlite3_column_int64(stmt,1);
        record.bookTitle = reinterpret_cast<const char*> (sqlite3_column_text(stmt,2));
        record.borrowerName = reinterpret_cast<const char*> (sqlite3_column_text(stmt,3));
        record.borrowDate = reinterpret_cast<const char*> (sqlite3_column_text(stmt,4));

        if(sqlite3_column_type(stmt,5) == SQLITE_NULL){
            record.returnDate = std::nullopt;
        }
        else{
            record.returnDate = reinterpret_cast<const char*> (sqlite3_column_text(stmt,5));
        }

        result.emplace_back(record);
    
    }

    if(rc != SQLITE_DONE){
        throwSqliteError(stmt);
    }

    sqlite3_finalize(stmt);
    stmt = nullptr;
    return result;

}

std::vector<BorrowRecord> Database::getAllBorrowRecords(){
    std::vector<BorrowRecord> result;
    
    const char* sql = "SELECT bh.id, bh.book_id, b.title, bh.borrower_name, "
                      "bh.borrow_date, bh.return_date "
                      "FROM borrow_history AS bh "
                      "JOIN books AS b ON bh.book_id = b.id "
                      "ORDER BY bh.borrow_date DESC;";

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if(rc != SQLITE_OK){
        throwSqliteError(stmt);
    }
    
    while((rc = sqlite3_step(stmt)) == SQLITE_ROW){
        BorrowRecord record;
        record.id = sqlite3_column_int64(stmt, 0);
        record.bookId = sqlite3_column_int64(stmt, 1);
        record.bookTitle = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        record.borrowerName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        record.borrowDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

        if(sqlite3_column_type(stmt, 5) == SQLITE_NULL){
            record.returnDate = std::nullopt;
        }
        else{
            record.returnDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        }

        result.emplace_back(record);
    }

    if(rc != SQLITE_DONE){
        throwSqliteError(stmt);
    }

    sqlite3_finalize(stmt);
    stmt = nullptr;
    return result;
    


}

Database::~Database() {
    if(db){
        sqlite3_close(db);
    }
    
}

void Database::createBookTables() {
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

void Database::createBorrowHistoryTables() {
    const char* sql =
                        "CREATE TABLE IF NOT EXISTS borrow_history ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "book_id INTEGER NOT NULL, "
                        "borrower_name TEXT NOT NULL, "
                        "borrow_date TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP, "
                        "return_date TEXT, "
                        "FOREIGN KEY(book_id) REFERENCES books(id) ON DELETE RESTRICT"
                        ");";

    char* errMsg = nullptr;
    int rc  = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if(rc != SQLITE_OK){
        std::string error = errMsg;
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }
    
}
