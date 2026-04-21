#include <iostream>
#include <mysql/jdbc.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <chrono>
#include "mysql.h"
#include "staff.h"
#include "Menu.h"
#include "DateUtils.h"

using namespace std;


void staff::ManageReservation()
{
	system("cls");
	Menu menu;
	mySQL_db db;
	string ReservationID, RoomID, GuestID, DateIn, DateOut, Payment_status, input;
	int selection;

	while (true)
	{
		cout << "Reservation management:\n"
			<< "1. Update Reservation details\n"
			<< "2. Delete Reservation\n"
			<< "3. Search a Reservation\n"
			<< "4. Return to staff main menu\n";
		cout << "Enter your selection: ";
		getline(cin, input);

		if (input.empty())
		{
			cout << "Input cannot be empty. Please enter a valid number (1-4).\n";
			continue;
		}

		try
		{
			selection = stoi(input);

			if (selection < 1 || selection > 4)
			{
				cout << "Invalid selection. Please enter a number between 1 and 4.\n";
				continue;
			}
			break;
		}
		catch (const invalid_argument&)
		{
			cout << "Invalid input. Please enter a valid number (1-4).\n";
		}
	}

if (selection == 1)
{
	system("cls");

	db.prepareStatement("SELECT * FROM reservation");
	db.QueryResult();

	cout << "+-----------------+-----------------+-----------------+-----------------------+-----------------------+-----------------+" << endl;
	cout << "| ReservationID  | Room ID         | Guest ID        | Check-In Date         | Check-Out Date        | Payment Status  |" << endl;
	cout << "+-----------------+-----------------+-----------------+-----------------------+-----------------------+-----------------+" << endl;

	bool hasData = false;
	while (db.result->next())
	{
		hasData = true;
		ReservationID = db.result->getString("ReservationID");
		RoomID = db.result->getString("RoomID");
		GuestID = db.result->getString("GuestID");
		DateIn = db.result->getString("DateIn");
		DateOut = db.result->getString("DateOut");
		Payment_status = db.result->getString("Payment_status");

		cout << "| " << setw(15) << left << ReservationID << " | "
			<< setw(15) << left << RoomID << " | "
			<< setw(15) << left << GuestID << " | "
			<< setw(23) << left << DateIn << " | "
			<< setw(23) << left << DateOut << " | "
			<< setw(15) << left << Payment_status << " |" << endl;

		cout << "+-----------------+-----------------+-----------------+-----------------------+-----------------------+-----------------+" << endl;
	}

	if (!hasData)
	{
		cout << "No reservations found in the database.\n";
		return;
	}

	cout << "Enter Reservation ID that you want to update: ";
	getline(cin, ReservationID);

	db.prepareStatement("SELECT * FROM reservation WHERE ReservationID = ?");
	db.statement->setString(1, ReservationID);
	db.QueryResult();

	if (!db.result->next())
	{
		cout << "Invalid Reservation ID. No matching record found.\n";
		return;
	}

	system("cls");
	cout << "1. Update RoomID\n";
	cout << "2. Update GuestID\n";
	cout << "3. Update Check-In Date\n";
	cout << "4. Update Check-Out Date\n";
	cout << "5. Update Payment Status\n";
	cout << "0. Exit\n";

	int choice;
	while (true)
	{
		cout << "Enter your choice: ";
		cin >> choice;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input! Please enter a number between 0 and 5.\n";
			continue;
		}

		if (choice >= 0 && choice <= 5)
		{
			break;
		}
		else
		{
			cout << "Invalid choice! Please enter a number between 0 and 5.\n";
		}
	}

	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	switch (choice)
	{
	case 1:
		system("cls");
		while (true)
		{
			cout << "Enter new RoomID: ";
			getline(cin, RoomID);

			if (RoomID.empty())
			{
				cout << "RoomID cannot be empty. Please try again.\n";
				continue;
			}

			db.prepareStatement("SELECT RoomID FROM room WHERE RoomID = ?");
			db.statement->setString(1, RoomID);
			db.QueryResult();

			if (!db.result->next())
			{
				cout << "Invalid Room ID. Please enter a valid one.\n";
				continue;
			}

			db.prepareStatement("SELECT * FROM reservation WHERE RoomID = ? AND ReservationID != ? AND (DateIn <= ? AND DateOut >= ?)");
			db.statement->setString(1, RoomID);
			db.statement->setString(2, ReservationID);
			db.statement->setString(3, DateOut);
			db.statement->setString(4, DateIn);
			db.QueryResult();

			if (db.result->next())
			{
				cout << "This room is already reserved for the selected dates. Please choose a different room.\n";
				continue;
			}

			db.prepareStatement("UPDATE reservation SET RoomID = ? WHERE ReservationID = ?");
			db.statement->setString(1, RoomID);
			db.statement->setString(2, ReservationID);
			db.QueryStatement();
			cout << "Room ID updated successfully!\n";
			system("pause");
			system("cls");
			menu.StaffMenu();
			break;
		}
		break;

	case 2:
		system("cls");
		while (true)
		{
			cout << "Enter new Guest ID: ";
			getline(cin, GuestID);

			db.prepareStatement("SELECT GuestID FROM hotel_guest WHERE GuestID = ?");
			db.statement->setString(1, GuestID);
			db.QueryResult();

			if (!db.result->next())
			{
				cout << "Invalid Guest ID. Please enter a valid one.\n";
				continue;
			}

			db.prepareStatement("UPDATE reservation SET GuestID = ? WHERE ReservationID = ?");
			db.statement->setString(1, GuestID);
			db.statement->setString(2, ReservationID);
			db.QueryStatement();
			cout << "Guest ID updated successfully!\n";
			system("pause");
			system("cls");
			menu.StaffMenu();
			break;
		}
		break;

	case 3:
		system("cls");
		while (true)
		{
			cout << "Enter new Check-In Date (YYYY-MM-DD): ";
			getline(cin, DateIn);

			if (!isValidDateFormat(DateIn))
			{
				cout << "Invalid date format! Please enter a valid date in YYYY-MM-DD format.\n";
				continue;
			}
			else if (!isTodayOrFuture(DateIn))
			{
				cout << "Check-In Date cannot be in the past! Please try again.\n";
				continue;
			}

			db.prepareStatement("UPDATE reservation SET DateIn = ? WHERE ReservationID = ?");
			db.statement->setString(1, DateIn);
			db.statement->setString(2, ReservationID);
			db.QueryStatement();
			cout << "Check-In Date updated successfully!\n";
			system("pause");
			system("cls");
			menu.StaffMenu();
			break;
		}
		break;

	case 4:
		system("cls");
		while (true)
		{
			cout << "Enter new Check-Out Date (YYYY-MM-DD): ";
			getline(cin, DateOut);

			if (!isValidDateFormat(DateOut))
			{
				cout << "Invalid date format! Please enter a valid date in YYYY-MM-DD format.\n";
				continue;
			}
			else if (DateOut <= DateIn)
			{
				cout << "Check-Out Date must be after Check-In Date! Please try again.\n";
				continue;
			}

			db.prepareStatement("UPDATE reservation SET DateOut = ? WHERE ReservationID = ?");
			db.statement->setString(1, DateOut);
			db.statement->setString(2, ReservationID);
			db.QueryStatement();
			cout << "Check-Out Date updated successfully!\n";
			system("pause");
			system("cls");
			menu.StaffMenu();
			break;
		}
		break;

	case 5:
		system("cls");
		while (true)
		{
			cout << "Enter new Payment Status (Paid/Unpaid): ";
			getline(cin, Payment_status);

			if (Payment_status != "Paid" && Payment_status != "Unpaid")
			{
				cout << "Invalid Payment Status! Please enter either 'Paid' or 'Unpaid'.\n";
				continue;
			}

			db.prepareStatement("UPDATE reservation SET Payment_status = ? WHERE ReservationID = ?");
			db.statement->setString(1, Payment_status);
			db.statement->setString(2, ReservationID);
			db.QueryStatement();
			cout << "Payment Status updated successfully!\n";
			system("pause");
			system("cls");
			menu.StaffMenu();
			break;
		}
		break;

	case 0:
		cout << "Exiting update process.\n";
		system("pause");
		system("cls");
		menu.StaffMenu();

	default:
		cout << "Invalid choice!\n";
		break;
	}
}

