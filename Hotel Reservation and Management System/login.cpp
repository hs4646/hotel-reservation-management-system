#include <iostream>
#include <iomanip>
#include <string>
#include <regex>
#include <conio.h>
#include <mysql/jdbc.h>
#include "mysql.h"
#include "Login.h"
#include "guest.h"
#include "passUserID.h"
#include "Menu.h"


using namespace std;

void Login::LoginMenu()
{
	mySQL_db db;
	Menu menu;
	Login login;
	string Username, UserType, Password, Name, Contact_No, Email, UserID, ManagerName, ManagerPass, IC, input;
	char ch;
	int Age, mode, maxLoginAttempts = 3, attemptCount = 0, choice = 0;
	bool loginSuccess = false;

	while (true)
	{
		cout << "1.Login\n2.Register\n3.Exit program\nEnter your selection: ";
		getline(cin, input);

		if (input.empty())
		{
			cout << "Input cannot be empty. Please enter a valid number (1, 2, or 3).\n";
			continue;
		}

		try
		{
			choice = stoi(input);

			if (choice < 1 || choice > 3)
			{
				cout << "Invalid choice. Please enter a number between 1 and 3.\n";
				continue;
			}
			break;
		}
		catch (const invalid_argument&)
		{
			cout << "Invalid input. Please enter a valid number (1, 2, or 3).\n";
		}
	}

	if (choice == 1)
	{
		system("cls");
		cout << "\033[1;34m--------------------------------------------------------------\033[0m\n";
		cout << "\033[1;33m  \nLogin Page\033[0m\n\n";
		cout << "\033[1;34m--------------------------------------------------------------\033[0m\n";
		while (true)
		{
			cout << "1.User Mode\n2.Manager Mode\n3.Exit\nEnter your mode: ";
			getline(cin, input);

			if (input.empty())
			{
				cout << "Input cannot be empty. Please enter a valid number (1, 2, or 3).\n";
				continue;
			}

			try
			{
				mode = stoi(input);

				if (mode < 1 || mode > 3)
				{
					cout << "Invalid mode. Please enter a number between 1 and 3.\n";
					continue;
				}
				break;
			}
			catch (const invalid_argument&)
			{
				cout << "Invalid input. Please enter a valid number (1, 2, or 3).\n";
			}
		}

		if (mode == 1)
		{
			system("cls");
			cout << "\033[1;34m--------------------------------------------------------------\033[0m\n";
			cout << "\033[1;33m  \nUser Mode\033[0m\n\n";
			cout << "\033[1;34m--------------------------------------------------------------\033[0m\n";
			while (!loginSuccess && attemptCount < maxLoginAttempts) 
			{
				cout << "Enter your username: ";
				getline(cin, Username);

				cout << "Enter your password: ";
				Password.clear();

				while (true)
				{
					ch = _getch();
					if (ch == '\r') break;
					if (ch == '\b' && !Password.empty()) {
						cout << "\b \b";
						Password.pop_back();
					}
					else if (ch != '\b')
					{
						Password += ch;
						cout << '*';
					}
				}

				db.prepareStatement("SELECT * FROM user WHERE Username = ? AND Password = ?");
				db.statement->setString(1, Username);
				db.statement->setString(2, Password);
				db.QueryResult();

				if (db.result->next())
				{
					UserID = db.result->getString("UserID");
					UserType = db.result->getString("UserType");
					pass::SetUserID(UserID);
					loginSuccess = true;

					if (UserType == "guest")
					{
						system("cls");
						menu.GuestMenu();
					}

					else
					{
						system("cls");
						menu.StaffMenu();
					}
				}
				else
				{
					attemptCount++;
					cout << "\nInvalid username or password. Attempt " << attemptCount << " of " << maxLoginAttempts << endl;
					system("pause");
				}
			}

			if (!loginSuccess && attemptCount == maxLoginAttempts)
			{
				cout << "\nMaximum login attempts reached. Redirecting to password recovery.\n";
				system("cls");
				menu.ForgetPass();
				return;
			}
		}

		else if (mode == 2)
		{
			system("cls");
			cout << "\033[1;34m--------------------------------------------------------------\033[0m\n";
			cout << "\033[1;33m  \nManager Mode\033[0m\n\n";
			cout << "\033[1;34m--------------------------------------------------------------\033[0m\n";
			while (!loginSuccess && attemptCount < maxLoginAttempts)
			{
				cout << "Enter manager's name: ";
				getline(cin, ManagerName);

				cout << "Enter password: ";
				ManagerPass.clear();

				while (true)
				{
					ch = _getch();
					if (ch == '\r') break;
					if (ch == '\b' && !ManagerPass.empty()) {
						cout << "\b \b";
						ManagerPass.pop_back();
					}
					else if (ch != '\b')
					{
						ManagerPass += ch;
						cout << '*';
					}
				}

				db.prepareStatement("SELECT * FROM hotel_management WHERE ManagerName = ? AND ManagerPass = ?");
				db.statement->setString(1, ManagerName);
				db.statement->setString(2, ManagerPass);
				db.QueryResult();

				if (db.result->next())
				{
					system("cls");
					menu.ManagementMenu();
				}
				else
				{
					attemptCount++;
					cout << "\nInvalid name or password. Attempt " << attemptCount << " of " << maxLoginAttempts << endl;
					system("pause");
				}
			}

			if (!loginSuccess && attemptCount == maxLoginAttempts)
			{
				cout << "\nMaximum login attempts reached. You are not able to access Manager mode.\n";
				return;
			}
		}
	}

	else if (choice == 2)
	{
		system("cls");
		cout << "\033[1;34m--------------------------------------------------------------\033[0m\n";
		cout << "\033[1;33m  \nRegistration Page\033[0m\n\n";
		cout << "\033[1;34m--------------------------------------------------------------\033[0m\n";
		while (true)
		{
			cout << "Username: ";
			getline(cin, Username);

			if (!Username.empty())
			{
				db.prepareStatement("SELECT COUNT(*) AS count FROM user WHERE Username = ?");
				db.statement->setString(1, Username);
				db.QueryResult();

				if (db.result->next() && db.result->getInt("count") > 0)
				{
					cout << "Error: This username is already taken. Try another.\n";
				}
				else
				{
					break;
				}
			}
			else
			{
				cout << "Username cannot be empty. Please try again.\n";
			}
		}

		while (true)
		{
			cout << "Password: ";
			Password.clear();

			while (true)
			{
				ch = _getch();
				if (ch == '\r') break;
				if (ch == '\b' && !Password.empty()) {
					cout << "\b \b";
					Password.pop_back();
				}
				else if (ch != '\b')
				{
					Password += ch;
					cout << '*';
				}
			}
			if (!Password.empty()) break;
			cout << "Password cannot be empty. Please try again.\n";
		}
		db.prepareStatement("INSERT INTO user (Username, Password, UserType) VALUES (?, ?, 'guest')");
		db.statement->setString(1, Username);
		db.statement->setString(2, Password);
		db.QueryStatement();
		db.prepareStatement("SELECT UserID FROM user WHERE Username = ?");
		db.statement->setString(1, Username);
		db.QueryResult();

		if (db.result->next()) {
			UserID = db.result->getString("UserID");
		}
		else {
			cout << "UserID not found. Registration failed.\n";
			return;
		}

		while (true)
		{
			cout << "\nYour full name: ";
			getline(cin, Name);

			if (Name.empty())
			{
				cout << "Name cannot be empty. Please enter your full name.";
				continue;
			}

			db.prepareStatement("SELECT COUNT(*) AS count FROM hotel_guest WHERE Name = ?");
			db.statement->setString(1, Name);
			db.QueryResult();

			if (db.result->next() && db.result->getInt("count") > 0)
			{
				cout << "This name already exists. Please use a different name.";
				continue;
			}
			break;
		}

		while (true)
		{
			cout << "IC (xxxxxx-xx-xxxx): ";
			getline(cin, IC);
			regex ic_regex("^\\d{6}-\\d{2}-\\d{4}$");

			if (regex_match(IC, ic_regex))
			{
				db.prepareStatement("SELECT COUNT(*) AS count FROM hotel_guest WHERE IC = ?");
				db.statement->setString(1, IC);
				db.QueryResult();

				if (db.result->next())
				{
					int count = db.result->getInt("count");
					if (count > 0)
					{
						cout << "This IC has already been registered. Please try again.\n";
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
				cout << "Invalid IC format. Please try again.\n";
			}
		}

		while (true)
		{
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

		while (true)
		{
			cout << "Phone Number (01x-xxxxxxx or 01x-xxxxxxxx): ";
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
				continue;
			}
		}

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

		db.prepareStatement("INSERT INTO hotel_guest (Name, IC, Age, Contact_No, Email, UserID) VALUES (?, ?, ?, ?, ?, ?)");
		db.statement->setString(1, Name);
		db.statement->setString(2, IC);
		db.statement->setInt(3, Age);
		db.statement->setString(4, Contact_No);
		db.statement->setString(5, Email);
		db.statement->setString(6, UserID);
		db.QueryStatement();

		cout << "Registration successful!\n";
		system("pause");
		system("cls");
		menu.MainMenu();
		LoginMenu();
		return;
	}

	else if (choice == 3)
	{
		cout << "Exiting the program. Goodbye!\n";
		exit(0);
	}
}

void Login::Reset()
{
	mySQL_db db;
	Menu menu;
	Login login;
	string Username, IC, NewUsername, NewPassword, ConfirmPassword, UserType, Contact_No, input;
	char ch;
	int choice;

	system("cls");
	cout << "\033[1;34m--------------------------------------------------------------\033[0m\n";
	cout << "\033[1;33m  \nReset Username/Password Page\033[0m\n\n";
	cout << "\033[1;34m--------------------------------------------------------------\033[0m\n";

	while (true)
	{
		cout << "\nWhat are your user type (guest/staff): ";
		getline(cin, UserType);

		if (UserType.empty())
		{
			cout << "Input cannot be empty. Please enter either 'guest' or 'staff'.\n";
			continue;
		}
		else if (UserType == "guest" || UserType == "staff")
		{
			break;
		}
		else
		{
			cout << "Invalid input. Please enter either 'guest' or 'staff'.\n";
		}
	}

	if (UserType == "guest")
	{
		while (true)
		{
			cout << "Phone Number (01x-xxxxxxx or 01x-xxxxxxxx): ";
			getline(cin, Contact_No);
			regex phone_regex("^01[0-9]-\\d{7,8}$");

			if (regex_match(Contact_No, phone_regex))
			{
				db.prepareStatement("SELECT Contact_No FROM hotel_guest WHERE Contact_No = ?");
				db.statement->setString(1, Contact_No);
				db.QueryResult();

				if (db.result->next())
				{
					break;
				}
				else
				{
					cout << "This phone number is invalid. Please try again.\n";
				}
			}
			else
			{
				cout << "Invalid phone number format. Please try again.\n";
			}
		}

		while (true)
		{
			cout << "Enter your IC to verify your account: ";
			getline(cin, IC);
			regex ic_regex("^\\d{6}-\\d{2}-\\d{4}$");

			if (regex_match(IC, ic_regex))
			{
				db.prepareStatement("SELECT IC FROM hotel_guest WHERE IC = ? AND Contact_No = ?");
				db.statement->setString(1, IC);
				db.statement->setString(2, Contact_No);
				db.QueryResult();

				if (db.result->next())
				{
					cout << "Access verified!\n";
					break;
				}
				else
				{
					cout << "The IC number is not associated with your account. Please try again.\n";
				}
			}
			else
			{
				cout << "Invalid IC format. Please try again.\n";
			}
		}

		cout << "\n";
		system("pause");
		system("cls");

		while (true)
		{
			cout << "1.Reset Username\n2.Reset Password\n3.Exit";
			cout << "\nEnter your choice:";
			getline(cin, input);

			if (input.empty())
			{
				cout << "Input cannot be empty. Please enter a valid number (1, 2, or 3).\n";
				continue;
			}

			try
			{
				choice = stoi(input);

				if (choice < 1 || choice > 3)
				{
					cout << "Invalid choice. Please enter a number between 1 and 3.\n";
					continue;
				}
				break;
			}
			catch (const invalid_argument&)
			{
				cout << "Invalid input. Please enter a valid number (1, 2, or 3).\n";
			}
		}

		if (choice == 1)
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			while (true)
			{
				cout << "Enter new Username: ";
				getline(cin, NewUsername);

				if (NewUsername.empty())
				{
					cout << "Username cannot be empty! Please enter a valid Username.\n";
					continue;
				}

				db.prepareStatement("SELECT COUNT(*) AS count FROM user WHERE Username = ?");
				db.statement->setString(1, NewUsername);
				db.QueryResult();

				if (db.result->next() && db.result->getInt("count") > 0)
				{
					cout << "Error: This username is already registered. Please use a different username.\n";
					continue;
				}

				break;
			}

			db.prepareStatement("UPDATE user SET Username = ? WHERE UserID = (SELECT UserID FROM hotel_guest WHERE Contact_No = ? AND IC = ?)");
			db.statement->setString(1, NewUsername);
			db.statement->setString(2, Contact_No);
			db.statement->setString(3, IC);
			db.QueryStatement();

			cout << "\nYour username has been successfully reset.\n";
			system("pause");
			system("cls");
			menu.MainMenu();
			LoginMenu();
		}

		else if (choice == 2)
		{
			while (true)
			{
				cout << "Enter a new password: ";
				NewPassword.clear();

				while (true)
				{
					ch = _getch();
					if (ch == '\r') break;
					if (ch == '\b' && !NewPassword.empty()) {
						cout << "\b \b";
						NewPassword.pop_back();
					}
					else if (ch != '\b')
					{
						NewPassword += ch;
						cout << '*';
					}
				}
				if (!NewPassword.empty()) break;
				cout << "Password cannot be empty. Please try again.\n";
			}

			while (true)
			{
				cout << "\nEnter again your new password to confirm: ";
				ConfirmPassword.clear();

				while (true)
				{
					ch = _getch();
					if (ch == '\r') break;
					if (ch == '\b' && !ConfirmPassword.empty()) {
						cout << "\b \b";
						ConfirmPassword.pop_back();
					}
					else if (ch != '\b')
					{
						ConfirmPassword += ch;
						cout << '*';
					}
				}
				if (!ConfirmPassword.empty()) break;

				cout << "Password cannot be empty. Please try again.\n";
				continue;

				if (NewPassword != ConfirmPassword)
				{
					cout << "\nPasswords do not match. Please try again.\n";
					continue;
				}
				break;
			}

			db.prepareStatement("UPDATE user SET Password = ? WHERE UserID = (SELECT UserID FROM hotel_guest WHERE Contact_No = ? AND IC = ?)");
			db.statement->setString(1, NewPassword);
			db.statement->setString(2, Contact_No);
			db.statement->setString(3, IC);
			db.QueryStatement();

			cout << "\nYour password has been successfully updated.\n";
			system("pause");
			system("cls");
			menu.MainMenu();
			LoginMenu();
		}

		else if (choice == 3)
		{
			cout << "Exiting the reset!\n";
			menu.MainMenu();
			login.LoginMenu();
			return;
		}
	}
	else if (UserType == "staff")
	{
		while (true)
		{
			cout << "Phone Number (01x-xxxxxxx or 01x-xxxxxxxx): ";
			getline(cin, Contact_No);
			regex phone_regex("^01[0-9]-\\d{7,8}$");

			if (regex_match(Contact_No, phone_regex))
			{
				db.prepareStatement("SELECT Contact_No FROM hotel_staff WHERE Contact_No = ?");
				db.statement->setString(1, Contact_No);
				db.QueryResult();

				if (db.result->next())
				{
					break;
				}
				else
				{
					cout << "This phone number is invalid. Please try again.\n";
				}
			}
			else
			{
				cout << "Invalid phone number format. Please try again.\n";
			}
		}

		while (true)
		{
			cout << "Enter your IC to verify your account: ";
			getline(cin, IC);
			regex ic_regex("^\\d{6}-\\d{2}-\\d{4}$");

			if (regex_match(IC, ic_regex))
			{
				db.prepareStatement("SELECT IC FROM hotel_staff WHERE IC = ? AND Contact_No = ?");
				db.statement->setString(1, IC);
				db.statement->setString(2, Contact_No);
				db.QueryResult();

				if (db.result->next())
				{
					cout << "Access verified!\n";
					break;
				}
				else
				{
					cout << "The IC number is not associated with your account. Please try again.\n";
				}
			}
			else
			{
				cout << "Invalid IC format. Please try again.\n";
			}
		}

		cout << "\n";
		system("pause");
		system("cls");

		while (true)
		{
			cout << "1.Reset Username\n2.Reset Password\n3.Exit reset";
			cout << "\nEnter your choice:";
			getline(cin, input);

			if (input.empty())
			{
				cout << "Input cannot be empty. Please enter a valid number (1, 2, or 3).\n";
				continue;
			}

			try
			{
				choice = stoi(input);

				if (choice < 1 || choice > 3)
				{
					cout << "Invalid choice. Please enter a number between 1 and 3.\n";
					continue;
				}
				break;
			}
			catch (const invalid_argument&)
			{
				cout << "Invalid input. Please enter a valid number (1, 2, or 3).\n";
			}
		}

		if (choice == 1)
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			while (true)
			{
				cout << "Enter new Username: ";
				getline(cin, NewUsername);

				if (NewUsername.empty())
				{
					cout << "Username cannot be empty! Please enter a valid Username.\n";
					continue;
				}

				db.prepareStatement("SELECT COUNT(*) AS count FROM user WHERE Username = ?");
				db.statement->setString(1, NewUsername);
				db.QueryResult();

				if (db.result->next() && db.result->getInt("count") > 0)
				{
					cout << "Error: This username is already registered. Please use a different username.\n";
					continue;
				}

				break;
			}

			db.prepareStatement("UPDATE user SET Username = ? WHERE UserID = (SELECT UserID FROM hotel_staff WHERE Contact_No = ? AND IC = ?)");
			db.statement->setString(1, NewUsername);
			db.statement->setString(2, Contact_No);
			db.statement->setString(3, IC);
			db.QueryStatement();

			cout << "\nYour username has been successfully reset.\n";
			system("pause");
			system("cls");
			menu.MainMenu();
			LoginMenu();
		}

		else if (choice == 2)
		{
			while (true)
			{
				cout << "Enter a new password: ";
				NewPassword.clear();

				while (true)
				{
					ch = _getch();
					if (ch == '\r') break;
					if (ch == '\b' && !NewPassword.empty()) {
						cout << "\b \b";
						NewPassword.pop_back();
					}
					else if (ch != '\b')
					{
						NewPassword += ch;
						cout << '*';
					}
				}
				if (!NewPassword.empty()) break;
				cout << "Password cannot be empty. Please try again.\n";
			}

			while (true)
			{
				cout << "\nEnter again your new password to confirm: ";
				ConfirmPassword.clear();

				while (true)
				{
					ch = _getch();
					if (ch == '\r') break;
					if (ch == '\b' && !ConfirmPassword.empty()) {
						cout << "\b \b";
						ConfirmPassword.pop_back();
					}
					else if (ch != '\b')
					{
						ConfirmPassword += ch;
						cout << '*';
					}
				}
				if (!ConfirmPassword.empty()) break;

				cout << "Password cannot be empty. Please try again.\n";
				continue;

				if (NewPassword != ConfirmPassword)
				{
					cout << "\nPasswords do not match. Please try again.\n";
					continue;
				}
				break;
			}

			db.prepareStatement("UPDATE user SET Password = ? WHERE UserID = (SELECT UserID FROM hotel_staff WHERE Contact_No = ? AND IC = ?)");
			db.statement->setString(1, NewPassword);
			db.statement->setString(2, Contact_No);
			db.statement->setString(3, IC);
			db.QueryStatement();

			cout << "\nYour password has been successfully updated.\n";
			system("pause");
			system("cls");
			menu.MainMenu();
			LoginMenu();
		}

		else if (choice == 3)
		{
			cout << "Exiting the reset!\n";
			menu.MainMenu();
			login.LoginMenu();
			return;
		}
	}
}
