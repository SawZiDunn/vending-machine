#include <iostream>
#include <string>
#include <fstream>
#include "user.h"
#include "admin.h"
#include "db_setup.h"

using namespace std;

bool fileExists(const std::string &fileName)
{
    std::ifstream file(fileName);
    return file.good();
}

int main()
{
    const std::string database_name = "my_database.db";

    if (!fileExists(database_name))
    {
        std::cout << "Database not found. Setting up the database..." << std::endl;
        setup_db(); // create tables and populate some data
    }
    // else
    // {
    //     std::cout << "Database already exists. Proceeding to main functionality..." << std::endl;
    // }

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