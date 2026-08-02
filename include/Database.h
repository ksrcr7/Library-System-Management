#include <sqlite3.h>
#include <string>

class Database {
    private:
    sqlite3* db = nullptr;
    std::string databasePath;

    public:
    Database(const std::string& path);
    bool openDatabase();


};


