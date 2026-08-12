#include "Database.h"
#include "ConsoleUtils.h"
#include <iostream>


int main(){
    try
    {
        Database DB("database/library.db");
        std::cerr<<"Database opened successfully."<<std::endl;
        

        while(true){
            printMenu();
            int command = getCommandInput();

            switch(command){
                case 1:{
                    std::string title, author;
                    int publishYear;

                    std::cout << "Enter title: ";
                    std::getline(std::cin >> std::ws, title);
                    std::cout << "Enter author: ";
                    std::getline(std::cin >> std::ws, author);
                    std::cout << "Enter publish year: ";
                    std::cin >> publishYear;

                    Book book(title, author, publishYear, true);
                    DB.addBook(book);
                    std::cout << "Book added successfully." << std::endl;
                    break;
                }
                case 2:{
                    std::string keword;
                    std::cerr <<"Enter the title or author: ";
                    std::getline(std::cin >> std::ws, keword);
                    auto searchResults = DB.searchBooks(keword);
                    if(searchResults.empty()){
                        std::cerr << "No books found matching the keyword." << std::endl;
                        break;
                    }
                    printSearchResults(searchResults);
                    sqlite3_int64 id = getValidIdInput(searchResults);
                    DB.deleteBook(id);
                    std::cout << "Book deleted successfully." << std::endl;
                    break;
                }
                case 3:{
                    std::string keyword;
                    std::cerr <<"Enter the title or author: ";
                    std::getline(std::cin >> std::ws, keyword);
                    auto searchResults = DB.searchBooks(keyword);
                    if(searchResults.empty()){
                        std::cerr << "No books found matching the keyword." << std::endl;
                        break;
                    }
                    printSearchResults(searchResults);
                    sqlite3_int64 id = getValidIdInput(searchResults);
                    Book book = getBookbyId(searchResults, id);
                    editBookData(book);
                    DB.updateBook(book);
                    std::cout << "Book updated successfully." << std::endl;
                    break;                  
            }
                case 4:{
                    auto books = DB.getAllBooks();
                    if(books.empty()){
                        std::cerr <<" No books in the database." << std::endl;
                        break;
                    }
                    printSearchResults(books);
                    break;
                }
                case 5:
                    std::cout << "Exiting..." << std::endl;
                    return 0;

                default:
                    std::cerr << "Invalid command." << std::endl;
                    break;    

    }
}

        







    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

};