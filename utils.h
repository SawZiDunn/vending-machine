#pragma once

#include <iostream>
#include <string>

bool isValidNumber(const std::string &s)
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