#pragma once
#include <mysql/jdbc.h>


class mySQL_db
{
public:
    mySQL_db();
    ~mySQL_db();

    void prepareStatement(std::string query);
    sql::PreparedStatement* statement;
    sql::ResultSet* result;
    void QueryStatement();
    void QueryResult();
    void executeQuery();
private:
    sql::Connection* connection;
};