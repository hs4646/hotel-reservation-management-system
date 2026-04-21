#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include <string>
#include <regex>
#include <ctime>

using namespace std;

inline bool isValidDateFormat(const string& date) {
    // Validate the format using a regex
    regex dateRegex(R"(\d{4}-\d{2}-\d{2})");
    if (!regex_match(date, dateRegex)) {
        return false;
    }

    try {
        // Split the date string and extract year, month, day
        int year = stoi(date.substr(0, 4));
        int month = stoi(date.substr(5, 2));
        int day = stoi(date.substr(8, 2));

        // Basic validation for month and day ranges
        if (month < 1 || month > 12 || day < 1 || day > 31) {
            return false;
        }

        // Validate days in the given month
        if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
            return false;
        }

        if (month == 2) {
            bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            if (isLeapYear && day > 29) return false;
            if (!isLeapYear && day > 28) return false;
        }

        return true;
    }
    catch (const exception&) {
        // Handle invalid substrings or conversion errors
        return false;
    }
}

inline bool isTodayOrFuture(const string& date) {
    try {
        // Extract year, month, day
        int year = stoi(date.substr(0, 4));
        int month = stoi(date.substr(5, 2));
        int day = stoi(date.substr(8, 2));

        // Get the current date
        time_t now = time(0);
        tm localTime;

        // Use localtime_s instead of localtime
        localtime_s(&localTime, &now); // Safer version

        int currentYear = 1900 + localTime.tm_year;
        int currentMonth = 1 + localTime.tm_mon;
        int currentDay = localTime.tm_mday;

        // Compare the provided date with the current date
        if (year > currentYear) return true;
        if (year == currentYear && month > currentMonth) return true;
        if (year == currentYear && month == currentMonth && day >= currentDay) return true;

        return false;
    }
    catch (...) 
    {
        cout << "Invalid date: " << endl;
        return false;
    }

}

inline bool isRoomAvailable(mySQL_db& db, const string& RoomID, const string& DateIn, const string& DateOut) 
{
    db.prepareStatement("SELECT COUNT(*) AS OverlapCount FROM reservation WHERE RoomID = ? AND(DateIn < ? AND DateOut > ? ); ");
    db.statement->setString(1, RoomID);
    db.statement->setString(2, DateOut); // DateOut > existing DateIn
    db.statement->setString(3, DateIn); // DateIn < existing DateOut
    db.QueryResult();

    if (db.result->next()) {
        int overlapCount = db.result->getInt("OverlapCount");
        return overlapCount == 0; // Room is available if no overlapping reservations
    }
   
    return false; // If query fails, assume room is unavailable
}

inline string getCurrentDate()
{
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    tm localTime;

    // Convert to local time
    if (localtime_s(&localTime, &now_c) != 0) {
        throw runtime_error("Failed to get the local time.");
    }

    // Format the date
    stringstream dateStream;
    dateStream << put_time(&localTime, "%Y-%m-%d");
    return dateStream.str(); // Return the formatted date as a string
}

#endif
