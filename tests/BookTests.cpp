#include <gtest/gtest.h>
#include <chrono>
#include "Book.h"

int getCurrentYear(){
    using namespace std::chrono;
    
    auto now = system_clock::now();
    auto year = year_month_day{floor<days>(now)}.year();

    return static_cast<int>(year);

}


TEST(BOOKTEST,CreateBookWithCorrectData){
    Book book ("Clean Data","Kasra",2025,true);

    EXPECT_EQ(book.getTitle(),"Clean Data");
    EXPECT_EQ(book.getAuthor(),"Kasra");
    EXPECT_EQ(book.getPublishYear(),2025);
    EXPECT_TRUE(book.isAvailable());
}

TEST(BOOKTEST,RejectsEmptyTitle){
    
    EXPECT_THROW(Book book("","Kasra",2026,true),std::invalid_argument);
    EXPECT_THROW(Book book("             ","Kasra",2026,true),std::invalid_argument);
}

TEST(BOOKTEST,RejectsEmptyAuthor){
    EXPECT_THROW(Book book("Clean Data","",2026,true),std::invalid_argument);
    EXPECT_THROW(Book book("Clean Data","          ",2026,true),std::invalid_argument);
}

TEST(BOOKTEST,RejectsFuturePublishyear){
    EXPECT_THROW(Book book("Clean Data","Kasra",2027,true),std::invalid_argument);
}

TEST(BOOKTEST,RejectsPublishyearBelowMinimum){
    EXPECT_THROW(Book book("Clean Data","Kasra",1449,true),std::invalid_argument);
}

TEST(BOOKTEST,AcceptsMinimumPublishyear){
    EXPECT_NO_THROW(Book book("Clean Data","Kasra",1450,true));
}

TEST(BOOKTEST,SaveMinimumPublishyear){
    Book book("Clean Data","Kasra",1450,true);
    EXPECT_EQ(book.getPublishYear(),1450);
}

TEST(BOOKTEST,AcceptsSetTitle){
    Book book("Clean Data","Kasra",1450,true);
    book.setTitle("New title");
    EXPECT_EQ(book.getTitle(),"New title");
}

TEST(BOOKTEST,RejectsSetTitleV1){
    Book book("Clean Data","Kasra",1450,true);
    EXPECT_THROW(book.setTitle(""),std::invalid_argument);
}

TEST(BOOKTEST,RejectsSetTitleV2){
    Book book("Clean Data","Kasra",1450,true);
    EXPECT_THROW(book.setTitle("         "),std::invalid_argument);
}

TEST(BOOKTEST,InvalidTitleDoesNotChangeExistingTitle){
    Book book("Clean Data","Kasra",1450,true);
    EXPECT_THROW(book.setTitle(""),std::invalid_argument);
    EXPECT_EQ(book.getTitle(),"Clean Data");
}

TEST(BOOKTEST,AcceptsSetAuthor){
    Book book("Clean Data","Kasra",1450,true);
    book.setAuthor("Kousha");
    EXPECT_EQ(book.getAuthor(),"Kousha");
}

TEST(BOOKTEST,RejectSetAuthorV1){
    Book book("Clean Data","Kasra",1450,true);
    EXPECT_THROW(book.setAuthor(""),std::invalid_argument);
}

TEST(BOOKTEST,RejectSetAuthorV2){
    Book book("Clean Data","Kasra",1450,true);
    EXPECT_THROW(book.setAuthor("         "),std::invalid_argument);
}

TEST(BOOKTEST,InvalidAuthorDoesNotChangeExistingAuthor){
    Book book("Clean Data","Kasra",1450,true);
    EXPECT_THROW(book.setAuthor(""),std::invalid_argument);
    EXPECT_EQ(book.getAuthor(),"Kasra");
}

TEST(BOOKTEST,AcceptsSetPublishYear){
    Book book("Clean Data","Kasra",1998,true);
    book.setPublishYear(1450);
    EXPECT_EQ(book.getPublishYear(),1450);
}

TEST(BOOKTEST,RejectsSetPublishYear){
    Book book("Clean Data","Kasra",1450,true);
    EXPECT_THROW(book.setPublishYear(1449),std::invalid_argument);
}

TEST(BOOKTEST,RejectsFuturePublishYear){
    Book book("Clean Data","Kasra",1450,true);
    int futureYear = getCurrentYear() + 1;
    EXPECT_THROW(book.setPublishYear(futureYear),std::invalid_argument);
}

TEST(BOOKTEST,InvalidPublishYearDoesNotChangeExistingPublishYear){
    Book book("Clean Data","Kasra",1450,true);
    EXPECT_THROW(book.setPublishYear(1449),std::invalid_argument);
    EXPECT_EQ(book.getPublishYear(),1450);
}

TEST(BOOKTEST,SetAvailableTrue){
    Book book("Clean Data","Kasra",1450,false);
    book.setAvailable(true);
    EXPECT_TRUE(book.isAvailable());
}

TEST(BOOKTEST,SetAvailableFalse){
    Book book("Clean Data","Kasra",1450,true);
    book.setAvailable(false);
    EXPECT_FALSE(book.isAvailable());
}