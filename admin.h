#pragma once

#include "database_controller.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

class Admin
{
public:
    string choice;

    Admin()
    {
        while (true)
        {
            cout << "\n1. Add a new product\n"
                 << "2. Refill a stock\n"
                 << "3. Check change box\n"
                 << "4. Check collection box\n"
                 << "5. Refill change box\n"
                 << "6. Collect collection box\n"
                 << "7. View products\n"
                 << "8. Exit\n> ";

            getline(cin, choice);

            if (choice == "1")
            {
                add_stock();
            }
            else if (choice == "2")
            {
                refill_stock();
            }
            else if (choice == "3")
            {
                check_change_box();
            }
            else if (choice == "4")
            {
                check_collection_box();
            }
            else if (choice == "5")
            {
                refill_change_box();
            }
            else if (choice == "6")
            {
                collect_change_box();
            }
            else if (choice == "7")
            {
                view_products();
            }
            else if (choice == "8")
            {
                cout << "Existing..." << endl;
                break;
            }
            else
            {
                cout << "Invalid Choice!" << endl;
            }
        }
    }

    void add_stock()
    {
        string name;
        string price;
        string quantity;

        cout << "Product Name: ";
        getline(cin, name);

        cout << "Unit Price: ";
        getline(cin, price);

        if (!isValidNumber(price))
        {
            cout << "Invalid input" << endl;
        }

        cout << "Quantity: ";
        getline(cin, quantity);
        if (!isValidNumber(quantity))
        {
            cout << "Invalid input" << endl;
        }

        // removing floating points
        int temp = stoi(quantity);
        quantity = to_string(temp);

        db.add_stock(name, price, quantity);
    }

    void refill_stock()
    {
        string id;
        string quantity;

        cout << "Enter product id: ";
        getline(cin, id);

        cout << "Quantity: ";
        getline(cin, quantity);

        if (!isValidNumber(quantity))
        {
            cout << "Invalid input" << endl;
        }

        int temp = stoi(quantity);
        quantity = to_string(temp);

        db.refill_stock(id, quantity);
    }

    void check_change_box()
    {
        int total = db.check_money_box("change_box");
        cout << "Total amount: " << total << " Baht" << endl;
    }

    void check_collection_box()
    {
        int total = db.check_money_box("collection_box");
        cout << "Total amount: " << total << " Baht" << endl;
    }

    void refill_change_box()
    {
        db.refill_change_box();
    }

    void collect_change_box()
    {
        int total = db.check_money_box("change_box");
        cout << "You have collected " << total << " Baht" << endl;
        db.collect_change_box();
    }

    void view_products()
    {
        auto items = db.get_all_items("stocks_67011653");

        if (items.size() == 0)
        {
            cout << "\nNo items in the table!" << endl;
        }
        else
        {
            cout << endl;

            cout << setw(7) << left << "ID" << setw(25) << "Name" << setw(10) << "Price" << "Quantity" << endl;
            for (auto item : items)
            {
                cout << setw(7) << item.item_id
                     << setw(25) << item.name
                     << setw(10) << item.price
                     << item.quantity << endl;
            }
        }
    }

private:
    DatabaseController db{};
};
