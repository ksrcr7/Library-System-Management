#include <gtest/gtest.h>
#include "Database.h"
#include "Book.h"

#include <algorithm>

class DatabaseTests : public ::testing::Test {
    protected:
        Database db;
        DatabaseTests() : db(":memory:") {}
};


TEST_F(DatabaseTests,AddBookSuccessfully) {
    Book book("Clean code","Kasra",2020,true);
    db.addBook(book);
    EXPECT_GT(book.getId(),0);

    auto books = db.getAllBooks();
    ASSERT_EQ (books.size(),1);
    EXPECT_EQ(books[0].getId(),book.getId());
    EXPECT_EQ(books[0].getTitle(),book.getTitle());
    EXPECT_EQ(books[0].getAuthor(),book.getAuthor());
    EXPECT_EQ(books[0].getPublishYear(),book.getPublishYear());
    EXPECT_TRUE(books[0].isAvailable());
}

TEST_F(DatabaseTests,SearchBookSuccessfully) {
    Book book1 ("Clean code","Kasra",2020,true);
    Book book2 ("Clean data","Kousha",2021,true);
    Book book3 ("algorithm","Kamran",2022,true);

    db.addBook(book1);
    db.addBook(book2);
    db.addBook(book3);

    auto result = db.searchBooks("cl");
    ASSERT_EQ(result.size(),2);
    EXPECT_EQ(result[0].getId(),book1.getId());
    EXPECT_EQ(result[1].getId(),book2.getId());
    EXPECT_EQ(result[0].getTitle(),book1.getTitle());
    EXPECT_EQ(result[1].getTitle(),book2.getTitle());



}

TEST_F(DatabaseTests,SearchBooksReturnEmtyWhenNoMatch){
    Book book1 ("Clean code","Kasra",2020,true);
    Book book2 ("Clean data","Kousha",2021,true);
    Book book3 ("algorithm","Kamran",2022,true);

    db.addBook(book1);
    db.addBook(book2);
    db.addBook(book3);

    auto result = db.searchBooks("python");
    EXPECT_TRUE(result.empty());

}

TEST_F(DatabaseTests,UpdateBookSuccessfully){
    Book book ("Clean code","Kasra",2020,true);
    db.addBook(book);

    book.setTitle("algorithm");
    book.setAuthor("Kousha");
    book.setPublishYear(2022);

    db.updateBook(book);

    auto result = db.getAllBooks();
    ASSERT_EQ(result.size(),1);

    EXPECT_EQ(result[0].getId(),book.getId());
    EXPECT_EQ(result[0].getTitle(),"algorithm");
    EXPECT_EQ(result[0].getAuthor(),"Kousha");
    EXPECT_EQ(result[0].getPublishYear(),2022);
    EXPECT_TRUE(result[0].isAvailable());
}

TEST_F(DatabaseTests,CannotUpdateDeletedBook) {
    Book book ("Clean code","Kasra",2020,true);
    db.addBook(book);
    db.deleteBook(book.getId());

    book.setTitle("python");
    EXPECT_THROW(db.updateBook(book),std::runtime_error);
    EXPECT_TRUE(db.getAllBooks().empty());
}

TEST_F(DatabaseTests,DeleteBookSuccessfully) {
    Book book ("Clean code","Kasra",2020,true);
    db.addBook(book);
    db.deleteBook(book.getId());

    auto result1 = db.getAllBooks();
    ASSERT_EQ(result1.size(),0);

    auto result2 = db.searchBooks("Clean code");
    ASSERT_EQ(result2.size(),0);


}

TEST_F(DatabaseTests,BorrowedBookCannotBeDeleted){
    Book book ("Clean code","Kasra",2020,true);
    db.addBook(book);
    db.borrowBook(book.getId(),"Kamran");

    EXPECT_THROW(db.deleteBook(book.getId()),std::runtime_error);

    auto result = db.getAllBooks();
    ASSERT_EQ(result.size(),1);
    EXPECT_EQ(result[0].getId(),book.getId());
    EXPECT_FALSE(result[0].isAvailable());

}

TEST_F(DatabaseTests,ReturnedBookCanBeDeletedWhileHistoryIsPreserved) {
    Book book ("Clean code","Kasra",2020,true);
    db.addBook(book);
    db.borrowBook(book.getId(),"Kamran");
    db.returnBook(book.getId());
    db.deleteBook(book.getId());

    auto books = db.getAllBooks();
    ASSERT_EQ(books.size(),0);

    auto history = db.getBorrowedBookHistory(book.getId());
    ASSERT_EQ(history.size(),1);
    EXPECT_EQ(history[0].bookId,book.getId());
    EXPECT_EQ(history[0].borrowerName, "Kamran");
    EXPECT_TRUE(history[0].returnDate.has_value());
    
}

TEST_F(DatabaseTests,BorrowBookSuccessfully){
    Book book ("Clean code","Kasra",2020,true);
    db.addBook(book);
    db.borrowBook(book.getId(),"Kamran");

    auto books = db.getAllBooks();
    ASSERT_EQ(books.size(),1);
    EXPECT_FALSE(books[0].isAvailable());

    auto result = db.getBorrowedBookHistory(book.getId());
    ASSERT_EQ(result.size(),1);
    EXPECT_EQ(result[0].borrowerName,"Kamran");
    EXPECT_FALSE(result[0].returnDate.has_value());
}