else if (selection == 2)
{
	char option{};

	db.prepareStatement("SELECT * FROM reservation");
	db.QueryResult();

	cout << "+-----------------+-----------------+-----------------+-----------------------+-----------------------+-----------------+" << endl;
	cout << "| Reservation ID  | Guest ID        | Room ID         | Check-In Date         | Check-Out Date        | Payment Status  |" << endl;
	cout << "+-----------------+-----------------+-----------------+-----------------------+-----------------------+-----------------+" << endl;

	while (db.result->next())
	{
		ReservationID = db.result->getString("ReservationID");
		GuestID = db.result->getString("GuestID");
		RoomID = db.result->getString("RoomID");
		DateIn = db.result->getString("DateIn");
		DateOut = db.result->getString("DateOut");
		Payment_status = db.result->getString("Payment_status");

		cout << "| " << setw(15) << left << ReservationID << " | "
			<< setw(15) << left << GuestID << " | "
			<< setw(15) << left << RoomID << " | "
			<< setw(23) << left << DateIn << " | "
			<< setw(23) << left << DateOut << " | "
			<< setw(15) << left << Payment_status << " |" << endl;

		cout << "+-----------------+-----------------+-----------------+-----------------------+-----------------------+-----------------+" << endl;
	}

	do
	{
		cout << "\nEnter Reservation ID to delete: ";
		getline(cin, ReservationID);

		if (ReservationID.empty())
		{
			cout << "Reservation ID cannot be empty. Please try again.\n";
			continue;
		}

		db.prepareStatement("SELECT * FROM reservation WHERE ReservationID = ?");
		db.statement->setString(1, ReservationID);
		db.QueryResult();

		if (db.result->next())
		{
			db.prepareStatement("DELETE FROM reservation WHERE ReservationID = ?");
			db.statement->setString(1, ReservationID);
			db.QueryStatement();

			cout << "Reservation with ID " << ReservationID << " has been deleted successfully!\n";
		}
		else
		{
			cout << "Error: No reservation found with ID " << ReservationID << ".\n";
		}

		cout << "\nDo you want to delete another reservation? (Y/N): ";
		while (true)
		{
			cin >> option;
			if (option == 'y' || option == 'Y' || option == 'n' || option == 'N')
			{
				break;
			}
			else
			{
				cout << "Invalid input. Please enter Y or N: ";
			}
		}

		cin.ignore(numeric_limits<streamsize>::max(), '\n');

	} while (option == 'y' || option == 'Y');

	system("pause");
	system("cls");
	menu.StaffMenu();
}

