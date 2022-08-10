#include <iostream>
#include <iomanip>
#include <string_view>
#include "database/mysql.h"

int main(int argc, char** argv)
{
    std::string user = "user";
    std::string pass = "password";
    std::string host = "127.0.0.1";
    int port = 33123;
    std::string dbname = "user";

    database::MySQL mysql;
    database::Error err = mysql.Connect(user, pass, host, port, dbname);
    if (err)
    {
        std::cout << err << std::endl;
        return 1;
    }
    database::MySQL::QueryResult query_result = mysql.Query("select `id`,`name`,`rank`,`rate` from `user`.`name`");
    if (err = query_result.Error())
    {
        std::cout << err << std::endl;
        return 1;
    }
    std::cout << std::setprecision(10);
    while (database::MySQL::Row row = query_result.Next())
    {
        std::int64_t id = row[0];
        std::string name = row[1];
        int rank = row[2];
        long double rate = row[3];
        std::cout << id << " " << name << " " << rank << " " << rate << std::endl;
    }
    database::MySQL::ExecuteResult execute_result = mysql.Execute(R"(INSERT INTO `user`.`name`(`id`, `name`) VALUES (0, 'user_name');)");
    if (err = execute_result.Error())
    {
        std::cout << err << std::endl;
        return 1;
    }
    std::cout << "affected rows:" << execute_result.AffectedRows() << std::endl;
    return 0;
}