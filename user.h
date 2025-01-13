#pragma once
#include "utils.h"
#include "database_controller.h"
#include "moneyInstance.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

class User
{

public:
    User()
    {

        while (true)
        {
            if (isOutOfStock())
            {
                cout << "\nThe Vending Machine is OUT OF STOCK!" << endl;
                break;
            }
            else if (isNoChange())
            {
                cout << "\nChange Box is empty! You cannot make any purchase!" << endl;
                break;
            }
            else if (isCollectBoxFull())
            {
                cout << "\nCollection Box is full! You cannot make any purchase!" << endl;
                break;
            }
            else
            {
                view_items();
                purchase_item();
            }
        }
    }

private:
    DatabaseController db{};

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

        int id = getValidNumber("\nEnter Item Code:");

        auto item = db.get_single_item(to_string(id));

        if (item.item_id == -1)
        {
            cout << "\nProduct does not exist!" << endl;
            return;
        }

        print_item(item); // print item details

        if (item.quantity == 0)
        {
            cout << "\nTHIS ITEM IS OUT OF STOCK!" << endl;
            return;
        }

        int to_pay = item.price;
        int total_change = 0;
        vector<MoneyInstance> money_instance_collections; // to store money instance for return change

        while (to_pay > 0)
        {

            int amt = getValidNumber("\nInsert Money: ");

            if (amt != 100 && amt != 20 && amt != 10 && amt != 5 && amt != 1)
            {
                cout << "\nPlease insert only (100, 20, 10, 5, 1) Bahts!" << endl;
                continue;
            }

            if (amt >= to_pay)
            {

                total_change = amt - to_pay;
                to_pay = 0;
            }
            else
            {
                to_pay -= amt;
                cout << "\nYou inserted " << amt << " Baht.\n"
                     << to_pay << " Baht more to pay!" << endl;
            }

            addMoneyInstance(amt, money_instance_collections);
        }

        vector<MoneyInstance> refundableChange = getRefundableChange(total_change);

        if (willCollectBoxFull(money_instance_collections))
        {
            cout << "\nSorry! The money collection box is full!" << endl;
            cout << "Here is your refund: " << item.price << " Baht" << endl;
            return;
        }
        else if (refundableChange.empty() && total_change != 0)
        {
            cout << "\nSorry! Change box does not have enough change to return!" << endl;
            cout << "You cannot make this purchase!" << endl;
            cout << "Please take back your money : " << total_change + item.price << " Baht" << endl;
            return;
        }

        // complete final transaction
        insertMoneyToCollectionBox(money_instance_collections);

        db.decrease_item_quantity(to_string(item.item_id));

        decreaseMoneyFromChangeBox(refundableChange);
        cout << "***********************" << endl;

        // display change details
        if (total_change > 0)
        {
            cout << "You received : " << endl;
        }
        for (MoneyInstance &box : refundableChange)
        {
            if (box.quantity > 0)
            {
                cout << box.quantity << " notes of " << box.denomination << " Baht" << ",\n";
            }
        }

        cout << "You successfully purchased " << item.name << "!" << endl;
        cout << "Total Change: " << fixed << setprecision(2) << total_change << " Baht" << endl;
    }

    void print_item(const Item &item)
    {
        cout << "\n********* Your Selected Item *********\n";
        cout << "---------------------------------------------\n";
        cout << setw(7) << left << "ID"
             << setw(25) << "Name"
             << setw(10) << "Price" << endl;
        cout << "---------------------------------------------\n";
        cout << setw(7) << left << item.item_id
             << setw(25) << item.name
             << setw(10) << fixed << setprecision(2) << item.price << " Baht" << endl;
        cout << "---------------------------------------------" << endl;
    }

    // Function to calculate refundable change
    vector<MoneyInstance> getRefundableChange(int total_change)
    {
        int temp_total_change = 0;
        vector<MoneyInstance> db_change_instances = db.get_money_instances("change_box");
        vector<MoneyInstance> refundable_change;

        // sort instances in descending order
        sort_money_instances(db_change_instances);

        for (MoneyInstance &i : db_change_instances)
        {
            while (i.quantity != 0 && i.denomination + temp_total_change <= total_change)
            {
                temp_total_change += i.denomination;
                --i.quantity;
                addMoneyInstance(i.denomination, refundable_change);
            }

            // immediately return if total change is achieved
            if (temp_total_change == total_change)
            {
                return refundable_change;
            }
        }

        // empty vector if total change is not refundable

        if (temp_total_change != total_change)
        {
            return {};
        }

        return refundable_change;
    }

    // sort money instances according to denomination in descending order
    void sort_money_instances(vector<MoneyInstance> &money_instance_collection)
    {
        for (int i = 0; i < money_instance_collection.size() - 1; ++i)
        {
            for (int j = i; j < money_instance_collection.size() - i - 1; ++j)
            {
                if (money_instance_collection[j].denomination < money_instance_collection[j + 1].denomination)
                {
                    MoneyInstance temp = money_instance_collection[j];
                    money_instance_collection[j] = money_instance_collection[j + 1];
                    money_instance_collection[j + 1] = temp;
                }
            }
        }
    }

    // create a money instance and push to the original vector
    void addMoneyInstance(int amount, vector<MoneyInstance> &money_instance_collections)
    {

        for (auto &i : money_instance_collections)
        {
            if (i.denomination == amount)
            {
                ++i.quantity;
            }
        }

        MoneyInstance moneybox;
        moneybox.denomination = amount;
        moneybox.quantity = 1;
        money_instance_collections.push_back(moneybox);
    }

    // check if the collection box will be full after user inserts coins
    bool willCollectBoxFull(const vector<MoneyInstance> &money_collections)
    {
        vector<MoneyInstance> money_instances = db.get_money_instances("collection_box");

        for (const auto &box : money_collections)
        {

            for (const auto &instance : money_instances)
            {
                if (box.denomination == instance.denomination && box.quantity + instance.quantity > 3)
                {
                    return true;
                }
            }
        }
        return false;
    }

    void insertMoneyToCollectionBox(vector<MoneyInstance> money_collections)
    {
        for (auto box : money_collections)
        {
            db.insert_money(box.denomination, box.quantity);
        }
    }

    bool decreaseMoneyFromChangeBox(vector<MoneyInstance> refundableChange)
    {
        if (refundableChange.empty())
        {
            return false;
        }

        for (auto box : refundableChange)
        {
            db.decrease_money(box.denomination, box.quantity);
        }
        return true;
    }

    bool isOutOfStock()
    {
        vector<Item> items = db.get_all_items("stocks_67011653");
        int emptyCount = 0;
        for (Item &item : items)
        {
            if (item.quantity == 0)
            {
                ++emptyCount;
            }
        }

        return static_cast<float>(emptyCount) / static_cast<float>(items.size()) >= 0.5;
    }

    bool isNoChange()
    {
        const vector<MoneyInstance> moneyboxes = db.get_money_instances("change_box");
        for (const MoneyInstance &box : moneyboxes)
        {
            if (box.quantity == 0)
            {
                return true;
            }
        }

        return false;
    }
    bool isCollectBoxFull()
    {
        const vector<MoneyInstance> moneyboxes = db.get_money_instances("collection_box");
        for (const MoneyInstance &box : moneyboxes)
        {
            if (box.quantity == 30)
            {
                return true;
            }
        }

        return false;
    }
};