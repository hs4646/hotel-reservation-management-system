#include <iostream>
#include <mysql/jdbc.h>
#include "mysql.h"
#include "Menu.h"
#include "staff.h"
#include "manager.h"
#include "guest.h"
#include "Login.h"
#include <iomanip>
using namespace std;

Login login;

void Menu::MainMenu()
{
	cout << "\033[1;34m--------------------------------------------------------------\033[0m\n";
	cout << "\033[1;33m  \nWelcome to the Hotel Reservation and Management System\033[0m\n\n";
	cout << "\033[1;34m--------------------------------------------------------------\033[0m\n";
	
}

void Menu::ForgetPass() 
{
	Menu menu;
	string input;
	int selection;
	Login login;


	cout << "Looks like you forget your username/password\n";

	while (true)
	{
		cout << "1. Reset username/password\n2. Back to start menu\n3. Exit\n";
		cout << "\nEnter the number of the selection:";
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

	if (selection == 1)
	{
		login.Reset();
	}

	else if (selection == 2)
	{
		system("cls");
		menu.MainMenu();
		login.LoginMenu();
	}

	else if (selection == 3)
	{
		cout << "Exiting the program";
		exit(0);
	}
}

void Menu::ManagementMenu()
{
	manager manage;
	Menu menu;
	int select;
	string input;

	cout << "\033[1;34m--------------------------------------------------------------\033[0m\n";
	cout << "\033[1;33m  \nWelcome to the Hotel Management System Manager Menu\033[0m\n\n";
	cout << "\033[1;34m--------------------------------------------------------------\033[0m\n";
	
	cout << "Select an action that provided below\n";

	while (true)
	{
		cout << "1. Manage room\n2. Manage staff\n3. Check report\n0. Exit\n";
		cout << "\nEnter the number of the selection:";
		getline(cin, input);

		if (input.empty())
		{
			cout << "Input cannot be empty. Please enter a valid number (0, 1, 2, or 3).\n";
			continue;
		}

		try
		{
			select = stoi(input);

			if (select < 0 || select > 3)
			{
				cout << "Invalid choice. Please enter a number between 0 and 3.\n";
				continue;
			}
			break;
		}
		catch (const invalid_argument&)
		{
			cout << "Invalid input. Please enter a valid number (0, 1, 2, or 3).\n";
		}
	}

	if (select == 1) 
	{
		manage.ManageRoom();
	}

	else if (select == 2) 
	{
		manage.ManageStaff();
	}

	else if (select == 3)
	{
		manage.CheckReport();
	}

	else if (select == 0) 
	{
		cout << "Exiting manager mode";
		menu.MainMenu();
		login.LoginMenu();
		return;
	}

}

void Menu::GuestMenu()
{
	Menu menu;
	guest guest;
	char choice;
	string input;

	cout << "\033[1;34m--------------------------------------------------------------\033[0m\n";
	cout << "\033[1;33m  \nWelcome to the Hotel Management System Guest Menu\033[0m\n\n";
	cout << "\033[1;34m--------------------------------------------------------------\033[0m\n";
	cout << "Select an action that provided below\n";

	while (true)
	{
		cout << "1. View room details and reserve for a room\n2. Make payment\n3. Profile\n0. Exit\n";
		cout << "\nEnter the number of the selection:";
		getline(cin, input);

		if (input.empty())
		{
			cout << "Input cannot be empty. Please enter a valid number (0, 1, 2, or 3).\n";
			continue;
		}

		try
		{
			choice = stoi(input);

			if (choice < 0 || choice > 3)
			{
				cout << "Invalid choice. Please enter a number between 0 and 3.\n";
				continue;
			}
			break;
		}
		catch (const invalid_argument&)
		{
			cout << "Invalid input. Please enter a valid number (0, 1, 2, or 3).\n";
		}
	}

	if (choice == 1) 
	{
		guest.ViewRoom();
	}

	else if (choice == 2) 
	{
		guest.MakePayment();
	}

	else if (choice == 3) 
	{
		guest.Profile();
	}

	else if (choice == 0) 
	{
		cout << "Exiting guest mode";
		menu.MainMenu();
		login.LoginMenu();
		return;
	}

}

void Menu::StaffMenu()
{
	staff staff;
	Menu menu;
	int choice;
	string input;

	cout << "\033[1;34m--------------------------------------------------------------\033[0m\n";
	cout << "\033[1;33m  \nWelcome to the Hotel Management System Staff Menu\033[0m\n\n";
	cout << "\033[1;34m--------------------------------------------------------------\033[0m\n";
	cout << "Select an action that provided below\n";

	while (true)
	{
		cout << "1. Manage reservation\n2. Manage guest\n3. Checkout for guest\n0. Exit\n";
		cout << "\nEnter the number of the selection:";
		getline(cin, input);

		if (input.empty())
		{
			cout << "Input cannot be empty. Please enter a valid number (0, 1, 2, or 3).\n";
			continue;
		}

		try
		{
			choice = stoi(input);

			if (choice < 0 || choice > 3)
			{
				cout << "Invalid choice. Please enter a number between 0 and 3.\n";
				continue;
			}
			break;
		}
		catch (const invalid_argument&)
		{
			cout << "Invalid input. Please enter a valid number (0, 1, 2, or 3).\n";
		}
	}

	if (choice == 1) 
	{
		staff.ManageReservation();
	}

	else if (choice == 2) 
	{
		staff.ManageGuest();
	}

	else if (choice == 3)
	{
		staff.CheckOutGuest();
	}

	else if (choice == 0) 
	{
		cout << "Exiting staff mode";
		menu.MainMenu();
		login.LoginMenu();
		return;
	}

}
