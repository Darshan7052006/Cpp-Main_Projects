# SLIMS SQLite Learning Notes

Running revision notes for the SQLite implementation in the SLIMS C++ project. These will be expanded as we learn and later formatted as a revision PDF.

## C++ and SQLite connection basics

- SQLite is a C library, so C++ code includes `sqlite3.h` and calls its C API.
- Including `sqlite3.h` supplies declarations (so the compiler knows function names/signatures), but the linker also needs SQLite's implementation from `sqlite3.c` (compiled to an object file) or a SQLite library.
- With MinGW, a reliable two-step build is `gcc -c sqlite-amalgamation-3530400/sqlite3.c -o sqlite3.o`, then `g++ SLIRMS.cpp sqlite3.o -o SLIRMS.exe` from the project directory.
- `sqlite3` is SQLite's connection-handle type. `sqlite3* db` and `sqlite3 *db` both declare a pointer named `db`; whitespace around `*` does not change meaning.
- `sqlite3* db = nullptr` starts with no connection handle.
- `sqlite3_open(filename, &db)` receives the address of the pointer (`sqlite3**`) so the function can store the opened connection handle in `db`.
- `sqlite3_errmsg(db)` asks for the latest error associated with that particular connection.
- `std::cout` is the conventional stream for normal output; `std::cerr` is for diagnostics/errors. They can be redirected separately, and `cerr` is typically unbuffered.
- A database-owning class should close a successfully opened connection when the object is destroyed.

## Loan dates and SQLite storage

- SLIMS stores loan dates in C++ as `time_t` values, populated using `time(nullptr)`.
- SQLite has no dedicated `time_t` type. Unix timestamps can be stored as `INTEGER` values (seconds since 1970-01-01 UTC).
- `issue_date` and `due_date` should be `INTEGER NOT NULL`; `return_date` can be `INTEGER` and remain `NULL` until return.
- Loading a row converts the stored integer back to `time_t`.

## Current issue in the `Loan` constructor

- The constructor accepts `issuedate`, `duedate`, `returndate`, and `status`, but the body ignores those parameters and assigns new values (`time(nullptr)`, calculated due date, zero return date, and `Active`).
- Therefore, it always creates a new active loan, even if passed historical values read from a database. That would lose the original dates/status when restoring loans.
- A persistence-friendly constructor should initialize members from the supplied values. A separate convenience constructor/factory can create a fresh loan using today's time and a 14-day due date.

## SQL schema concepts (to expand during lesson)

- A table stores rows (records) with named columns (fields).
- `PRIMARY KEY` uniquely identifies a row.
- `NOT NULL` requires a value; `UNIQUE` prevents duplicates.
- `FOREIGN KEY` links a row to a row in another table, e.g. a loan's `user_id` references `users(user_id)`.
- SQLite foreign-key checking is enabled per connection with `PRAGMA foreign_keys = ON;`; declaring the foreign keys alone does not guarantee they are enforced.
- `CREATE TABLE IF NOT EXISTS` allows startup to create missing tables without dropping existing tables or their data.
- `INTEGER` is a suitable SQLite storage class for Unix timestamp values.
- `sqlite3_exec(connection, sql, callback, callbackData, errorPointer)` asks SQLite to interpret and execute the SQL text against that open connection. For table creation no rows are returned, so callback and callback data can be `nullptr`.
- For inserting C++ values, use prepared statements: SQL contains `?` placeholders, then `sqlite3_bind_*` supplies values separately. This handles quoting safely and avoids building SQL by concatenating user data.
- Prepared-statement flow: `sqlite3_prepare_v2` compiles the SQL, bind each placeholder, call `sqlite3_step` (`SQLITE_DONE` means an insert completed), then call `sqlite3_finalize` to release the statement.
- SQLite placeholder indices start at 1. Bind text with `sqlite3_bind_text` and numbers with `sqlite3_bind_int`/`sqlite3_bind_int64`. `SQLITE_TRANSIENT` tells SQLite to copy a text value before the call returns.
- A successful `INSERT` has `sqlite3_step` return `SQLITE_DONE`; a primary-key or other constraint violation is an error (for example, inserting an ID that already exists).

## Inserting one book with a prepared statement

Example SQL:

```sql
INSERT INTO books
    (book_id, isbn, title, author, category, publication_year, status)
VALUES (?, ?, ?, ?, ?, ?, ?);
```

