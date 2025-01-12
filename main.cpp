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
        cout << "User(1) or Admin(2): ";
        getline(cin, choice);

        if (choice == "1")
        {
            User user;
        }
        else if (choice == "2")
        {
            // admin
            Admin admin;
        }
        else
        {
            cout << "Invalid Choice!" << endl;
            return 1;
        }
    }

    return 0;
}