#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>

using namespace std;

// represents each item
struct Item
{
    int item_id;
    string name;
    float price;
    int quantity;
};

class DatabaseController
{
public:
    DatabaseController()
    {
        int rc;

        // open database
        rc = sqlite3_open("my_database.db", &db);

        if (rc != SQLITE_OK)
        {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            db = NULL;
        }
    }

    ~DatabaseController()
    {
        if (db)
        {
            sqlite3_close(db); // Close the database when the object goes out of scope
            std::cout << "Closed database successfully!" << std::endl;
        }
    }

    void add_stock(string name, string price, string quantity)
    {
        // Create SQL string with the values to be inserted
        string values = "'" + name + "', " + price + ", " + quantity;
        string insertSQL = "INSERT INTO stocks_67011653 (name, price, quantity) VALUES (" + values + ");";

        char *errorMessage = nullptr;
        int rc = sqlite3_exec(db, insertSQL.c_str(), nullptr, nullptr, &errorMessage);

        if (rc != SQLITE_OK)
        {
            std::cerr << "SQL error: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
        }
        else
        {
            std::cout << "A new item added successfully!" << std::endl;
        }
    };

    vector<Item> get_all_items(const string &tableName)
    {
        vector<Item> items = {};

        string query = "SELECT * FROM " + tableName + ";";
        sqlite3_stmt *stmt;

        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

        if (rc != SQLITE_OK)
        {
            cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
            return items; // Return an empty table
        }

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            Item item;
            item.item_id = sqlite3_column_int(stmt, 0);                               // Assuming column 0 is "id"
            item.name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)); // Column 1 is "name"
            item.price = static_cast<float>(sqlite3_column_double(stmt, 2));          // Column 2 is "price"
            item.quantity = sqlite3_column_int(stmt, 3);                              // Column 3 is "quantity"

            items.push_back(item); // Add the item to the vector
        }
        sqlite3_finalize(stmt);
        return items;
    }

    Item get_single_item(const string &item_id)
    {
        Item item;
        string query = "SELECT * FROM stocks_67011653 WHERE item_id = " + item_id + ";";

        sqlite3_stmt *stmt;
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

        if (rc != SQLITE_OK)
        {
            cout << query << endl;
            cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << endl;
            item.item_id = -1;
            return item;
        }

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            item.item_id = sqlite3_column_int(stmt, 0);                               // Assuming column 0 is "id"
            item.name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)); // Column 1 is "name"
            item.price = static_cast<float>(sqlite3_column_double(stmt, 2));          // Column 2 is "price"
            item.quantity = sqlite3_column_int(stmt, 3);                              // Column 3 is "quantity"
        }
        else
        {
            item.item_id = -1;
        }
        sqlite3_finalize(stmt);
        cout << item.item_id << item.name << endl;
        return item;
    }

    void refill_stock(const string &id, string quantity)
    {
        string query = "SELECT quantity FROM stocks_67011653 WHERE item_id = " + id + ";";

        sqlite3_stmt *stmt;
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

        if (rc != SQLITE_OK)
        {
            cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << endl;
            return;
        }

        // Execute the query and get the current quantity
        int currentQuantity = 0;
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            currentQuantity = sqlite3_column_int(stmt, 0); // Get the 'quantity' column value
        }
        else
        {
            cout << "Product not found!" << endl;
            sqlite3_finalize(stmt);
            return;
        }

        // Add the new quantity to the current quantity
        int newQuantity = currentQuantity + stoi(quantity);

        string updateQuery = "UPDATE stocks_67011653 SET quantity = " + to_string(newQuantity) + " WHERE item_id = " + id + ";";

        char *errorMessage = nullptr;
        rc = sqlite3_exec(db, updateQuery.c_str(), nullptr, nullptr, &errorMessage);

        if (rc != SQLITE_OK)
        {
            cerr << "SQL error: " << errorMessage << endl;
            sqlite3_free(errorMessage);
        }
        else
        {
            cout << "Stock refilled successfully!" << endl;
        }

        sqlite3_finalize(stmt);
    }

    // get quantity for a single denomination
    int getQuantity(int denomination, const string &table_name)
    {
        string query = "SELECT quantity FROM " + table_name + " WHERE denomination = " + to_string(denomination) + ";";

        sqlite3_stmt *stmt;
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

        if (rc != SQLITE_OK)
        {
            cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << endl;
            return 0;
        }

        int quantity;
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int q = sqlite3_column_int(stmt, 1); // Get the 'quantity' column value
            quantity = q;
        }

        sqlite3_finalize(stmt);
        return quantity;
    }

    // decrease item quantity by 1
    void decrease_item_quantity(const string &item_id)
    {
        string query = "SELECT quantity FROM stocks_67011653 WHERE item_id = " + item_id + ";";

        sqlite3_stmt *stmt;
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

        if (rc != SQLITE_OK)
        {
            cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << endl;
            return;
        }

        // Execute the query and get the current quantity
        int currentQuantity = 0;
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            currentQuantity = sqlite3_column_int(stmt, 0); // Get the 'quantity' column value
        }
        else
        {
            cout << "Product not found!" << endl;
            sqlite3_finalize(stmt);
            return;
        }

        // Add the new quantity to the current quantity
        int newQuantity = currentQuantity - 1;

        string updateQuery = "UPDATE stocks_67011653 SET quantity = " + to_string(newQuantity) + " WHERE item_id = " + item_id + ";";

        char *errorMessage = nullptr;
        rc = sqlite3_exec(db, updateQuery.c_str(), nullptr, nullptr, &errorMessage);

        if (rc != SQLITE_OK)
        {
            cerr << "SQL error: " << errorMessage << endl;
            sqlite3_free(errorMessage);
        }

        sqlite3_finalize(stmt);
    }

    // insert a single denomination (100, 20, etc) to collection_box
    void insert_money(int denomination, int quantity)
    {
        string query = "SELECT quantity FROM collection_box WHERE denomination = " + to_string(denomination) + ";";

        sqlite3_stmt *stmt;
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

        if (rc != SQLITE_OK)
        {
            cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << endl;
            return;
        }

        int currentQuantity = 0;
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            currentQuantity = sqlite3_column_int(stmt, 0);
        }

        int newQuantity = currentQuantity + quantity;

        string updateQuery = "UPDATE collection_box SET quantity = " + to_string(newQuantity) + " WHERE denomination = " + to_string(denomination) + ";";

        char *errorMessage = nullptr;
        rc = sqlite3_exec(db, updateQuery.c_str(), nullptr, nullptr, &errorMessage);

        if (rc != SQLITE_OK)
        {
            cerr << "SQL error: " << errorMessage << endl;
            sqlite3_free(errorMessage);
        }

        sqlite3_finalize(stmt);
    }

    void refill_change_box()
    {
        string query = "UPDATE change_box SET quantity = 30;"; // SQL query to reset quantities to 0
        char *errorMessage = nullptr;

        int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errorMessage);

        if (rc != SQLITE_OK)
        {
            cerr << "SQL error: " << errorMessage << endl;
            sqlite3_free(errorMessage);
        }
        else
        {
            cout << "Change box is successfully refilled!" << endl;
        }
    }

    void collect_change_box()
    {
        string query = "UPDATE collection_box SET quantity = 0;"; // SQL query to reset quantities to 0
        char *errorMessage = nullptr;

        int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errorMessage);

        if (rc != SQLITE_OK)
        {
            cerr << "SQL error: " << errorMessage << endl;
            sqlite3_free(errorMessage);
        }
        else
        {
            cout << "Collection box quantities have been reset!" << endl;
        }
    }

    // get total money from money_box
    int check_money_box(const string &table_name)
    {
        string query = "SELECT denomination, quantity FROM " + table_name + ";";
        int total = 0;

        sqlite3_stmt *stmt;
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

        if (rc != SQLITE_OK)
        {
            cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << endl;
            return 1;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int denomination = sqlite3_column_int(stmt, 0);
            int quantity = sqlite3_column_int(stmt, 1); // Get the 'quantity' column value
            total += denomination * quantity;
        }

        sqlite3_finalize(stmt);
        return total;
    }

private:
    sqlite3 *db = nullptr;

    // bool executeSQL(const string &query)
    // {
    //     char *errorMessage = nullptr;

    //     int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errorMessage);
    //     if (rc != SQLITE_OK)
    //     {
    //         cerr << "SQL error: " << errorMessage << endl;
    //         sqlite3_free(errorMessage);
    //         return false;
    //     }
    //     return true;
    // }
};
