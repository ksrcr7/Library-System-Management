#include "Book.h"
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <chrono>
#include "StringUtils.h"

constexpr int MIN_YEAR = 1450;

static int currentYear(){
    using namespace std::chrono;
    auto now = system_clock::now();
    auto year = year_month_day{floor<days>(now)}.year();

    return int(year);
}

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
    if(publishYear > currentYear() || publishYear < MIN_YEAR){
        throw std::invalid_argument("Invalid publish year.");
    }

}



Book::Book( const std::string &title, const std::string &author, int publishYear, bool available) :
title(trim(title)),author(trim(author)),publishYear(publishYear),available(available) {

    validate();
    
}

sqlite3_int64 Book::getId() const
{
    return id;
}

const std::string &Book::getTitle() const
{
    return title;
}

const std::string &Book::getAuthor() const
{
    return author;
}

int Book::getPublishYear() const
{
    return publishYear;
}

bool Book::isAvailable() const
{
    return available;
}

void Book::setId(sqlite3_int64 id)
{
    if(id <= 0){
        throw std::invalid_argument("Invalid Id.");
    }
    
    if(this->id != 0){
        throw std::logic_error("Id is already set.");
    }

    this->id = id;
}