else if (selection == 3)
{
	char option{};

	do
	{
		cout << "Enter the Reservation ID to search: ";
		getline(cin, ReservationID);

		if (ReservationID.empty())
		{
			cout << "Reservation ID cannot be empty. Please try again.\n";
			continue;
		}

		db.prepareStatement("SELECT * FROM reservation WHERE ReservationID = ?");
		db.statement->setString(1, ReservationID);
		db.QueryResult();

		cout << "+-----------------+-----------------+-----------------+-----------------------+-----------------------+-----------------+" << endl;
		cout << "| Reservation ID  | Guest ID        | Room ID         | Check-In Date         | Check-Out Date        | Payment Status  |" << endl;
		cout << "+-----------------+-----------------+-----------------+-----------------------+-----------------------+-----------------+" << endl;

		bool hasData = false;
		while (db.result->next())
		{
			hasData = true;

			ReservationID = db.result->getString("ReservationID");
			GuestID = db.result->getString("GuestID");
			RoomID = db.result->getString("RoomID");
			DateIn = db.result->getString("DateIn");
			DateOut = db.result->getString("DateOut");
			Payment_status = db.result->getString("Payment_status");

			cout << "| " << setw(15) << left << ReservationID << " | "
				<< setw(15) << left << GuestID << " | "
				<< setw(15) << left << RoomID << " | "
				<< setw(23) << left << DateIn << " | "
				<< setw(23) << left << DateOut << " | "
				<< setw(15) << left << Payment_status << " |" << endl;

			cout << "+-----------------+-----------------+-----------------+-----------------------+-----------------------+-----------------+" << endl;
		}

		if (!hasData)
		{
			cout << "No data found matching the criteria.\n";
		}


		cout << "\nDo you want to search for another reservation? (Y/N): ";
		while (true)
		{
			cin >> option;
			if (option == 'y' || option == 'Y' || option == 'n' || option == 'N')
			{
				break;
			}
			else
			{
				cout << "Invalid input. Please enter Y or N: ";
			}
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

	} while (option == 'y' || option == 'Y');

	system("pause");
	system("cls");
	menu.StaffMenu();
}

else if (selection == 4)
{
	system("cls");
	menu.StaffMenu();
	return;
}

}

