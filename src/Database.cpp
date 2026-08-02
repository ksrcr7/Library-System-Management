#include <Database.h>
#include <stdexcept>

Database::Database(const std::string &path) : databasePath(path) {
    int rc = sqlite3_open(databasePath.c_str(),&db);
    if(rc != SQLITE_OK){
        std::string errormsg = sqlite3_errmsg(db);
        sqlite3_close(db);
        throw std::runtime_error(errormsg);
    }
}

Database::~Database() {
    if(db){
        sqlite3_close(db);
    }
    
}
