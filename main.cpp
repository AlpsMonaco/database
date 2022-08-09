#include <iostream>
#include <string_view>
#include "database/mysql.h"

int main(int argc, char** argv)
{
    std::string user = "hotgame";
    std::string pass = "hotgame82year.HL";
    std::string host = "127.0.0.1";
    int port = 33123;
    std::string dbname = "hotgame_hl40";
    database::MySQL mysql;
    if (!mysql.Connect(user, pass, host, port, dbname))
    {
        std::cout << mysql.Errno() << " " << mysql.Error() << std::endl;
        return 1;
    }
    database::MySQL::Result res = mysql.Query("select id,name,level from d_user limit 10");
    while (auto row = res.Next())
    {
        std::int64_t id = row[0];
        std::string name = row[1];
        std::int64_t level = row[2];
        std::cout << id << " " << name << " " << level << std::endl;
    }
    return 0;
}