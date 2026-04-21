#pragma once
#include <iostream>
#ifndef PASS_USERID_H
#define PASS_USERID_H

using namespace std;

class pass 
{
public:
    static string currentUserID;
    static void SetUserID(const string& userID) 
    {
        currentUserID = userID;
    }

    // Get the UserID
    static string GetUserID() 
    {
        return currentUserID;
    }

    static bool IsUserIDSet() 
    {
        return !currentUserID.empty();
    }
};

#endif