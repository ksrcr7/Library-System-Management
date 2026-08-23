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
    validateTitle(title);
    validateAuthor(author);
    validatePublishyear(publishYear);

}

void Book::validateTitle(const std::string &Temptitle) const {
    if(Temptitle.empty()){
        throw std::invalid_argument("Title is empty.");
    }
}

void Book::validateAuthor(const std::string &Tempauthor) const {
    if(Tempauthor.empty()){
        throw std::invalid_argument("Author is empty.");
    }
}

void Book::validatePublishyear(int TempPublishYear) const {
    if(TempPublishYear > currentYear() || TempPublishYear < MIN_YEAR){
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

void Book::setTitle(const std::string &Newtitle) {
    std::string trimmedTitle = trim(Newtitle);
    validateTitle(trimmedTitle);
    title = trimmedTitle;
}

void Book::setAuthor(const std::string &Newauthor) {
    std::string trimmedAuthor = trim(Newauthor);
    validateAuthor(trimmedAuthor);
    author = trimmedAuthor;
}

void Book::setPublishYear(int NewpublishYear) {
    validatePublishyear(NewpublishYear);
    publishYear = NewpublishYear;
}

void Book::setAvailable(bool Newavailable) {
    this->available = Newavailable;
}

void Book::setId(sqlite3_int64 id)
{
    if(id <= 0){
        throw std::invalid_argument("Invalid Id.");
    }

    this->id = id;
}
