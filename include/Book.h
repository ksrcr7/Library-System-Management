#include <string>
#include <sqlite3.h>

class Database;


class Book {
    friend class Database;

    private:
        std::string title;
        std::string author;
        sqlite3_int64 id = 0;
        int publishYear;
        bool available;
        void validate()const;
        void setId(sqlite3_int64 id);

    public:
        Book(const std::string& title,const std::string& author,int publishYear,
            bool available = true);
        
        sqlite3_int64 getId()const;
        const std::string& getTitle()const;
        const std::string& getAuthor()const;
        int getPublishYear()const;
        bool isAvailable()const;  

        void setTitle(const std::string& title);
        void setAuthor(const std::string& author);
        void setPublishYear(int publishYear);
        void setAvailable(bool available);
        
        



};