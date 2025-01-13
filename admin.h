#pragma once

#include "db_controller.h"
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
            cout << "\n1. Add a new item\n"
                 << "2. Refill a stock\n"
                 << "3. Check change box\n"
                 << "4. Check collection box\n"
                 << "5. Refill change box\n"
                 << "6. Collect collection box\n"
                 << "7. View Items\n"
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
                collect_collection_box();
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

private:
    DatabaseController db{};

    void add_stock()
    {
        string name;
        int price;
        int quantity;

        cout << "Product Name: ";
        getline(cin, name);

        price = getValidNumber("Unit Price: ");

        quantity = getValidNumber("Quantity: ");

        db.add_stock(name, price, quantity);
        std::cout << "A new item added successfully!" << std::endl;
    }

    void refill_stock()
    {
        string id;
        int quantity;

        view_products();

        cout << "\nEnter product id: ";
        getline(cin, id);

        quantity = getValidNumber("Quantity: ");

        db.refill_stock(id, quantity);
        cout << "Stock refilled successfully!" << endl;
    }

    void check_change_box()
    {
        int total = db.check_money_box("change_box");
        cout << "\nYou have " << total << " Baht in the Change Box." << endl;
    }

    void check_collection_box()
    {
        int total = db.check_money_box("collection_box");
        cout << "\nYou have " << total << " Baht in the Collection Box." << endl;
    }

    void refill_change_box()
    {
        db.refill_change_box();
        cout << "Change Box is successfully refilled!" << endl;
    }

    void collect_collection_box()
    {
        int total = db.check_money_box("collection_box");

        db.collect_collection_box();
        cout << "You have collected " << total << " Baht from the Collection Box." << endl;
    }

    void view_products()
    {
        auto items = db.get_all_items("stocks_67011653");

        if (items.empty())
        {
            cout << "\nNo items in the table!" << endl;
        }
        else
        {
            cout << "\n************ Available Items ************\n";
            cout << "---------------------------------------------\n";
            cout << setw(7) << left << "ID"
                 << setw(25) << "Name"
                 << setw(10) << "Price"
                 << "Quantity" << endl;
            cout << "---------------------------------------------\n"
                 << endl;

            for (const auto &item : items)
            {
                cout << setw(7) << left << item.item_id
                     << setw(25) << item.name
                     << setw(10) << fixed << setprecision(2) << item.price
                     << item.quantity << endl;
            }
            cout << "---------------------------------------------" << endl;
        }
    }
};
