#pragma once

#include <iostream>
#include <string>

using namespace std;

bool isValidNumber(const string &s)
{
    try
    {
        stof(s);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

int getValidNumber(const string prompt)
{
    while (true)
    {
        string input;
        cout << prompt;
        getline(cin, input);
        if (isValidNumber(input))
        {
            return stoi(input);
        }
        else
        {
            cout << "Please inter a valid input!" << endl;
        }
    }

    return 0;
}
