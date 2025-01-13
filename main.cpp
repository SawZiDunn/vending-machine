#include <iostream>
#include <string>
#include "user.h"
#include "admin.h"

using namespace std;

int main()
{

    string choice;

    while (true)
    {
        cout << "\n********* Vending Machine ********\n";
        cout << "Choose an option:\n";
        cout << "1. User\n";
        cout << "2. Admin\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        getline(cin, choice);

        if (choice == "1")
        {
            cout << "\nEntering User Mode...\n";
            User user;
        }
        else if (choice == "2")
        {
            cout << "\nEntering Admin Mode...\n";
            Admin admin;
        }
        else if (choice == "3")
        {
            cout << "Exiting program. Goodbye!" << endl;
            break;
        }
        else
        {
            cout << "Invalid Choice! Please enter 1, 2, or 3." << endl;
        }
    }

    return 0;
}