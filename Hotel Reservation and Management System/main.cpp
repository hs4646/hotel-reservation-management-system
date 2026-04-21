#include <iostream>
#include <regex>
#include <mysql/jdbc.h>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "mysql.h"
#include "Menu.h"
#include "Login.h"


using namespace std;

int main()
{
	mySQL_db db;
	string Username;
	string Password;
	Menu menu;
	Login loginmenu;
	menu.MainMenu();
	loginmenu.LoginMenu();
    return 0;

}
	

