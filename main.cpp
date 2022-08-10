#include <iostream>
#include <iomanip>
#include <string_view>
#include "database/mysql.h"

int main(int argc, char** argv)
{
    // mysql connection info.
    std::string user = "user";
    std::string pass = "password";
    std::string host = "127.0.0.1";
    int port = 33123;
    std::string dbname = "user";

    // construct MySQL instance without any parameter.
    database::MySQL mysql;

    // err could be cast to bool type.
    // if err == true, error exists while trying to connect to MySQL server.
    database::Error err = mysql.Connect(user, pass, host, port, dbname);
    if (err)
    {
        std::cout << err << std::endl;
        return 1;
    }

    // for select statement,use MySQL::Query.
    // return MySQL::QueryResult
    database::MySQL::QueryResult query_result = mysql.Query("select `id`,`name`,`rank`,`rate` from `user`.`name`");
    // call MySQL::QueryResult::Error() to check whether there's error or not.
    if (err = query_result.Error())
    {
        std::cout << err << std::endl;
        return 1;
    }
    std::cout << std::setprecision(10);

    // call MySQL::QueryResult::Next() to get next row.
    // MySQL::Row could be cast to bool type indicates whether there
    // are further rows to be fetch or not.
    while (database::MySQL::Row row = query_result.Next())
    {
        // MySQL::Row provides legacy api like MySQL::Row::Int(),MySQL::Row::Int64(),
        // MySQL::Row::Double(),MySQL::Row::String(),etc to fetch data directly.
        // std::int64_t id = row.Get(0).Int64();
        // std::int64_t id = row[0].Int64();

        // But for fluent use and clear code look,
        // it is recommand to use implicit data assigment below.
        // this word properly using SFINAE.
        std::int64_t id = row[0];
        // on c++17 or later,consider using std::string_view for better performance
        // std::string_view name = row[1];
        std::string name = row[1];
        int rank = row[2];
        long double rate = row[3];
        std::cout << id << " " << name << " " << rank << " " << rate << std::endl;
    }

    // for other statement,use MySQL::Exexute.
    // return MySQL::ExecuteResult
    database::MySQL::ExecuteResult execute_result = mysql.Execute(R"(INSERT INTO `user`.`name`(`id`, `name`) VALUES (0, 'user_name');)");
    // call MySQL::ExecuteResult::Error() to check whether there's error or not.
    if (err = execute_result.Error())
    {
        std::cout << err << std::endl;
        return 1;
    }
    // call MySQL::ExecuteResult::AffectedRows() to get the number of affected rows.
    std::cout << "affected rows:" << execute_result.AffectedRows() << std::endl;
    return 0;
}