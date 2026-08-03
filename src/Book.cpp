#include "Book.h"
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include "StringUtils.h"

constexpr int MAX_YEAR = 2026;
constexpr int MIN_YEAR = 1450;

void Book::validate() const {
    if(title.empty()){
        throw std::invalid_argument("Title is empty.");
    }
    if(author.empty()){
        throw std::invalid_argument("Author is empty.");
    }
    if(id <= 0){
        throw std::invalid_argument("Invalid Id.");
    }
    if(publishYear > MAX_YEAR || publishYear < MIN_YEAR){
        throw std::invalid_argument("Invalid publish year.");
    }

}



Book::Book(int id, const std::string &title, const std::string &author, int publishYear, bool available) :
id(id),title(trim(title)),author(trim(author)),publishYear(publishYear),available(available) {

    validate();
    
}