void staff::ManageGuest()
{
	staff staff;
	Menu menu;
	mySQL_db db;
	string GuestID, Name, Contact_No, Email, UserID, input, IC;
	int selection, choice, Age;
	char option;
	bool GuestValid = false;
	bool hasData = false;

	system("cls");
	while (true)
	{
		cout << "Guest management:\n1.Delete guest information\n2.Search guest\n3.Return to staff main menu";
		cout << "\nEnter your selection:";
		getline(cin, input);

		if (input.empty())
		{
			cout << "Input cannot be empty. Please enter a valid number (1, 2, or 3).\n";
			continue;
		}

		try
		{
			selection = stoi(input);

			if (selection < 1 || selection > 3)
			{
				cout << "Invalid selection. Please enter a number between 1 and 3.\n";
				continue;
			}
			break;
		}
		catch (const invalid_argument&)
		{
			cout << "Invalid input. Please enter a valid number (1, 2, or 3).\n";
		}
	}
	db.prepareStatement("SELECT * FROM reservation");
	db.QueryResult();

	if (selection == 1)
	{
		do
		{
			system("cls");
			while (true)
			{
				cout << "Enter Guest ID that you want to delete: ";
				getline(cin, GuestID);

				if (GuestID.empty())
				{
					cout << "GuestID cannot be empty. Please try again.\n";
					continue;
				}

				db.prepareStatement("SELECT * FROM hotel_guest WHERE GuestID = ?");
				db.statement->setString(1, GuestID);
				db.QueryResult();

				if (db.result->next())
				{
					break;
				}
				else
				{
					cout << "Invalid GuestID. Please enter a valid GuestID.\n";
					continue;
				}
			}

			db.prepareStatement("DELETE FROM hotel_guest WHERE GuestID = ?");
			db.statement->setString(1, GuestID);
			db.QueryStatement();

			cout << "Delete Successful!\n\n";

			cout << "Would you like to delete another? (Y/N): ";
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
			staff.ManageGuest();
			return;

		}while (option == 'Y' || option == 'y');
	}

	else if (selection == 2)
	{
		do
		{
			system("cls");
			while (true)
			{
				cout << "Enter the guest IC number that you want to search: ";
				getline(cin, IC);

				if (IC.empty())
				{
					cout << "GuestID cannot be empty. Please try again.\n";
					continue;
				}

				db.prepareStatement("SELECT * FROM hotel_guest WHERE IC = ?");
				db.statement->setString(1, IC);
				db.QueryResult();

				if (db.result->next())
				{
					break;
				}
				else
				{
					cout << "Invalid IC number. Please enter a valid number.\n";
					continue;
				}
			}

			bool hasData = false;
			cout << "+-----------------+-------------------+-----+-------------------+---------------------+-----------------+" << endl;
			cout << "| Guest ID       | Guest Name        | Age | Phone Number      | Email               | User ID         |" << endl;
			cout << "+-----------------+-------------------+-----+-------------------+---------------------+-----------------+" << endl;

			do
			{
				GuestID = db.result->getString("GuestID");
				Name = db.result->getString("Name");
				Age = db.result->getInt("Age");
				Contact_No = db.result->getString("Contact_No");
				Email = db.result->getString("Email");
				UserID = db.result->getString("UserID");

				cout << "| " << setw(15) << left << GuestID << " | "
					<< setw(17) << left << Name << " | "
					<< setw(3) << left << Age << " | "
					<< setw(17) << left << Contact_No << " | "
					<< setw(19) << left << Email << " | "
					<< setw(15) << left << UserID << " |" << endl;

				cout << "+-----------------+-------------------+-----+-------------------+---------------------+-----------------+" << endl;

				hasData = true;

			} while (db.result->next());

			if (!hasData)
			{
				cout << "No data found matching the criteria." << endl;
			}

			cout << "Would you like to search another? (Y/N): ";
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
			staff.ManageGuest();
			return;

		}while (option == 'Y' || option == 'y');
	}

	else if (selection == 3)
	{
		system("cls");
		menu.StaffMenu();
		return;
	}
}

