#include <iostream>
#include <string_view>
#include "database/mysql.h"

int main(int argc, char** argv)
{
    std::string user = "user";
    std::string pass = "password";
    std::string host = "127.0.0.1";
    int port = 3306;
    std::string dbname = "user";
    
    database::MySQL mysql;
    database::Error err = mysql.Connect(user, pass, host, port, dbname);
    if (err)
    {
        std::cout << err.Code() << " " << err.Message() << std::endl;
        return 1;
    }
    database::MySQL::QueryResult query_result = mysql.Query("select count(0) * from d_user");
    if (err = query_result.Error())
    {
        std::cout << err.Code() << " " << err.Message() << std::endl;
        return 1;
    }
    while (database::MySQL::Row row = query_result.Next())
    {
        std::int64_t id = row[0];
        std::string name = row[1];
        std::int64_t level = row[2];
        std::cout << id << " " << name << " " << level << std::endl;
    }
    database::MySQL::ExecuteResult execute_result = mysql.Execute(R"(INSERT INTO `hotgame_hl40`.`cmd_d`(`id`, `type`, `user_id`, `eff_seconds`, `state`, `start_time`, `create_Time`, `txt`) VALUES (0, 24, 3700001009900, 0, 2, 0, '2022-08-01 16:46:10', '{\"idMatch\":3700001011449,\"idUser\":3700001009900,\"nEndTime\":1659386757,\"nItemNum\":3,\"nItemType\":3001003}') on duplicate key update txt = '123';)");
    if (err = execute_result.Error())
    {
        std::cout << err.Code() << " " << err.Message() << std::endl;
        return 1;
    }
    std::cout << "affected rows:" << execute_result.AffectedRows() << std::endl;
    return 0;
}