- `INSERT INTO books` names the target table.
- The parenthesized column list says which columns receive values and defines their order.
- Each `?` is a placeholder for a value. There are seven columns and seven placeholders, in corresponding order.
- `const char* sql` points to the SQL instruction text; it does not point to a C++ `Book` or to the database table.
- `sqlite3_stmt* statement` is SQLite's prepared-statement handle, distinct from the database connection handle (`sqlite3* db`).
- `sqlite3_prepare_v2(db, sql, -1, &statement, nullptr)` asks SQLite to parse/prepare the SQL for this connection. `-1` says the SQL is NUL-terminated; the final optional tail pointer is unused. On success, SQLite fills `statement`.
- Bind positions are numbered from 1. For the SLIMS `Book`, bind `getBookID()` to 1, ISBN to 2, title to 3, author to 4, category to 5, publication year to 6, and status to 7.
- Use `sqlite3_bind_text` for text and `sqlite3_bind_int` for the publication year. C++ strings are passed as `.c_str()`. A text length of `-1` means NUL-terminated text. `SQLITE_TRANSIENT` tells SQLite to copy the text before the bind call returns, so it does not depend on the lifetime of the C++ string buffer.
- `Book::getbook()` returns an enum, so convert it into a database representation such as `"Available"` or `"Issued"` before binding. This is the mapping from a C++ enum to stored SQL text.
- Check each bind result. If binding fails, report `sqlite3_errmsg(db)`, finalize the statement, and return failure.
- `sqlite3_step(statement)` executes the prepared insert. A successful insert returns `SQLITE_DONE`. Errors such as a duplicate primary key return a different result and can be explained with `sqlite3_errmsg(db)`.
- `sqlite3_finalize(statement)` releases the prepared-statement resource after success or failure. It does not close the database connection; the `Database` object owns that separate connection and closes it in its destructor.
- Prepared statements keep SQL structure separate from values. This safely handles quotes in values and avoids constructing SQL by concatenating user input.
- A sample insert with a fixed primary-key ID succeeds once. Running it again with the same ID violates the primary-key constraint, which protects against duplicate book IDs.
- Adding a book to SQLite does not automatically add it to the in-memory `Library::books` vector. Loading database rows into `Library` is a later step.

## Startup order for the initial database exercise

1. Construct a `Database` object.
2. Call `open("slims.db")`.
3. Call `enableForeignKeys()` for this connection.
4. Call `createTables()` to make missing tables.
5. Construct one sample `Book` and call `insertBook(sampleBook)`.
6. Return from `main`; the `Database` destructor closes the connection.

The database file is created in the program's current working directory. Creating the schema makes empty tables; it does not populate them by itself.

## How `Database::insertbook` works

1. It first checks `db`. If the database has not been opened, it prints an error and returns `false` without preparing SQL.
2. It stores an `INSERT INTO books` statement in `sql`. The column list fixes the order of the seven values, and each `?` is a placeholder to be filled later.
3. It declares `sqlite3_stmt* statement = nullptr`. `sqlite3_prepare_v2` parses the SQL for this database connection and, on success, puts the prepared statement handle in `statement`. The `-1` length means SQLite should read the SQL through its terminating NUL character.
4. If preparation fails, the function reports `sqlite3_errmsg(db)` and returns `false`. No statement was successfully prepared for execution.
5. It translates the C++ `bookstatus` enum to the text SQLite will store: `Available` or `Issued`.
6. It binds the `Book` properties to the placeholders in matching order: book ID (1), ISBN (2), title (3), author (4), category (5), publication year (6), and status (7). Text values use `sqlite3_bind_text`; the year uses `sqlite3_bind_int`. `SQLITE_TRANSIENT` asks SQLite to copy each text value, so SQLite does not depend on the lifetime of the C++ string buffer.
7. It calls `sqlite3_step(statement)` to run the insert. `SQLITE_DONE` means the row was inserted; a different result means the insert failed, for example because `book_id` already exists.
8. It calls `sqlite3_finalize(statement)` to release the prepared statement on the success and insert-failure paths. The `Database` destructor separately closes the connection.
9. It prints a success message and returns `true` only after `sqlite3_step` returns `SQLITE_DONE`.

### Binding check

`insertbook` stores the combined results of its `sqlite3_bind_*` calls in `bindingsSucceeded` and checks the value before calling `sqlite3_step`. If any binding fails, it reports the SQLite error, finalizes `statement`, and returns `false` without attempting the insert.

The function writes to the SQLite `books` table only. It does not add the `Book` to the in-memory `Library::books` vector; that requires a separate call to `Library::addbook` if both copies are needed.

## Selecting all books from SQLite

`Database::selectbooks(vector<Book>& books)` reads every row from the `books` table and places the results in the supplied vector.

- It first checks that the database connection is open, then prepares a `SELECT` statement naming the seven columns needed to rebuild a `Book`.
- `sqlite3_step(statement)` returns `SQLITE_ROW` for each row. The function reads text columns with `sqlite3_column_text` and the publication year with `sqlite3_column_int`.
- The `SELECT` column order determines the zero-based column indexes: book ID 0, ISBN 1, title 2, author 3, category 4, publication year 5, and status 6.
- `sqlite3_column_text` returns a pointer to SQLite-owned text. The function copies it into a C++ `string` before calling `sqlite3_step` again, because SQLite may invalidate the pointer when it advances to another row.
- The stored status text is converted back to the C++ `bookstatus` enum. The function accepts `Available` and `Issued`; an unexpected value is reported as an error rather than silently treated as `Issued`. `books.emplace_back(...)` constructs a `Book` from a valid row and appends it to the vector.
- When no more rows remain, `sqlite3_step` returns `SQLITE_DONE`. Any other result is reported as a read failure. The prepared statement is finalized on both the read-error and successful paths.
- The function clears the supplied vector after successfully preparing the query, so each call replaces its contents with the current database rows instead of appending duplicates.
