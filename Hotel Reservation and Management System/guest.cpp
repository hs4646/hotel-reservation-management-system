#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <sstream>
#include <mysql/jdbc.h>
#include "mysql.h"
#include "guest.h"
#include "DateUtils.h"
#include "passUserID.h"
#include "Menu.h"

using namespace std;

mySQL_db db;
Menu menu;
guest make;
pass Pass;
string GuestID, Name, Contact_No, Email, RoomID, Type, Status, ReservationID, StaffID, input;
int selection, choice;
char option;
double Price;

void guest::ViewRoom()
{
    system("cls");

    db.prepareStatement("SELECT r.RoomID, r.Type, r.Price, IFNULL(COUNT(res.RoomID), 0) AS activeReservations FROM room r LEFT JOIN reservation res "
        "ON r.RoomID = res.RoomID AND res.DateIn <= ? AND res.DateOut >= ? GROUP BY r.RoomID");

    string today = getCurrentDate();
    db.statement->setString(1, today);
    db.statement->setString(2, today);

    db.QueryResult();

    cout << "+---------------+-------------------------+--------------------+--------------------+" << endl;
    cout << "| RoomID        | Room Type               | Price per night    | Status             |" << endl;
    cout << "+---------------+-------------------------+--------------------+--------------------+" << endl;

    bool hasData = false;

    while (db.result->next())
    {
        hasData = true;
        string RoomID = db.result->getString("RoomID");
        string Type = db.result->getString("Type");
        double Price = db.result->getDouble("Price");
        int activeReservations = db.result->getInt("activeReservations");

        string status = (activeReservations > 0) ? "Unavailable" : "Available";

        cout << "| " << setw(13) << left << RoomID << " | "
            << setw(23) << left << Type << " | "
            << setw(18) << left << Price << " | "
            << setw(18) << left << status << " |" << endl;

        cout << "+---------------+-------------------------+--------------------+--------------------+" << endl;
    }

    if (!hasData)
    {
        cout << "| No data available.                                                     |" << endl;
        cout << "+---------------+-------------------------+--------------------+--------------------+" << endl;
    }

    while (true)
    {
        cout << "1.Make a reservation\n2.Return to menu\n";
        cout << "Enter your selection: ";
        getline(cin, input);

        if (input.empty())
        {
            cout << "Input cannot be empty. Please enter a valid number (1, 2, or 3).\n";
            continue;
        }

        try
        {
            selection = stoi(input);

            if (selection < 1 || selection > 2)
            {
                cout << "Invalid input. Please enter a number between 1 or 2.\n";
                continue;
            }
            break;
        }
        catch (const invalid_argument&)
        {
            cout << "Invalid input. Please enter a valid number (1 or 2).\n";
        }
    }
    if (selection == 1)
    {
        make.ReserveRoom();
        return;
    }

    else if (selection == 2)
    {
        system("cls");
        menu.GuestMenu();
    }
}