TEST_F(DatabaseTests,CannotBorrowAlreadyBorrowedBook){
    Book book ("Clean code","Kasra",2020,true);
    db.addBook(book);
    db.borrowBook(book.getId(),"Kamran");

    EXPECT_THROW(db.borrowBook(book.getId(),"Ali"),std::runtime_error);

    auto books = db.getAllBooks();
    ASSERT_EQ(books.size(),1);
    EXPECT_FALSE(books[0].isAvailable());

    auto result = db.getBorrowedBookHistory(book.getId());
    ASSERT_EQ(result.size(),1);
    EXPECT_EQ(result[0].borrowerName,"Kamran");
}

TEST_F(DatabaseTests,ReturnBookSuccessfully){
    Book book ("Clean code","Kasra",2020,true);
    db.addBook(book);
    db.borrowBook(book.getId(),"Kamran");
    db.returnBook(book.getId());

    auto books = db.getAllBooks();
    ASSERT_EQ(books.size(),1);
    EXPECT_TRUE(books[0].isAvailable());
    
    auto result = db.getBorrowedBookHistory(book.getId());
    ASSERT_EQ(result.size(),1);
    EXPECT_TRUE(result[0].returnDate.has_value());
}

TEST_F(DatabaseTests,CannotReturnAlreadyReturnedBook){
    Book book ("Clean code","Kasra",2020,true);
    db.addBook(book);
    db.borrowBook(book.getId(),"Kamran");
    db.returnBook(book.getId());

    EXPECT_THROW(db.returnBook(book.getId()),std::runtime_error);

    auto books = db.getAllBooks();
    ASSERT_EQ(books.size(), 1);
    EXPECT_TRUE(books[0].isAvailable());

    auto history = db.getBorrowedBookHistory(book.getId());
    ASSERT_EQ(history.size(),1);
    EXPECT_TRUE(history[0].returnDate.has_value());
}

TEST_F(DatabaseTests,BorrowedHistorySuccessfully) {
    Book book1 ("Clean code","Kasra",2020,true);
    db.addBook(book1);
    db.borrowBook(book1.getId(),"Ali");
    db.returnBook(book1.getId());

    db.borrowBook(book1.getId(),"Sarah");
    db.returnBook(book1.getId());

    Book book2 ("Algorithm","Kousha",2021,true);
    db.addBook(book2);
    db.borrowBook(book2.getId(),"Hasan");

    auto history = db.getBorrowedBookHistory(book1.getId());
    ASSERT_EQ(history.size(),2);
    EXPECT_EQ(history[0].bookId,book1.getId());
    EXPECT_EQ(history[1].bookId,book1.getId());
    
    bool hasAli;
    bool hasSarah;
    for(const auto& it : history){
        if(it.borrowerName == "Ali")
            hasAli = true;
        if(it.borrowerName == "Sarah")
            hasSarah = true;    
    }

    EXPECT_TRUE(hasAli);
    EXPECT_TRUE(hasSarah);
}

TEST_F(DatabaseTests,GetAllBorrowRecordsSuccessfully) {
    Book book1 ("Clean code","Kasra",2020,true);
    db.addBook(book1);
    db.borrowBook(book1.getId(),"Ali");
    db.returnBook(book1.getId());

    db.borrowBook(book1.getId(),"Sarah");
    db.returnBook(book1.getId());

    Book book2 ("Algorithm","Kousha",2021,true);
    db.addBook(book2);
    db.borrowBook(book2.getId(),"Hasan");

    auto history = db.getAllBorrowRecords();
    ASSERT_EQ(history.size(),3);

    auto book1Count = std::count_if(history.begin(),history.end(),[&](const BorrowRecord& record){
        return record.bookId == book1.getId();
    });
    ASSERT_EQ(book1Count, 2);

    auto book2Count = std::count_if(history.begin(),history.end(),[&](const BorrowRecord& record){
        return record.bookId == book2.getId();
    });
    ASSERT_EQ(book2Count, 1);

    bool hasAli = std::any_of(history.begin(),history.end(),[&](const BorrowRecord& record){
        return record.borrowerName == "Ali";
    });
    bool hasSarah = std::any_of(history.begin(),history.end(),[&](const BorrowRecord& record){
        return record.borrowerName == "Sarah";
    });
    bool hasHasan = std::any_of(history.begin(),history.end(),[&](const BorrowRecord& record){
        return record.borrowerName == "Hasan";
    });

    EXPECT_TRUE(hasAli);
    EXPECT_TRUE(hasSarah);
    EXPECT_TRUE(hasHasan);
}

TEST_F(DatabaseTests,GetBorrowedBookHistoryReturnsEmptyForNonExistentBook) {
        
    auto history = db.getBorrowedBookHistory(999);
    EXPECT_TRUE(history.empty());
}

TEST_F(DatabaseTests,CannotBorrowNonExistentBook) {
    EXPECT_THROW(db.borrowBook(999,"Ali"),std::runtime_error);

    auto history = db.getAllBorrowRecords();
    EXPECT_TRUE(history.empty());
}

TEST_F(DatabaseTests,CannotReturnNonExistentBook) {
    EXPECT_THROW(db.returnBook(999),std::runtime_error);

    auto history = db.getAllBorrowRecords();
    EXPECT_TRUE(history.empty());
}

TEST_F(DatabaseTests,CannotDeleteNonExistentBook) {
    EXPECT_THROW(db.deleteBook(999),std::runtime_error);

    auto books = db.getAllBooks();
    EXPECT_TRUE(books.empty());
}

TEST_F(DatabaseTests,CannotDeleteAlreadyDeletedBook) {
    Book book ("Clean code","Kasra",2020,true);
    db.addBook(book);
    db.deleteBook(book.getId());

    EXPECT_THROW(db.deleteBook(book.getId()),std::runtime_error);

    auto books = db.getAllBooks();
    EXPECT_TRUE(books.empty());
}
