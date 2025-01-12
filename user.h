#pragma once
#include "utils.h"
#include "database_controller.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

struct MoneyBox
{
    int denomination;
    int quantity;
};

class User
{

public:
    User()
    {

        while (true)
        {
            view_items();
            purchase_item();
        }
    }

    void view_items()
    {
        auto items = db.get_all_items("stocks_67011653");

        if (items.size() == 0)
        {
            cout << "\nOut of Stock!" << endl;
        }
        else
        {
            cout << endl;

            cout << setw(7) << left << "ID" << setw(25) << "Name" << "Price" << endl;
            cout << endl;
            for (const auto &item : items)
            {
                cout << setw(7) << item.item_id
                     << setw(25) << item.name
                     << setw(10) << item.price << endl;
            }
        }
    }

    void purchase_item()
    {
        string id;
        cout << "\nEnter Item Code: ";
        getline(cin, id);

        if (!isValidNumber(id))
        {
            cout << "Invalid input! Please enter a valid numeric Item Code."
                 << endl;
            return;
        }

        auto item = db.get_single_item(id);

        if (item.item_id == -1)
        {
            cout << "\nProduct does not exist!" << endl;
            return;
        }

        cout << "\n***Your selected Item***\n";
        cout << setw(7) << left << "ID" << setw(25) << "Name" << "Price" << endl;
        cout << "---------------------------------------------" << endl;

        if (item.quantity == 0)
        {
            cout << "\nOUT OF STOCK!" << endl;
        }

        int to_pay = item.price;
        int total_change = 0;

        while (to_pay != 0)
        {
            string money;
            cout << "\nInsert Money: ";
            getline(cin, money);

            if (!isValidNumber(money))
            {
                cout << "Invalid Amount!" << endl;
                continue;
            }

            int x = stoi(money);
            if (x != 100 && x != 20 && x != 10 && x != 5 && x != 1)
            {
                cout << "Please insert only (100, 20, 10, 5, 1) Bahts!" << endl;
                continue;
            }

            if (x >= to_pay)
            {
                total_change = x - to_pay;
                to_pay = 0;
            }
            else
            {
                to_pay -= x;
                cout << "You inserted " << x << " Baht.\n"
                     << to_pay << " Baht left to pay!" << endl;
            }
        }

        vector<MoneyBox> moneybox_collections = separate_money(item.price);
        if (!isCollectionBoxFull(moneybox_collections))
        {
            insertMoneyToCollectionBox(moneybox_collections);
            cout << "You successfully purchased " << item.name << endl;
            db.decrease_item_quantity(to_string(item.item_id));
            cout << "Total Change: " << total_change << endl;
        }
        else
        {
            cout << "Sorry! The money collection box is full!" << endl;
            cout << "Here is your refund: " << item.price << " Baht" << endl;
        }
    }

    vector<MoneyBox> separate_money(int price)
    {
        vector denominations = {100, 20, 10, 5, 1};
        vector<MoneyBox> moneybox_collections;
        for (int i : denominations)
        {
            if (price / i > 0)
            {
                MoneyBox moneybox;
                moneybox.denomination = i;
                moneybox.quantity = price / i;
                price = price % i;
                moneybox_collections.push_back(moneybox);
            }
        }
        return moneybox_collections;
    }

    bool isCollectionBoxFull(const vector<MoneyBox> &money_collections)
    {
        for (auto box : money_collections)
        {
            int q = db.getQuantity(box.denomination, "collection_box");
            if (q + box.quantity > 3)
            {
                return true;
            }
        }
        return false;
    }

    void insertMoneyToCollectionBox(vector<MoneyBox> money_collections)
    {
        for (auto box : money_collections)
        {
            db.insert_money(box.denomination, box.quantity);
        }
    }

private:
    DatabaseController db{};
};