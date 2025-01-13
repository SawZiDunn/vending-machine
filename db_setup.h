// to run only one time

#include <iostream>
#include <sqlite3.h>
#include <string>

void executeSQL(sqlite3 *db, const std::string &sql);

int setup_db()
{
    sqlite3 *db;
    const char *database_name = "my_database.db";

    // Open (or create) the database
    int rc = sqlite3_open(database_name, &db);
    if (rc)
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Opened database successfully: " << database_name << std::endl;
    }

    std::string createTableSQL = R"(
    CREATE TABLE IF NOT EXISTS stocks_67011653 (
        item_id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL UNIQUE,
        price INTEGER NOT NULL CHECK (price > 0),
        quantity INTEGER NOT NULL
    );

    CREATE TABLE IF NOT EXISTS change_box (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        denomination INTEGER NOT NULL UNIQUE,
        quantity INTEGER NOT NULL CHECK(quantity >= 0 AND quantity <= 30)
    );

    CREATE TABLE IF NOT EXISTS collection_box (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        denomination INTEGER NOT NULL UNIQUE,
        quantity INTEGER NOT NULL CHECK(quantity >= 0 AND quantity <= 30)
    );
    )";

    std::string populate_items = R"(
    INSERT INTO stocks_67011653 (name, price, quantity) VALUES ('Coke', 30, 30);
    INSERT INTO stocks_67011653 (name, price, quantity) VALUES ('Pepsi', 23, 30);
    INSERT INTO stocks_67011653 (name, price, quantity) VALUES ('Snickers', 30, 30);
    INSERT INTO stocks_67011653 (name, price, quantity) VALUES ('KitKat', 14, 30);
    INSERT INTO stocks_67011653 (name, price, quantity) VALUES ('Potato Chips', 10, 30);
    INSERT INTO stocks_67011653 (name, price, quantity) VALUES ('Mineral Water (500ml)', 25, 30);
    INSERT INTO stocks_67011653 (name, price, quantity) VALUES ('Chocolate Milk', 45, 30);
    INSERT INTO stocks_67011653 (name, price, quantity) VALUES ('Oreo Cookies', 9, 30);

    )";
    std::string populate_collection_box = R"(
    INSERT INTO collection_box (denomination, quantity) VALUES (1, 0);
    INSERT INTO collection_box (denomination, quantity) VALUES (5, 0);
    INSERT INTO collection_box (denomination, quantity) VALUES (10, 0);
    INSERT INTO collection_box (denomination, quantity) VALUES (20, 0);
    INSERT INTO collection_box (denomination, quantity) VALUES (100, 0);
    )";

    std::string populate_change_box = R"(
    INSERT INTO change_box (denomination, quantity) VALUES (1, 30);
    INSERT INTO change_box (denomination, quantity) VALUES (5, 30);
    INSERT INTO change_box (denomination, quantity) VALUES (10, 30);
    INSERT INTO change_box (denomination, quantity) VALUES (20, 30);
    INSERT INTO change_box (denomination, quantity) VALUES (100, 30);
    )";

    // Execute the SQL command
    executeSQL(db, createTableSQL);
    executeSQL(db, populate_items);
    executeSQL(db, populate_collection_box);
    executeSQL(db, populate_change_box);

    // Close the database
    sqlite3_close(db);
    std::cout << "Database closed." << std::endl;
    return 0;
}

void executeSQL(sqlite3 *db, const std::string &sql)
{
    char *errorMessage = nullptr;

    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
    else
    {
        std::cout << "SQL executed successfully: " << sql << std::endl;
    }
}