void staff::CheckOutGuest()
{
	Menu menu;
	mySQL_db db;
	string Name, GuestID, RoomID, DateOut, currentDateTime;
	char option;
	bool hasActiveReservation = false;

	system("cls");
	cout << "--------------- Checkout Room for Guest ---------------\n";

	while (true)
	{
		cout << "\nEnter Guest Name: ";
		getline(cin, Name);

		if (!Name.empty())
		{
			db.prepareStatement("SELECT GuestID FROM hotel_guest WHERE Name = ?;");
			db.statement->setString(1, Name);
			db.QueryResult();

			if (db.result->next())
			{
				GuestID = db.result->getString("GuestID");
				break;
			}
			else
			{
				cout << "Error: No guest found with this name. Try again.\n";
			}
		}
		else
		{
			cout << "Name cannot be empty. Please try again.\n";
		}
	}

	cout << "GuestID retrieved: " << GuestID << endl;

	// Get Active Reservations
	db.prepareStatement("SELECT reservation.RoomID, room.Price, reservation.DateOut "
		"FROM reservation JOIN room ON reservation.RoomID = room.RoomID "
		"WHERE reservation.GuestID = ? AND CURDATE() <= reservation.DateOut;");
	db.statement->setString(1, GuestID);
	db.QueryResult();

	int index = 1;

	cout << "\nActive Reservations for Guest:\n";
	while (db.result->next())
	{
		hasActiveReservation = true;
		RoomID = db.result->getString("RoomID");
		DateOut = db.result->getString("DateOut");

		cout << index++ << ". RoomID: " << RoomID << ", DateOut: " << DateOut << endl;
	}

	if (!hasActiveReservation)
	{
		cout << "No active reservations found for GuestID: " << GuestID << endl;
		system("pause");
		system("cls");
		menu.StaffMenu();
		return;
	}

	int choice;
	while (true)
	{
		cout << "\nEnter the number of the reservation you wish to check out (or 0 to cancel): ";
		cin >> choice;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		if (choice == 0)
		{
			cout << "Check-out process canceled.\n";
			system("pause");
			system("cls");
			menu.StaffMenu();
			return;
		}

		if (choice >= 1 && choice < index)
		{
			break;
		}
		else
		{
			cout << "Invalid selection. Please try again.\n";
		}
	}

	// Retrieve the selected reservation
	db.prepareStatement("SELECT reservation.RoomID, reservation.DateOut, room.Price "
		"FROM reservation JOIN room ON reservation.RoomID = room.RoomID "
		"WHERE reservation.GuestID = ?;");
	db.statement->setString(1, GuestID);
	db.QueryResult();

	index = 1;
	while (db.result->next())
	{
		if (index == choice)
		{
			RoomID = db.result->getString("RoomID");
			DateOut = db.result->getString("DateOut");
			break;
		}
		index++;
	}

	// Perform Check-Out Process
	db.prepareStatement("SELECT NOW() AS CurrentDateTime;");
	db.QueryResult();
	if (db.result->next())
	{
		currentDateTime = db.result->getString("CurrentDateTime");
	}

	int lateDays = 0;
	db.prepareStatement("SELECT DATEDIFF(?, ?) AS LateDays;");
	db.statement->setString(1, currentDateTime);
	db.statement->setString(2, DateOut);
	db.QueryResult();

	if (db.result->next())
	{
		lateDays = db.result->getInt("LateDays");
	}

	if (lateDays > 0)
	{
		double finePerDay = db.result->getDouble("Price") * 1.5;
		double totalFine = finePerDay * lateDays;

		cout << "\nLate Check-Out Fine Applied:\n";
		cout << "Late Days: " << lateDays << endl;
		cout << "Fine Per Day: RM" << finePerDay << endl;
		cout << "Total Fine: RM" << totalFine << endl;
		cout << "Please pay the fine of RM" << totalFine << " at the counter before leaving.\n";
	}
	else
	{
		cout << "No late check-out fine. Guest checked out on time.\n";
	}

	db.prepareStatement("UPDATE room SET Status = 'Available' WHERE RoomID = ?;");
	db.statement->setString(1, RoomID);
	db.QueryStatement();

	db.prepareStatement("DELETE FROM reservation WHERE GuestID = ? AND RoomID = ?;");
	db.statement->setString(1, GuestID);
	db.statement->setString(2, RoomID);
	db.QueryStatement();

	cout << "Reservation for Room " << RoomID << " checked out successfully.\n";
	system("pause");
	system("cls");
	menu.StaffMenu();
}