void guest::ReserveRoom()
{
    string RoomID, DateIn, DateOut, IC, GuestID;
    char option{};

    bool hasData = false;
    bool validIC = false;

    while (!validIC)
    {
        cout << "Identification number (xxxxxx-xx-xxxx): ";
        getline(cin, IC);
        regex IC_regex("^\\d{6}-\\d{2}-\\d{4}$");
        if (regex_match(IC, IC_regex))
        {
            db.prepareStatement("SELECT GuestID, UserID FROM hotel_guest WHERE IC = ?;");
            db.statement->setString(1, IC);
            db.QueryResult();

            if (db.result->next())
            {
                GuestID = db.result->getString("GuestID");
                string UserID = db.result->getString("UserID");

                string currentUserID = Pass.GetUserID();

                if (UserID != currentUserID)
                {

                    cout << "Error: The IC number entered does not belong to the logged-in user.\nPlease try again.\n";
                    continue;
                }
                else
                {
                    cout << "GuestID: " << GuestID << " verified successfully!\n";
                    validIC = true;
                    hasData = true;
                    break;
                }
            }
            else
            {
                cout << "Error: Guest not found for the given IC number.\nPlease try again.\n";
                continue;
            }
        }
        else
        {
            cout << "Invalid IC number format. Please follow the IC format.\n";
        }
    }

    if (!hasData)
    {
        cout << "Unable to verify guest.\n";
        menu.GuestMenu();
        return;
    }

    do
    {
        while (true)
        {
            cout << "Enter the RoomID you wish to reserve: ";
            getline(cin, RoomID);
            if (RoomID.empty())
            {
                cout << "RoomID cannot be empty. Please try again.\n";
                continue;
            }
            db.prepareStatement("SELECT COUNT(*) AS RoomCount FROM room WHERE RoomID = ?");
            db.statement->setString(1, RoomID);
            db.QueryResult();

            if (db.result->next() && db.result->getInt("RoomCount") > 0)
            {

                cout << "Enter Check-In Date (YYYY-MM-DD): ";
                getline(cin, DateIn);                
                if (!isValidDateFormat(DateIn))
                {
                    cout << "Invalid Check-In Date format. Please enter a date in YYYY-MM-DD format.\n";
                    continue;
                }

                if (!isTodayOrFuture(DateIn))
                {
                    cout << "Check-In Date must be today or in the future.\n";
                    continue;
                }

                cout << "Enter Check-Out Date (YYYY-MM-DD): ";
                getline(cin, DateOut);

                if (!isValidDateFormat(DateOut))
                {
                    cout << "Invalid Check-Out Date format. Please enter a date in YYYY-MM-DD format.\n";
                    continue;
                }

                if (!isTodayOrFuture(DateOut))
                {
                    cout << "Check-Out Date must be today or in the future.\n";
                    continue;
                }

                if (DateOut <= DateIn)
                {
                    cout << "Check-Out Date must be later than Check-In Date.\n";
                    continue;
                }
                if (!isRoomAvailable(db, RoomID, DateIn, DateOut))
                {
                    cout << "Room is not available for the selected dates. Please choose a different room or adjust your dates.\n";
                    continue;
                }
            }
            else
            {
                cout << "Error: RoomID " << RoomID << " is invalid or not available. Please try again.\n";
                continue;
            }
            break;

        }

        db.prepareStatement("INSERT INTO reservation(RoomID, GuestID, DateIn, DateOut) VALUES (?, ?, ?, ?);");
        db.statement->setString(1, RoomID);
        db.statement->setString(2, GuestID);
        db.statement->setString(3, DateIn);
        db.statement->setString(4, DateOut);
        db.QueryResult();

        cout << "Reservation for Room " << RoomID << " successfully added.\n";

        do
        {
            cout << "Would you like to reserve another room? (Y/N): ";
            cin >> option;

            if (option != 'Y' && option != 'y' && option != 'N' && option != 'n')
            {
                cout << "Invalid input. Please enter 'Y' for Yes or 'N' for No.\n";
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } while (option != 'Y' && option != 'y' && option != 'N' && option != 'n');

        if (option == 'N' || option == 'n')
        {
            system("pause");
            system("cls");
            menu.GuestMenu();
            return;
        }

    } while (option == 'Y' || option == 'y');

    system("cls");
    ViewRoom();
}

void guest::GenerateReceipt(const string& reservationID, double amountPaid)
{
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    tm timeStruct;

    if (localtime_s(&timeStruct, &now_c) == 0)
    {
        cout << "\n===== Receipt =====\n";
        cout << "Reservation ID: " << reservationID << endl;
        cout << "Amount Paid: RM" << amountPaid << endl;

        stringstream dateStream;
        dateStream << put_time(&timeStruct, "%Y-%m-%d %H:%M:%S");
        string formattedDate = dateStream.str();

        cout << "Payment Date: " << formattedDate << endl;
        cout << "===================\n";

        string currentUserID = pass::GetUserID();

        string guestID;
        db.prepareStatement("SELECT GuestID FROM hotel_guest WHERE UserID = ?");
        db.statement->setString(1, currentUserID);
        db.QueryResult();

        if (db.result->next()) 
        {
            guestID = db.result->getString("GuestID");
        }
        else 
        {
            cerr << "Error: UserID " << currentUserID << " does not exist in hotel_guest table.\n";
            return;
        }

        db.prepareStatement("INSERT INTO invoice (Date, Payment_Amount, GuestID) VALUES (?, ?, ?)");
        db.statement->setString(1, formattedDate);
        db.statement->setDouble(2, amountPaid);
        db.statement->setString(3, guestID);
        db.QueryStatement();

        cout << "Invoice successfully saved in the database.\n";
    }
    else
    {
        cerr << "Error: Failed to retrieve local time." << endl;
    }
}

void guest::MakePayment()
{
    system("cls");
    bool hasReservations = false;
    double totalAmount = 0.0, paymentAmount = 0.0, balance = 0.0;
    string reservationID, paymentAmountStr, choice;

    try
    {
        string currentUserID = pass::GetUserID();
        if (currentUserID.empty())
        {
            cout << "Error: No user is currently logged in.\n";
            return;
        }

        db.prepareStatement("SELECT GuestID FROM hotel_guest WHERE UserID = ?");
        db.statement->setString(1, currentUserID);
        db.QueryResult();

        if (db.result->next())
        {
            GuestID = db.result->getString("GuestID");
            cout << "GuestID retrieved: " << GuestID << endl;
        }
        else
        {
            cout << "Error: No guest record found for the logged-in user.\n";
            return;
        }

        while (true)
        {
            cout << "\nActive reservations (GuestID: " << GuestID << "):\n";
            cout << left << setw(15) << "Reservation ID" << setw(15) << "Room Price" << setw(15) << "Total Amount" << endl;
            cout << string(45, '-') << endl;

            db.prepareStatement("SELECT res.ReservationID, r.Price, DATEDIFF(res.DateOut, res.DateIn) AS nights "
                "FROM reservation res JOIN room r ON res.RoomID = r.RoomID "
                "WHERE res.GuestID = ? AND res.Payment_status != 'Paid'");
            db.statement->setString(1, GuestID);
            db.QueryResult();

            bool hasUnpaidReservations = false;

            cout << "+---------------+---------------+---------------+" << endl;
            cout << "| ReservationID | Price/Night   | Total Amount  |" << endl;
            cout << "+---------------+---------------+---------------+" << endl;

            while (db.result->next())
            {
                hasUnpaidReservations = true;
                string reservationID = db.result->getString("ReservationID");
                double pricePerNight = db.result->getDouble("Price");
                int nights = db.result->getInt("nights");
                double totalAmount = pricePerNight * nights;

                cout << "| " << setw(13) << left << reservationID << " | "
                    << setw(13) << left << pricePerNight << " | "
                    << setw(13) << left << totalAmount << " |" << endl;

                cout << "+---------------+---------------+---------------+" << endl;
            }

            if (!hasUnpaidReservations)
            {
                cout << "\nNo unpaid reservations found.\n";
                system("pause");
                system("cls");
                menu.GuestMenu();
                break;
            }

            while (true) 
            {
                cout << "\nEnter the Reservation ID you wish to pay: ";
                getline(cin, reservationID);

                if (reservationID.empty()) 
                {
                    cout << "Error: Reservation ID cannot be empty. Please try again.\n";
                    continue;
                }

                db.prepareStatement("SELECT r.Price, DATEDIFF(res.DateOut, res.DateIn) AS nights "
                    "FROM reservation res JOIN room r ON res.RoomID = r.RoomID "
                    "WHERE res.ReservationID = ? AND res.GuestID = ? AND res.Payment_status != 'Paid'");
                db.statement->setString(1, reservationID);
                db.statement->setString(2, GuestID);
                db.QueryResult();

                if (db.result->next()) 
                {
                    double pricePerNight = db.result->getDouble("Price");
                    int nights = db.result->getInt("nights");
                    totalAmount = pricePerNight * nights;
                    cout << "Total amount due for Reservation ID " << reservationID << ": RM " << totalAmount << "\n";
                    break;
                }
                else 
                {
                    cout << "Error: Invalid or already paid Reservation ID. Please try again.\n";
                }
            }

            while (true) 
            {
                cout << "Enter the payment amount (RM): ";
                getline(cin, paymentAmountStr);

                if (paymentAmountStr.empty()) 
                {
                    cout << "Error: Payment amount cannot be empty. Please try again.\n";
                    continue;
                }

                try {
                    paymentAmount = stod(paymentAmountStr);
                    if (paymentAmount <= 0) 
                    {
                        cout << "Error: Payment amount must be greater than 0. Please try again.\n";
                        continue;
                    }
                    if (paymentAmount < totalAmount) 
                    {
                        cout << "Error: Payment amount is not enough to cover the total. Please enter at least RM " << totalAmount << ".\n";
                        continue;
                    }
                    break;
                }
                catch (const invalid_argument&) 
                {
                    cout << "Error: Invalid input. Please enter a valid number for the payment amount.\n";
                }
                catch (const out_of_range&) 
                {
                    cout << "Error: Payment amount is too large. Please try again.\n";
                }
            }

            while (true) 
            {
                cout << "Confirm payment of RM " << paymentAmount << " for Reservation ID " << reservationID << "? (Y/N): ";
                getline(cin, choice);

                if (choice.empty()) {
                    cout << "Error: Input cannot be empty. Please enter 'Y' for Yes or 'N' for No.\n";
                    continue;
                }

                if (choice == "Y" || choice == "y") 
                {
                    if (paymentAmount > totalAmount)
                    {
                        double balance = paymentAmount - totalAmount; 
                        cout << "\nPayment amount exceeds the total amount needed to be pay, return balance: RM" << balance << endl;
                    }
                    db.prepareStatement("UPDATE reservation SET Payment_status = 'Paid' WHERE ReservationID = ?");
                    db.statement->setString(1, reservationID);
                    db.QueryStatement();

                    cout << "Payment of RM " << paymentAmount << " for Reservation ID " << reservationID << " is successful!\n";
                    GenerateReceipt(reservationID, paymentAmount);
                    break;
                }
                else if (choice == "N" || choice == "n") {
                    cout << "Payment canceled.\n";
                    break;
                }
                else {
                    cout << "Error: Invalid input. Please enter 'Y' for Yes or 'N' for No.\n";
                }
            }

            cout << "\nWould you like to pay for another reservation? (Y/N): ";
            getline(cin, choice);

            if (choice == "N" || choice == "n") {
                cout << "Returning to the main menu.\n";
                system("pause");
                system("cls");
                menu.GuestMenu();
                break;
            }
        }
    }
    catch (const exception& e) {
        cout << "An error occurred: " << e.what() << endl;
    }
}

void guest::Profile()
{
    int Age;
    bool hasData = false;
    string UserID = Pass.GetUserID();

    system("cls");
    if (UserID.empty()) 
    {
        cout << "You are not logged in. Please log in first.\n";
        return;
    }

    try 
    {
        db.prepareStatement("SELECT * FROM hotel_guest h INNER JOIN user u ON h.UserID = u.UserID WHERE u.UserID = ?");
        db.statement->setString(1, UserID);
        db.QueryResult();

        if (db.result->next())
        {
            cout << "+----------------+--------------------------------+" << endl;
            cout << "| Field          | Value                          |" << endl;
            cout << "+----------------+--------------------------------+" << endl;

            cout << "| Name           | " << setw(30) << left << db.result->getString("Name") << "|" << endl;
            cout << "+----------------+--------------------------------+" << endl;

            cout << "| IC Number      | " << setw(30) << left << db.result->getString("IC") << "|" << endl;
            cout << "+----------------+--------------------------------+" << endl;

            cout << "| Age            | " << setw(30) << left << db.result->getInt("Age") << "|" << endl;
            cout << "+----------------+--------------------------------+" << endl;

            cout << "| Phone Number   | " << setw(30) << left << db.result->getString("Contact_No") << "|" << endl;
            cout << "+----------------+--------------------------------+" << endl;

            cout << "| Email Address  | " << setw(30) << left << db.result->getString("Email") << "|" << endl;
            cout << "+----------------+--------------------------------+" << endl;

            cout << "| User ID        | " << setw(30) << left << db.result->getString("UserID") << "|" << endl;
            cout << "+----------------+--------------------------------+" << endl;

            cout << "| Username       | " << setw(30) << left << db.result->getString("Username") << "|" << endl;
            cout << "+----------------+--------------------------------+" << endl;

            cout << "| Password       | " << setw(30) << left << db.result->getString("Password") << "|" << endl;
            cout << "+----------------+--------------------------------+" << endl;

            while (true)
            {
                cout << "1.Update Guest Name\n";
                cout << "2.Update Age\n";
                cout << "3.Update Phone number\n";
                cout << "4.Update Email\n";
                cout << "5.Return menu\n";
                cout << "\nEnter your choice:";
                getline(cin, input);

                if (input.empty())
                {
                    cout << "Input cannot be empty. Please enter a valid number (1, 2, 3, 4, or 5).\n";
                    continue;
                }

                try
                {
                    choice = stoi(input);

                    if (choice < 1 || choice > 5)
                    {
                        cout << "Invalid choice. Please enter a number between 1 and 5.\n";
                        continue;
                    }
                    break;
                }
                catch (const invalid_argument&)
                {
                    cout << "Invalid input. Please enter a valid number (1, 2, 3, 4, or 5).\n";
                }
            }

            switch (choice)
            {
            case 1:
                    system("cls");
                    while (true)
                    {
                        cout << "Enter New Guest Name: ";
                        getline(cin, Name);

                        if (Name.empty())
                        {
                            cout << "Name cannot be empty! Please enter a valid name.\n";
                            continue;
                        }

                        db.prepareStatement("SELECT COUNT(*) AS count FROM hotel_guest WHERE Name = ?");
                        db.statement->setString(1, Name);
                        db.QueryResult();

                        if (db.result->next() && db.result->getInt("count") > 0)
                        {
                            cout << "Error: This name is already registered. Please use a different name.\n";
                            continue;
                        }

                        break;
                    }

                    db.prepareStatement("UPDATE hotel_guest SET Name = ? WHERE UserID = ?");
                    db.statement->setString(1, Name);
                    db.statement->setString(2, UserID);
                    db.QueryStatement();

                    system("cls");
                    cout << "Update Successful!\n";

                    db.prepareStatement("SELECT * FROM hotel_guest h INNER JOIN user u ON h.UserID = u.UserID WHERE u.UserID = ?");
                    db.statement->setString(1, UserID);
                    db.QueryResult();

                    while (db.result->next())
                    {
                        hasData = true;
                        cout << "+----------------+--------------------------------+" << endl;
                        cout << "| Field          | Value                          |" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Name           | " << setw(30) << left << db.result->getString("Name") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| IC Number      | " << setw(30) << left << db.result->getString("IC") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Age            | " << setw(30) << left << db.result->getInt("Age") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Phone Number   | " << setw(30) << left << db.result->getString("Contact_No") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Email Address  | " << setw(30) << left << db.result->getString("Email") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| User ID        | " << setw(30) << left << db.result->getString("UserID") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Username       | " << setw(30) << left << db.result->getString("Username") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Password       | " << setw(30) << left << db.result->getString("Password") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;
                    }

                    if (!hasData)
                    {
                        cout << "No data found matching the criteria." << endl;
                    }
                    
                    system("pause");
                    system("cls");
                    Profile();
                    return;

            case 2:
                    system("cls");
                    while (true)
                    {
                        string input;
                        cout << "Age: ";
                        getline(cin, input);

                        if (input.empty())
                        {
                            cout << "Age cannot be empty. Please enter a valid Age.\n";
                            continue;
                        }

                        try
                        {
                            Age = stoi(input);

                            if (Age <= 0 || Age >= 122)
                            {
                                cout << "Invalid age. Please enter a valid age between 1 and 121.\n";
                            }
                            else
                            {
                                break;
                            }
                        }
                        catch (const invalid_argument& e)
                        {
                            cout << "Invalid input. Please enter a valid Age (numeric value).\n";
                        }
                        catch (const out_of_range& e)
                        {
                            cout << "Age is out of valid range. Please enter a valid Age.\n";
                        }
                    }

                    db.prepareStatement("UPDATE hotel_guest SET Age = ? WHERE UserID = ?");
                    db.statement->setInt(1, Age);
                    db.statement->setString(2, UserID);
                    db.QueryStatement();

                    system("cls");
                    cout << "Update Successful!\n";

                    db.prepareStatement("SELECT * FROM hotel_guest h INNER JOIN user u ON h.UserID = u.UserID WHERE u.UserID = ?");
                    db.statement->setString(1, UserID);
                    db.QueryResult();

                    while (db.result->next())
                    {
                        hasData = true;
                        cout << "+----------------+--------------------------------+" << endl;
                        cout << "| Field          | Value                          |" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Name           | " << setw(30) << left << db.result->getString("Name") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| IC Number      | " << setw(30) << left << db.result->getString("IC") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Age            | " << setw(30) << left << db.result->getInt("Age") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Phone Number   | " << setw(30) << left << db.result->getString("Contact_No") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Email Address  | " << setw(30) << left << db.result->getString("Email") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| User ID        | " << setw(30) << left << db.result->getString("UserID") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Username       | " << setw(30) << left << db.result->getString("Username") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Password       | " << setw(30) << left << db.result->getString("Password") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;
                    }

                    if (!hasData)
                    {
                        cout << "No data found matching the criteria." << endl;
                    }
                    system("pause");
                    system("cls");
                    Profile();
                    return;

            case 3:
                    system("cls");
                    while (true)
                    {
                        cout << "Enter new phone number (000-0000000 or 000-00000000): ";
                        getline(cin, Contact_No);
                        regex phone_regex("^01[0-9]-\\d{7,8}$");

                        if (regex_match(Contact_No, phone_regex))
                        {
                            db.prepareStatement("SELECT COUNT(*) AS count FROM hotel_guest WHERE Contact_No = ?");
                            db.statement->setString(1, Contact_No);
                            db.QueryResult();

                            if (db.result->next())
                            {
                                int count = db.result->getInt("count");
                                if (count > 0)
                                {
                                    cout << "This phone number has already been registered. Please try again.\n";
                                    continue;
                                }
                                else
                                {
                                    break;
                                }
                            }
                            else
                            {
                                cout << "Error querying the database. Please try again.\n";
                                continue;
                            }
                        }
                        else
                        {
                            cout << "Invalid phone number format. Please try again.\n";
                        }
                    }

                    db.prepareStatement("UPDATE hotel_guest SET Contact_No = ? WHERE GuestID = ?");
                    db.statement->setString(1, Contact_No);
                    db.statement->setString(2, GuestID);
                    db.QueryStatement();

                    system("cls");
                    cout << "Update Successful!\n";

                    db.prepareStatement("SELECT * FROM hotel_guest h INNER JOIN user u ON h.UserID = u.UserID WHERE u.UserID = ?");
                    db.statement->setString(1, UserID);
                    db.QueryResult();

                    while (db.result->next())
                    {
                        hasData = true;
                        cout << "+----------------+--------------------------------+" << endl;
                        cout << "| Field          | Value                          |" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Name           | " << setw(30) << left << db.result->getString("Name") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| IC Number      | " << setw(30) << left << db.result->getString("IC") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Age            | " << setw(30) << left << db.result->getInt("Age") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Phone Number   | " << setw(30) << left << db.result->getString("Contact_No") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Email Address  | " << setw(30) << left << db.result->getString("Email") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| User ID        | " << setw(30) << left << db.result->getString("UserID") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Username       | " << setw(30) << left << db.result->getString("Username") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Password       | " << setw(30) << left << db.result->getString("Password") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;
                    }

                    if (!hasData)
                    {
                        cout << "No data found matching the criteria." << endl;
                    }
                    system("pause");
                    system("cls");
                    Profile();
                    return;

            case 4:
                    system("cls");
                    while (true)
                    {
                        cout << "Email: ";
                        getline(cin, Email);
                        regex email_regex("^[\\w.%+-]+@[\\w.-]+\\.[a-zA-Z]{2,}$");

                        if (regex_match(Email, email_regex))
                        {
                            db.prepareStatement("SELECT COUNT(*) AS count FROM hotel_guest WHERE Email = ?");
                            db.statement->setString(1, Email);
                            db.QueryResult();

                            if (db.result->next())
                            {
                                int count = db.result->getInt("count");
                                if (count > 0)
                                {
                                    cout << "This email has already been registered. Please try again.\n";
                                    continue;
                                }
                                else
                                {
                                    break;
                                }
                            }
                            else
                            {
                                cout << "Error querying the database. Please try again.\n";
                                continue;
                            }
                        }
                        else
                        {
                            cout << "Invalid email format. Please try again.\n";
                        }
                    }

                    db.prepareStatement("UPDATE hotel_guest SET Email = ? WHERE GuestID = ?");
                    db.statement->setString(1, Email);
                    db.statement->setString(2, GuestID);
                    db.QueryStatement();

                    system("cls");
                    cout << "Update Successful!\n";

                    db.prepareStatement("SELECT * FROM hotel_guest h INNER JOIN user u ON h.UserID = u.UserID WHERE u.UserID = ?");
                    db.statement->setString(1, UserID);
                    db.QueryResult();

                    while (db.result->next())
                    {
                        hasData = true;
                        cout << "+----------------+--------------------------------+" << endl;
                        cout << "| Field          | Value                          |" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Name           | " << setw(30) << left << db.result->getString("Name") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| IC Number      | " << setw(30) << left << db.result->getString("IC") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Age            | " << setw(30) << left << db.result->getInt("Age") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Phone Number   | " << setw(30) << left << db.result->getString("Contact_No") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Email Address  | " << setw(30) << left << db.result->getString("Email") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| User ID        | " << setw(30) << left << db.result->getString("UserID") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Username       | " << setw(30) << left << db.result->getString("Username") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;

                        cout << "| Password       | " << setw(30) << left << db.result->getString("Password") << "|" << endl;
                        cout << "+----------------+--------------------------------+" << endl;
                    }

                    if (!hasData)
                    {
                        cout << "No data found matching the criteria." << endl;
                    }
                    system("pause");
                    system("cls");
                    Profile();
                    return;

            case 5:
                cout << "\nReturning to the guest menu.";
                system("pause");
                system("cls");
                menu.GuestMenu();

            }
        }
        else 
        {
            cout << "Error retrieving profile details.\n";
        }
    }
    catch (const sql::SQLException& e) 
    {
        cerr << "Database error during profile retrieval: " << e.what() << endl;
    }
}
