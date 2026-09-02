# Library Management System
A C++20 Library management application backed by SQLite, designed to demonstrate database integration, resource-safe C++ programming, automated testing, and continuous integration.
The application supports book management, borrowing and returning workflows, persistant borrowing history and soft deletion while preserving historical records.


## Features

- Add, update, search, and list books
- Soft-delete books while preserving borrowing history
- Prevent deletion of books that are currently borrowed
- Borrow and return books using transactional database operations
- Track complete borrowing history for each book
- View all borrowing records across the library
- Prevent invalid operations such as double borrowing or double returning
- Validate book data including title, author, and publication year
- Use prepared SQL statements and parameter binding for database operations
- Enforce relational integrity using SQLite foreign keys


## Tech Stack

- **Language:** C++20
- **Database:** SQLite
- **Build System:** CMake
- **Testing:** GoogleTest / CTest
- **CI:** GitHub Actions
- **Code Coverage:** GCC (gcov) + lcov
- **Version Control:** Git


## Architecture

The project separates the core application logic from the executable and test targets.

```text
                    LibraryCore
                  /      |       \
              Book    Database   Utilities
                \         |         /
                 \        |        /
                  ┌───────┴───────┐
                  │               │
        LibraryManagementSystem   LibraryManagementTests

```        



LibraryCore contains the reusable application logic and is linked by both the main application and the automated test executable.
This structure avoids compiling and maintaining duplicated source lists across different targets and keeps the business and persistence logic independent from the program entry point.    



## Database Design

The application uses two main SQLite tables:

### `books`

Stores the current state of each book:

- `id` — Primary key
- `title` — Book title
- `author` — Book author
- `publish_year` — Publication year
- `available` — Indicates whether the book can currently be borrowed
- `active` — Controls soft deletion

### `borrow_history`

Stores borrowing transactions:

- `id` — Primary key
- `book_id` — References the associated book
- `borrower_name` — Name of the borrower
- `borrow_date` — Borrowing timestamp
- `return_date` — Return timestamp; `NULL` while the book is still borrowed

`borrow_history.book_id` references `books.id` using a foreign key.

Books are soft-deleted by setting `active = 0` rather than physically deleting the row. A book cannot be deleted while it is borrowed, but it can be deleted after being returned. This preserves its borrowing history while removing it from the active collection.



## Testing

The project includes automated unit and integration tests using GoogleTest and CTest.

Tests cover both normal workflows and important edge cases, including:

- Book creation and validation
- Title, author, and publication year updates
- Adding, updating, searching, and soft-deleting books
- Preventing deletion of currently borrowed books
- Borrowing and returning books
- Preventing double borrowing and double returning
- Handling non-existent book IDs
- Preserving borrowing history after soft deletion
- Retrieving history for individual books
- Retrieving borrowing records across multiple books
- Verifying database state after failed operations

Database tests use an SQLite in-memory database:

```cpp
Database db(":memory:");
```

This keeps tests isolated from the application's persistent database and allows each test fixture to start with a clean database.




## Continuous Integration

GitHub Actions automatically validates the project on every push and pull request to `main`.

The CI pipeline runs on Ubuntu and performs the following steps:

1. Checks out the repository
2. Installs the required build dependencies
3. Configures the project with CMake and Ninja
4. Builds the application and test targets
5. Runs the complete automated test suite with CTest
6. Generates a code coverage report

This verifies that the project can be built and tested successfully in a clean Linux environment independently of the local development environment.



## Code Coverage

Code coverage is collected in CI using GCC coverage instrumentation and `lcov`.

Current coverage:

- **Line coverage:** 82.9% (306 / 369 lines)
- **Function coverage:** 97.0% (32 / 33 functions)

Coverage reporting focuses on the project's source files under `src/`.




## Build and Run

### Prerequisites

The following tools are required:

- C++20-compatible compiler
- CMake 3.16 or newer
- SQLite3 development library
- Git

Ninja is recommended as the build system.

### Clone the Repository

```bash
git clone <https://github.com/ksrcr7/Library-System-Management.git>
cd Library-System-Management
```

### Configure

Using Ninja:

```bash
cmake -S . -B build -G Ninja
```

Or using the default CMake generator:

```bash
cmake -S . -B build
```

### Build

```bash
cmake --build build
```

### Run Tests

```bash
ctest --test-dir build --output-on-failure
```

### Run the Application

Run the generated `LibraryManagementSystem` executable from the build directory.

On Linux:

```bash
./build/LibraryManagementSystem
```

On Windows, depending on the selected CMake generator:

```powershell
.\build\LibraryManagementSystem.exe
```




## Project Structure

```text
.
├── .github/
│   └── workflows/
│       └── ci.yml
├── database/
├── include/
│   ├── Book.h
│   ├── BorrowRecord.h
│   ├── ConsoleUtils.h
│   ├── Database.h
│   ├── SQLiteUtils.h
│   └── StringUtils.h
├── src/
│   ├── Book.cpp
│   ├── ConsoleUtils.cpp
│   ├── Database.cpp
│   ├── main.cpp
│   └── StringUtils.cpp
├── tests/
│   ├── BookTests.cpp
│   └── DatabaseTests.cpp
├── .gitignore
├── CMakeLists.txt
└── README.md
```



## Design Highlights

- RAII-based management of SQLite prepared statements
- Parameterized SQL queries using prepared statements and bound parameters
- Transactional borrow and return operations with rollback on failure
- Soft deletion to preserve historical borrowing data
- Foreign key enforcement through SQLite
- Separation of reusable core logic from executable and test targets
- Isolated in-memory databases for integration testing
- Automated cross-platform validation through GitHub Actions