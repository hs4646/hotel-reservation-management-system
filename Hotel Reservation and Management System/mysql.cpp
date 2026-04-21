#include "mysql.h"
#include <conio.h>
using namespace std;
using namespace sql;

mySQL_db::mySQL_db() {
	try
	{
		mysql::MySQL_Driver* driver = mysql::get_mysql_driver_instance();
		connection = driver->connect("localhost", "root", "12345");

		connection->setSchema("hotel_reservation_and_management");
		statement = nullptr;
		result = nullptr;
	}
	catch (sql::SQLException& e)
	{
		if (e.getErrorCode() == 0) {
			system("cls");
			cout << "Unable to connect to database";
			_getch();
			exit(0);
			return;
		}

		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MYSQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		_getch();
	}
}

mySQL_db::~mySQL_db() {
	try {
		if (connection) {
			connection->close();
		}
		if (statement) {
			statement->close();
		}
		if (result) {
			result->close();
		}
	}
	catch (sql::SQLException& e)
	{
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MYSQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		_getch();
	}
}

void mySQL_db::prepareStatement(string query) {
	statement = connection->prepareStatement(query);
}

void mySQL_db::QueryStatement() {
	try {
		statement->executeUpdate();
	}
	catch (sql::SQLException& e)
	{
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MYSQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		_getch();
	}
}

void mySQL_db::QueryResult() {
	try {
		result = statement->executeQuery();
	}
	catch (sql::SQLException& e)
	{
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MYSQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		_getch();
	}

}

