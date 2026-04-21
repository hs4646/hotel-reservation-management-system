#include <iostream>
#include <mysql/jdbc.h>
#include <conio.h>
#include "mysql.h"
#include "manager.h"
#include "menu.h"
#include <iomanip>
#include <string>
#include <regex>

using namespace std;

void manager::ManageStaff()
{
	mySQL_db db;
	manager Manage;
	Menu menu;
	string Username, Password, Name, Contact_No, Email,Address, Nationality, UserID, StaffName, ManagerName, ManagerPass, UserType, StaffID, IC, input;
	int Age, selection,choice;
	char option;
	bool StaffIDValid = false;
	bool hasData = false;
	bool validInput = false;

	system("cls");
	while (true)
	{
		cout << "Staff Management:\n1.Add New Staff\n2.Update Staff details\n3.Delete Staff\n4.Search Staff\n5.Return to main manager menu";
		cout << "\nEnter your selection:";
		getline(cin, input);

		if (input.empty())
		{
			cout << "Input cannot be empty. Please enter a valid number (1, 2, 3, 4, or 5).\n";
			continue;
		}

		try
		{
			selection = stoi(input);

			if (selection < 1 || selection > 5)
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

	if (selection == 1)
	{
		system("cls");
		cout << "---------------Registration for new staff user---------------" << endl;
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
				char ch;
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
		cout << "\n-------------------------------------------------------------";

		db.prepareStatement("INSERT INTO user (Username, Password, UserType) VALUES (?, ?, 'Staff')");
		db.statement->setString(1, Username);
		db.statement->setString(2, Password);
		db.QueryStatement();

		while (true)
		{
			cout << "\nThe staff full name: ";
			getline(cin, StaffName);

			if (StaffName.empty())
			{
				cout << "Name cannot be empty. Please enter staff full name.\n";
				continue;
			}

			db.prepareStatement("SELECT COUNT(*) AS count FROM hotel_staff WHERE StaffName = ?");
			db.statement->setString(1, StaffName);
			db.QueryResult();

			if (db.result->next() && db.result->getInt("count") > 0)
			{
				cout << "This name already exists. Please use a different name.\n";
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
		while (true)
		{
			cout << "Address: ";
			getline(cin, Address);

			if (Address.empty())
			{
				cout << "Address cannot be empty. Please enter a valid address.\n";
				continue;
			}

			break;
		}


		while (true)
		{
			cout << "Phone Number (01x-xxxxxxx or 01x-xxxxxxxx): ";
			getline(cin, Contact_No);
			regex phone_regex("^01[0-9]-\\d{7,8}$");
			if (Contact_No.empty())
			{
				cout << "Phone number cannot be empty. Please enter a valid number.\n";
				continue;
			}

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

		while (true)
		{
			cout << "Email: ";
			getline(cin, Email);
			regex email_regex("^[\\w.%+-]+@[\\w.-]+\\.[a-zA-Z]{2,}$");
			if (Email.empty())
			{
				cout << "Email cannot be empty. Please enter a valid email.\n";
				continue;
			}
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
			break;
		}
			db.prepareStatement("SELECT * FROM user ORDER BY UserID DESC LIMIT 1");
			db.QueryResult();
			db.result->next();
			UserID = db.result->getString("UserID");

			db.prepareStatement("INSERT INTO hotel_staff (StaffName, IC, Age, Address, Contact_No, Email, UserID) VALUES (?, ?, ?, ?, ?, ?, ?)");
			db.statement->setString(1, StaffName);
			db.statement->setString(2, IC);
			db.statement->setInt(3, Age);
			db.statement->setString(4, Address);
			db.statement->setString(5, Contact_No);
			db.statement->setString(6, Email);
			db.statement->setString(7, UserID);
			db.QueryStatement();

			cout << "Staff Registration Successful!\n";
			system("pause");
			system("cls");
			menu.ManagementMenu();
	}

	else if (selection == 2)
	{
		system("cls");

		db.prepareStatement("SELECT * FROM hotel_staff");
		db.QueryResult();

		cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
		cout << "| StaffID       | Staff Name              | IC Number          | Age      | Address            | Phone Number                           | Email Address      | UserID                       |" << endl;
		cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;

		bool hasData = false;

		while (db.result->next())
		{
			hasData = true;

			string StaffID = db.result->getString("StaffID");
			string StaffName = db.result->getString("StaffName");
			string IC = db.result->getString("IC");
			int Age = db.result->getInt("Age");
			string Address = db.result->getString("Address");
			string Contact_No = db.result->getString("Contact_No");
			string Email = db.result->getString("Email");
			string UserID = db.result->getString("UserID");

			cout << "| " << setw(13) << left << StaffID << " | "
				<< setw(23) << left << StaffName << " | "
				<< setw(18) << left << IC << " | "
				<< setw(8) << left << Age << " | "
				<< setw(18) << left << Address << " | "
				<< setw(38) << left << Contact_No << " | "
				<< setw(18) << left << Email << " | "
				<< setw(28) << left << UserID << " |" << endl;

			cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
		}

		if (!hasData)
		{
			cout << "| No data found matching the criteria.                                                                                                      |" << endl;
			cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
		}
		else
		{
			while (true)
			{
				cout << "Enter Staff ID that you want to update: ";
				getline(cin, StaffID);

				if (StaffID.empty())
				{
					cout << "Staff ID cannot be empty. Please try again." << endl;
					continue;
				}

				db.prepareStatement("SELECT * FROM hotel_staff WHERE StaffID = ?");
				db.statement->setString(1, StaffID);
				db.QueryResult();

				bool StaffIDValid = db.result->next();
				if (!StaffIDValid)
				{
					cout << "No data found matching the Staff ID. Please try again." << endl;
					continue;
				}

				while (true)
				{
					cout << "1. Update Staff Name\n";
					cout << "2. Update Age\n";
					cout << "3. Update Address\n";
					cout << "4. Update Contact Number\n";
					cout << "5. Update Email\n";
					cout << "Enter your choice (1-5): ";

					if (!(cin >> choice) || choice < 1 || choice > 5)
					{
						cout << "Invalid choice. Please enter a number between 1 and 5.\n";
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						continue;
					}

					switch (choice)
					{
					
					case 1:
						while (!validInput)
						{
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
							cout << "Enter New Staff Name: ";
							getline(cin, StaffName);

							if (StaffName.empty())
							{
								cout << "Staff name cannot be empty. Please try again.\n";
							}
							else
							{
								validInput = true;
							}
						}

						db.prepareStatement("UPDATE hotel_staff SET StaffName = ? WHERE StaffID = ?");
						db.statement->setString(1, StaffName);
						db.statement->setString(2, StaffID);
						db.QueryStatement();

						cout << "Update Successful!\n";
						system("cls");

						db.prepareStatement("SELECT * FROM hotel_staff WHERE StaffID = ?");
						db.statement->setString(1, StaffID);
						db.QueryResult();

						cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
						cout << "| StaffID       | Staff Name              | IC Number          | Age      | Address            | Phone Number                           | Email Address      | UserID                       |" << endl;
						cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;

						while (db.result->next())
						{
							hasData = true;

							StaffID = db.result->getString("StaffID");
							StaffName = db.result->getString("StaffName");
							IC = db.result->getString("IC");
							Age = db.result->getInt("Age");
							Address = db.result->getString("Address");
							Contact_No = db.result->getString("Contact_No");
							Email = db.result->getString("Email");
							UserID = db.result->getString("UserID");

							cout << "| " << setw(13) << left << StaffID << " | "
								<< setw(23) << left << StaffName << " | "
								<< setw(18) << left << IC << " | "
								<< setw(8) << left << Age << " | "
								<< setw(18) << left << Address << " | "
								<< setw(38) << left << Contact_No << " | "
								<< setw(18) << left << Email << " | "
								<< setw(28) << left << UserID << " |" << endl;

							cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
						}

						if (!hasData)
						{
							cout << "| No data found matching the criteria.                                                                                                      |" << endl;
							cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
						}

						system("cls");
						system("pause");
						Manage.ManageStaff();
						break;

					case 2:
						while (true)
						{
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
							cout << "Enter New Age (1 to 122): ";
							cin >> Age;
							cin.ignore();

							if (cin.fail() || Age <= 0 || Age > 122)
							{
								cin.clear();
								cin.ignore(numeric_limits<streamsize>::max(), '\n');
								cout << "Age must be a positive number between 1 and 122. Please try again.\n";
							}
							else
							{
								break;
							}
						}

						db.prepareStatement("UPDATE hotel_staff SET Age = ? WHERE StaffID = ?");
						db.statement->setInt(1, Age);
						db.statement->setString(2, StaffID);
						db.QueryStatement();

						cout << "Update Successful!\n";
						system("cls");

						db.prepareStatement("SELECT * FROM hotel_staff WHERE StaffID = ?");
						db.statement->setString(1, StaffID);
						db.QueryResult();

						cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
						cout << "| StaffID       | Staff Name              | IC number          | Age      | Address            | Phone Number                           | Email address      | UserID                       |" << endl;
						cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;

						while (db.result->next())
						{
							hasData = true;

							StaffID = db.result->getString("StaffID");
							StaffName = db.result->getString("StaffName");
							IC = db.result->getString("IC");
							Age = db.result->getInt("Age");
							Address = db.result->getString("Address");
							Contact_No = db.result->getString("Contact_No");
							Email = db.result->getString("Email");
							UserID = db.result->getString("UserID");

							cout << "| " << setw(15) << left << StaffID << " | "
								<< setw(23) << left << StaffName << " | "
								<< setw(18) << left << IC << " | "
								<< setw(8) << left << Age << " | "
								<< setw(18) << left << Address << " | "
								<< setw(38) << left << Contact_No << " | "
								<< setw(18) << left << Email << " | "
								<< setw(28) << left << UserID << " |" << endl;

							cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
						}

						if (!hasData)
						{
							cout << "| No data found matching the criteria.                                                                                                      |" << endl;
							cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
						}

						system("cls");
						system("pause");
						Manage.ManageStaff();
						break;

					case 3:
						system("cls");
						while (true)
						{
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
							cout << "Enter New Address: ";
							getline(cin, Address);

							if (Address.empty())
							{
								cout << "Address cannot be empty. Please try again.\n";
							}
							else
							{
								break;
							}
						}

						db.prepareStatement("UPDATE hotel_staff SET Address = ? WHERE StaffID = ?");
						db.statement->setString(1, Address);
						db.statement->setString(2, StaffID);
						db.QueryStatement();

						cout << "Update Successful!\n";
						system("cls");

						db.prepareStatement("SELECT * FROM hotel_staff WHERE StaffID = ?");
						db.statement->setString(1, StaffID);
						db.QueryResult();

						cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
						cout << "| StaffID       | Staff Name              | IC number          | Age      | Address            | Phone Number                           | Email address      | UserID                       |" << endl;
						cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;

						while (db.result->next())
						{
							hasData = true;

							StaffID = db.result->getString("StaffID");
							StaffName = db.result->getString("StaffName");
							IC = db.result->getString("IC");
							Age = db.result->getInt("Age");
							Address = db.result->getString("Address");
							Contact_No = db.result->getString("Contact_No");
							Email = db.result->getString("Email");
							UserID = db.result->getString("UserID");

							cout << "| " << setw(15) << left << StaffID << " | "
								<< setw(23) << left << StaffName << " | "
								<< setw(18) << left << IC << " | "
								<< setw(8) << left << Age << " | "
								<< setw(18) << left << Address << " | "
								<< setw(38) << left << Contact_No << " | "
								<< setw(18) << left << Email << " | "
								<< setw(28) << left << UserID << " |" << endl;

							cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
						}

						if (!hasData)
						{
							cout << "| No data found matching the criteria.                                                                                                      |" << endl;
							cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
						}

						system("cls");
						system("pause");
						Manage.ManageStaff();
						break;

					case 4:
						system("cls");

						while (true)
						{
							cout << "Phone Number (01x-xxxxxxx or 01x-xxxxxxxx): ";
							getline(cin, Contact_No);
							regex phone_regex("^01[0-9]-\\d{7,8}$");

							if (regex_match(Contact_No, phone_regex))
							{
								db.prepareStatement("SELECT COUNT(*) AS count FROM hotel_staff WHERE Contact_No = ?");
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

						db.prepareStatement("UPDATE hotel_staff SET Contact_No = ? WHERE StaffID = ?");
						db.statement->setString(1, Contact_No);
						db.statement->setString(2, StaffID);
						db.QueryStatement();

						cout << "Update Successful!\n";
						system("cls");

						db.prepareStatement("SELECT * FROM hotel_staff WHERE StaffID = ?");
						db.statement->setString(1, StaffID);
						db.QueryResult();

						cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
						cout << "| StaffID       | Staff Name              | IC number          | Age      | Address            | Phone Number                           | Email address      | UserID                       |" << endl;
						cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;

						while (db.result->next())
						{
							hasData = true;

							StaffID = db.result->getString("StaffID");
							StaffName = db.result->getString("StaffName");
							IC = db.result->getString("IC");
							Age = db.result->getInt("Age");
							Address = db.result->getString("Address");
							Contact_No = db.result->getString("Contact_No");
							Email = db.result->getString("Email");
							UserID = db.result->getString("UserID");

							cout << "| " << setw(15) << left << StaffID << " | "
								<< setw(23) << left << StaffName << " | "
								<< setw(18) << left << IC << " | "
								<< setw(8) << left << Age << " | "
								<< setw(18) << left << Address << " | "
								<< setw(38) << left << Contact_No << " | "
								<< setw(18) << left << Email << " | "
								<< setw(28) << left << UserID << " |" << endl;

							cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
						}

						if (!hasData)
						{
							cout << "| No data found matching the criteria.                                                                                                      |" << endl;
							cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
						}

						system("cls");
						system("pause");
						Manage.ManageStaff();
						break;

					case 5:
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
								continue;
							}
						}

						db.prepareStatement("UPDATE hotel_staff SET Email = ? WHERE StaffID = ?");
						db.statement->setString(1, Email);
						db.statement->setString(2, StaffID);
						db.QueryStatement();

						cout << "Update Successful!\n";
						system("cls");

						db.prepareStatement("SELECT * FROM hotel_staff WHERE StaffID = ?");
						db.statement->setString(1, StaffID);
						db.QueryResult();

						cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
						cout << "| StaffID       | Staff Name              | IC number          | Age      | Address            | Phone Number                           | Email address      | UserID                       |" << endl;
						cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;

						while (db.result->next())
						{
							hasData = true;

							StaffID = db.result->getString("StaffID");
							StaffName = db.result->getString("StaffName");
							IC = db.result->getString("IC");
							Age = db.result->getInt("Age");
							Address = db.result->getString("Address");
							Contact_No = db.result->getString("Contact_No");
							Email = db.result->getString("Email");
							UserID = db.result->getString("UserID");

							cout << "| " << setw(15) << left << StaffID << " | "
								<< setw(23) << left << StaffName << " | "
								<< setw(18) << left << IC << " | "
								<< setw(8) << left << Age << " | "
								<< setw(18) << left << Address << " | "
								<< setw(38) << left << Contact_No << " | "
								<< setw(18) << left << Email << " | "
								<< setw(28) << left << UserID << " |" << endl;

							cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
						}

						if (!hasData)
						{
							cout << "| No data found matching the criteria.                                                                                                      |" << endl;
							cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
						}

						system("cls");
						system("pause");
						Manage.ManageStaff();
						break;
					}
					break;  // Exit the loop once valid input is provided
				}
			}

		}
	}

	else if (selection == 3)
	{
		do
		{
			system("cls");
			db.prepareStatement("SELECT * FROM hotel_staff");
			db.QueryResult();

			cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
			cout << "| StaffID       | Staff Name              | IC number          | Age      | Address            | Phone Number                           | Email address      | UserID                       |" << endl;
			cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;

			bool hasData = false;
			while (db.result->next())
			{
				hasData = true;
				StaffID = db.result->getString("StaffID");
				StaffName = db.result->getString("StaffName");
				IC = db.result->getString("IC");
				Age = db.result->getInt("Age");
				Address = db.result->getString("Address");
				Contact_No = db.result->getString("Contact_No");
				Email = db.result->getString("Email");
				UserID = db.result->getString("UserID");

				cout << "| " << setw(15) << left << StaffID << " | "
					<< setw(23) << left << StaffName << " | "
					<< setw(18) << left << IC << " | "
					<< setw(8) << left << Age << " | "
					<< setw(18) << left << Address << " | "
					<< setw(38) << left << Contact_No << " | "
					<< setw(18) << left << Email << " | "
					<< setw(28) << left << UserID << " |" << endl;

				cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
			}

			if (!hasData)
			{
				cout << "| No data found matching the criteria.                                                                                                      |" << endl;
				cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
			}

			cout << "\nEnter Staff ID that you want to delete: ";
			getline(cin, StaffID);

			db.prepareStatement("SELECT * FROM hotel_staff WHERE StaffID = ?");
			db.statement->setString(1, StaffID);
			db.QueryResult();

			if (db.result->next())
			{
				db.prepareStatement("DELETE FROM hotel_staff WHERE StaffID = ?");
				db.statement->setString(1, StaffID);
				db.QueryStatement();

				cout << "Delete Successful!\n\n";
				system("cls");

				db.prepareStatement("SELECT * FROM hotel_staff");
				db.QueryResult();

				cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
				cout << "| StaffID       | Staff Name              | IC number          | Age      | Address            | Phone Number                           | Email address      | UserID                       |" << endl;
				cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;

				while (db.result->next())
				{
					hasData = true;
					StaffID = db.result->getString("StaffID");
					StaffName = db.result->getString("StaffName");
					IC = db.result->getString("IC");
					Age = db.result->getInt("Age");
					Address = db.result->getString("Address");
					Contact_No = db.result->getString("Contact_No");
					Email = db.result->getString("Email");
					UserID = db.result->getString("UserID");

					cout << "| " << setw(15) << left << StaffID << " | "
						<< setw(23) << left << StaffName << " | "
						<< setw(18) << left << IC << " | "
						<< setw(8) << left << Age << " | "
						<< setw(18) << left << Address << " | "
						<< setw(38) << left << Contact_No << " | "
						<< setw(18) << left << Email << " | "
						<< setw(28) << left << UserID << " |" << endl;

					cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
				}

				if (!hasData)
				{
					cout << "| No data found matching the criteria.                                                                                                      |" << endl;
					cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
				}

				break;
			}
			else
			{
				cout << "No data found matching the Staff ID. Please try again.\n";
				system("pause");
				continue;
			}
		} while (true);

		cout << "Do you want to delete another staff member? (Y/N): ";
		while (true)
		{
			cin >> option;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			if (option == 'y' || option == 'Y')
			{
				break;
			}
			else if (option == 'n' || option == 'N')
			{
				system("pause");
				system("cls");
				menu.ManagementMenu();
				return;
			}
			else
			{
				cout << "\nInvalid input. Please enter Y or N: ";
			}
		}
}

	else if (selection == 4)
	{
		system("cls");
		cout << "Enter the staff ID to search:";
		getline(cin, StaffID);

		db.prepareStatement("SELECT * FROM hotel_staff WHERE StaffID = ?");
		db.statement->setString(1, StaffID);
		db.QueryResult(); 

		StaffIDValid = db.result->next();
		if (!StaffIDValid)
		{
			cout << "No data found matching the Staff ID. Please try again." << endl;
		} while (!StaffIDValid);

		cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
		cout << "| StaffID       | Staff Name              | IC number          | Age      | Address            | Phone Number                           | Email address      | UserID                       |" << endl;
		cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;

		do
		{
			StaffID = db.result->getString("StaffID");
			StaffName = db.result->getString("StaffName");
			IC = db.result->getString("IC");
			Age = db.result->getInt("Age");
			Address = db.result->getString("Address");
			Contact_No = db.result->getString("Contact_No");
			Email = db.result->getString("Email");
			UserID = db.result->getString("UserID");

			cout << "| " << setw(15) << left << StaffID << " | "
				<< setw(23) << left << StaffName << " | "
				<< setw(18) << left << IC << " | "
				<< setw(8) << left << Age << " | "
				<< setw(18) << left << Address << " | "
				<< setw(38) << left << Contact_No << " | "
				<< setw(18) << left << Email << " | "
				<< setw(28) << left << UserID << " |" << endl;

			cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;

			hasData = true;

		} while (db.result->next());

		if (!hasData)
		{
			cout << "| No data found matching the criteria.                                                                                                      |" << endl;
			cout << "+---------------+-------------------------+--------------------+----------+--------------------+----------------------------------------+--------------------+------------------------------+" << endl;
		}
		system("pause");
		system("cls");
		menu.ManagementMenu();
		return;

	}

	else if (selection == 5)
	{
		cout << "\nReturning to the menu";
		system("pause");
		system("cls");
		menu.ManagementMenu();
	}

	else
	{
		cout << "invalid selection, please try again";
		Manage.ManageStaff();
	}
}

void manager::ManageRoom()
{
	mySQL_db db;
	manager Manage;
	Menu menu;
	string RoomID, Type, Status;
	int selection, choice;
	char option;
	double Price;
	bool RoomValid = false;

	system("cls");
	cout << "Room Management:\n1.Add New Room\n2.Update Room details\n3.Delete Room\n0.Return to main manager menu.";
	cout << "\nEnter your selection:";
	cin >> selection;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');


	if (selection == 1)
	{
		system("cls");
		cout << "---------------Registration for new room---------------";
		cout << "\nNew room type is: ";
		getline(cin, Type);

		cout << "Price: ";
		cin >> Price;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		db.prepareStatement("INSERT INTO room (Type, Status, Price) VALUES (?, 'Available', ?)");
		db.statement->setString(1, Type);
		db.statement->setDouble(2, Price);
		db.QueryStatement();

		cout << "New Room Created Successful!\n";
		cout << "+---------------+-------------------------+-------------------------+---------------------+" << endl;
		cout << "| RoomID       | Room Type               | Room Current Status     | Price per night     |" << endl;
		cout << "+---------------+-------------------------+-------------------------+---------------------+" << endl;

		db.prepareStatement("SELECT * FROM room");
		db.QueryResult();
		while (db.result->next())
		{
			RoomID = db.result->getString("RoomID");
			Type = db.result->getString("Type");
			Status = db.result->getString("Status");
			Price = db.result->getDouble("Price");

			cout << "| " << setw(15) << left << RoomID << " | "
				<< setw(23) << left << Type << " | "
				<< setw(23) << left << Status << " | "
				<< setw(19) << left << Price << " |" << endl;

			cout << "+---------------+-------------------------+-------------------------+---------------------+" << endl;
		}
		system("pause");
		system("cls");
		Manage.ManageRoom();

	}

	else if (selection == 2)
	{
		system("cls");
		cout << "+---------------+-------------------------+-------------------------+---------------------+" << endl;
		cout << "| RoomID       | Room Type               | Room Current Status     | Price per night     |" << endl;
		cout << "+---------------+-------------------------+-------------------------+---------------------+" << endl;

		db.prepareStatement("SELECT * FROM room");
		db.QueryResult();
		while (db.result->next())
		{
			RoomID = db.result->getString("RoomID");
			Type = db.result->getString("Type");
			Status = db.result->getString("Status");
			Price = db.result->getDouble("Price");

			cout << "| " << setw(15) << left << RoomID << " | "
				<< setw(23) << left << Type << " | "
				<< setw(23) << left << Status << " | "
				<< setw(19) << left << Price << " |" << endl;

			cout << "+---------------+-------------------------+-------------------------+---------------------+" << endl;
		}

		while (true)
		{
			cout << "Enter Room ID that you want to update: ";
			getline(cin, RoomID);
			if (RoomID.empty())
			{
				cout << "\nRoom ID cannot be empty, please enter a valid id";
				continue;
			}
			else break;
		}
		
		db.prepareStatement("SELECT * FROM room");
		db.QueryResult();
		RoomValid = db.result->next();
		if (!RoomValid)
		{
			cout << "No data found matching the Room ID. Please try again." << endl;
		} while (!RoomValid);

		cout << "1.Update Room Type\n";
		cout << "2.Update Room Status\n";
		cout << "3.Update Price\n";
		cout << "0.Back to menu\n";
		
		cout << "Enter your choice: ";
		cin >> choice;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		switch (choice)
		{
		case 0:
			cout << "Returning to previous menu...\n";
			system("pause");
			system("cls");
			menu.ManagementMenu();
			return;

		case 1:
			system("cls");
			cout << "Enter New Room Type: ";
			getline(cin, Type);

			db.prepareStatement("UPDATE room SET Type = ? WHERE RoomID = ?");
			db.statement->setString(1, Type);
			db.statement->setString(2, RoomID);
			db.QueryStatement();

			cout << "Update Successful!\n";
			system("cls");
			cout << "+---------------+-------------------------+-------------------------+---------------------+" << endl;
			cout << "| RoomID       | Room Type               | Room Current Status     | Price per night     |" << endl;
			cout << "+---------------+-------------------------+-------------------------+---------------------+" << endl;

			db.prepareStatement("SELECT * FROM room");
			db.QueryResult();
			while (db.result->next())
			{
				RoomID = db.result->getString("RoomID");
				Type = db.result->getString("Type");
				Status = db.result->getString("Status");
				Price = db.result->getDouble("Price");

				cout << "| " << setw(15) << left << RoomID << " | "
					<< setw(23) << left << Type << " | "
					<< setw(23) << left << Status << " | "
					<< setw(19) << left << Price << " |" << endl;

				cout << "+---------------+-------------------------+-------------------------+---------------------+" << endl;
			}

			system("pause");
			system("cls");
			Manage.ManageRoom();
			return;

		case 2:
			system("cls");
			cout << "Enter New Room Status(Available/Uavailable): ";
			getline(cin, Status);

			if (Status == "Available" || Status == "Unavailable") 
			{
				db.prepareStatement("UPDATE room SET Status = ? WHERE RoomID = ?");
				db.statement->setString(1, Status);
				db.statement->setString(2, RoomID);
				db.QueryStatement();

				cout << "Update Successful!\n";
				system("cls");
				cout << "+---------------+-------------------------+-------------------------+---------------------+" << endl;
				cout << "| RoomID       | Room Type               | Room Current Status     | Price per night     |" << endl;
				cout << "+---------------+-------------------------+-------------------------+---------------------+" << endl;

				db.prepareStatement("SELECT * FROM room");
				db.QueryResult();
				while (db.result->next())
				{
					RoomID = db.result->getString("RoomID");
					Type = db.result->getString("Type");
					Status = db.result->getString("Status");
					Price = db.result->getDouble("Price");

					cout << "| " << setw(15) << left << RoomID << " | "
						<< setw(23) << left << Type << " | "
						<< setw(23) << left << Status << " | "
						<< setw(19) << left << Price << " |" << endl;

					cout << "+---------------+-------------------------+-------------------------+---------------------+" << endl;
				}

				system("pause");
				system("cls");
				Manage.ManageRoom();
				return;
			}
			else 
			{
				cout << "Invalid input, please try again.\n";
				system("pause");
				system("cls");
				Manage.ManageRoom();
				return;
			}
			
		case 3:
			system("cls");
			cout << "Enter New Price for the room: ";
			cin >> Price;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			db.prepareStatement("UPDATE room SET Price = ? WHERE RoomID = ?");
			db.statement->setDouble(1, Price);
			db.statement->setString(2, RoomID);
			db.QueryStatement();


			cout << "Update Successful!\n";
			system("cls");
			cout << "+---------------+-------------------------+-------------------------+---------------------+" << endl;
			cout << "| RoomID       | Room Type               | Room Current Status     | Price per night     |" << endl;
			cout << "+---------------+-------------------------+-------------------------+---------------------+" << endl;

			db.prepareStatement("SELECT * FROM room");
			db.QueryResult();
			while (db.result->next())
			{
				RoomID = db.result->getString("RoomID");
				Type = db.result->getString("Type");
				Status = db.result->getString("Status");
				Price = db.result->getDouble("Price");

				cout << "| " << setw(15) << left << RoomID << " | "
					<< setw(23) << left << Type << " | "
					<< setw(23) << left << Status << " | "
					<< setw(19) << left << Price << " |" << endl;

				cout << "+---------------+-------------------------+-------------------------+---------------------+" << endl;
			}

			system("pause");
			system("cls");
			Manage.ManageRoom();
			return;
			

		default:
			cout << "\nInvalid input! Please try again.\n";
		}
	}

	else if (selection == 3)
	{
		do
		{
			system("cls");
			cout << "\nEnter Room ID that you want to delete: ";
			getline(cin, RoomID);

			db.prepareStatement("SELECT * FROM room WHERE RoomID = ?");
			db.statement->setString(1, RoomID);
			db.QueryResult();

			if (!db.result->next())
			{
				cout << "No data found matching the Room ID. Please try again.\n";
				system("pause");
				continue;
			}

			db.prepareStatement("DELETE FROM room WHERE RoomID = ?");
			db.statement->setString(1, RoomID);
			db.QueryStatement();

			cout << "Delete Successful!\n\n";
			system("cls");
			cout << "+---------------+-------------------------+-------------------------+---------------------+" << endl;
			cout << "| RoomID       | Room Type               | Room Current Status     | Price per night     |" << endl;
			cout << "+---------------+-------------------------+-------------------------+---------------------+" << endl;

			db.prepareStatement("SELECT * FROM room");
			db.QueryResult();
			while (db.result->next())
			{
				RoomID = db.result->getString("RoomID");
				Type = db.result->getString("Type");
				Status = db.result->getString("Status");
				Price = db.result->getDouble("Price");

				cout << "| " << setw(15) << left << RoomID << " | "
					<< setw(23) << left << Type << " | "
					<< setw(23) << left << Status << " | "
					<< setw(19) << left << Price << " |" << endl;

				cout << "+---------------+-------------------------+-------------------------+---------------------+" << endl;
			}


			cout << "Do you want to delete another room? (Y/N): ";
			while (true)
			{
				cin >> option;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				if (option == 'y' || option == 'Y')
				{
					break;
				}
				else if (option == 'n' || option == 'N')
				{
					system("pause");
					system("cls");
					Manage.ManageRoom();
					return;
				}
				else
				{
					cout << "\nInvalid input. Please enter Y or N: ";
				}
			}
		} while (true);
	}

	else if (selection == 0)
	{
		cout << "Returning to main manager menu...\n";
		system("pause");
		system("cls");
		menu.ManagementMenu();
	}
}

void manager::CheckReport()
{
	mySQL_db db;
	Menu menu;

	try
	{
		// Prompt the manager for the desired month and year
		int month, year;
		cout << "Enter the year (e.g., 2025): ";
		cin >> year;
		cout << "Enter the month (1-12): ";
		cin >> month;

		// Validate the input
		if (month < 1 || month > 12)
		{
			cout << "Invalid month entered. Please try again.\n";
			system("pause");
			system("cls");
			CheckReport(); // Restart the function
			return;
		}

		// Retrieve total revenue for the specified month
		db.prepareStatement("SELECT SUM(Payment_amount) AS TotalRevenue FROM invoice "
			"WHERE MONTH(Date) = ? AND YEAR(Date) = ?");
		db.statement->setInt(1, month);
		db.statement->setInt(2, year);
		db.QueryResult();

		double totalRevenue = 0.0;
		if (db.result->next())
		{
			totalRevenue = db.result->getDouble("TotalRevenue");
		}

		// Count the total number of invoices (rooms occupied) for the specified month
		db.prepareStatement("SELECT COUNT(InvoiceID) AS TotalRoomsOccupied FROM invoice "
			"WHERE MONTH(Date) = ? AND YEAR(Date) = ?");
		db.statement->setInt(1, month);
		db.statement->setInt(2, year);
		db.QueryResult();

		int totalRoomsOccupied = 0;
		if (db.result->next())
		{
			totalRoomsOccupied = db.result->getInt("TotalRoomsOccupied");
		}

		// Query total number of rooms
		db.prepareStatement("SELECT COUNT(*) AS TotalRooms FROM room");
		db.QueryResult();

		int totalRooms = 0;
		if (db.result->next())
		{
			totalRooms = db.result->getInt("TotalRooms");
		}

		// Calculate total expenses
		double expensePerRoom = 1000.0; // Fixed expense per room
		double totalExpenses = totalRooms * expensePerRoom;

		// Calculate profit
		double profit = totalRevenue - totalExpenses;

		// Calculate occupancy rate
		double occupancyRate = (totalRoomsOccupied / static_cast<double>(totalRooms)) * 100;

		// ADR (Average Daily Rate) calculation
		double adr = (totalRoomsOccupied > 0) ? (totalRevenue / totalRoomsOccupied) : 0.0;

		// Display report
		system("cls");
		cout << "Financial Report for the Hotel\n";
		cout << "-------------------------------\n";
		cout << "Month: " << month << ", Year: " << year << "\n";
		cout << left;
		cout << setw(30) << "Total Revenue: " << "RM" << setw(15) << totalRevenue << endl;
		cout << setw(30) << "Total Expenses: " << "RM" << setw(15) << totalExpenses << endl;
		cout << setw(30) << "Profit: " << "RM" << setw(15) << profit << endl;
		cout << setw(30) << "Total Rooms Occupied: " << setw(15) << totalRoomsOccupied << endl;
		cout << setw(30) << "Occupancy Rate: " << setw(5) << occupancyRate << "%" << endl;
		cout << setw(30) << "Average Daily Rate (ADR): " << "RM" << setw(15) << adr << endl;

		cout << "\nEnd of Report.\n";
		system("pause");
		system("cls");

		menu.ManagementMenu();
	}
	catch (const sql::SQLException& e)
	{
		cerr << "Database error during report generation: " << e.what() << endl;
	}
}

