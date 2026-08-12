#include <iostream>
#include "ConsoleUtils.h"
#include <algorithm>
#include <iomanip>

void printMenu(){
    std::cout << "1. Add a book" << std::endl;
    std::cout << "2. Delete a book" << std::endl;
    std::cout << "3. Update a book" << std::endl;
    std::cout << "4. List all books" << std::endl;
    std::cout << "5. Borrow a book" << std::endl;
    std::cout << "6. Return a book" << std::endl;
    std::cout << "7. Exit" << std::endl;
}

int getCommandInput(){
    int command;
    while(true){
        std::cout << "Enter a command: ";
        if(!(std::cin >> command)){
            std::cerr << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if(command < 1 || command > 7){
            std::cerr << "Invalid command. Please enter a number between 1 and 7." << std::endl;
            continue;
        }
        break;
    }
    return command;
}

sqlite3_int64 getIdInput(){
    sqlite3_int64 id;
    while(true){
        std::cout << "Enter the ID of the book: ";
        if(!(std::cin >> id)){
            std::cerr << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        break;
    }
    return id;
}

sqlite3_int64 getValidIdInput(const std::vector<Book>& books){
    while(true){
        sqlite3_int64 id = getIdInput();
        auto it = std::find_if(books.begin(), books.end(), [id](const Book& book){
            return book.getId() == id;
        });

        if(it != books.end()){
            return id;
        } else {
            std::cerr << "Invalid ID. Please choose one of the IDs shown above." << std::endl;
        }
    }
    
}

Book getBookbyId(const std::vector<Book>& books, sqlite3_int64 id){
    auto it = std::find_if(books.begin(), books.end(), [id](const Book& book){
        return book.getId() == id;
    });

    if(it != books.end()){
        return *it;
    } 

    throw std::runtime_error("Book with the given ID not found.");
    
}

void editBookData(Book& book){
    std::string title, author;
    int publishYear;
    bool available;

    std::cout << "Enter new title (current: " << book.getTitle() << "): ";
    std::getline(std::cin >> std::ws, title);
    if(!title.empty()){
        try {
            book.setTitle(title);
        } catch(const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    std::cout << "Enter new author (current: " << book.getAuthor() << "): ";
    std::getline(std::cin >> std::ws, author);
    if(!author.empty()){
        try {
            book.setAuthor(author);
        } catch(const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    while(true){
        std::cout << "Enter new publish year (current: " << book.getPublishYear() << "): ";
        if (!(std::cin >> publishYear)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Please enter a valid number.\n";
            continue;
        }
        try {
            book.setPublishYear(publishYear);
            break;
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    
    }

    while(true){
        std::cout << "Is the book available? (1 = yes, 0 = no) (current: "
                  << (book.isAvailable() ? "yes" : "no") << "): ";

        if (!(std::cin >> available)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Please enter 0 or 1.\n";
            continue;
        }
        if (available != 0 && available != 1) {
            std::cerr << "Please enter 0 or 1.\n";
            continue;
        }

        try {
            book.setAvailable(static_cast<bool>(available));
            break;
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    
}

void printSearchResults(const std::vector<Book>& books){
    std::cout << std::left << std::setw(5) << "ID" 
              << std::setw(30) << "Title" 
              << std::setw(30) << "Author" 
              << std::setw(15) << "Publish Year" 
              << std::setw(10) << "Available" 
              << std::endl;

    for(const auto& book : books){
        std::cout << std::left << std::setw(5) << book.getId() 
                  << std::setw(30) << book.getTitle() 
                  << std::setw(30) << book.getAuthor() 
                  << std::setw(15) << book.getPublishYear() 
                  << std::setw(10) << (book.isAvailable() ? "Yes" : "No") 
                  << std::endl;
    }
}