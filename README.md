# database
A modern,fluent simple and cross-platform C++ mysql database library implemented wit C MySQL API.Runtime safety and no exception.  


## Full Example
```c++
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
```

## Example With Comment
```c++
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

```


## Usage

### MySQL Class
```c++
// header defines in "database/mysql.h"
#include "database/mysql.h"

...

// construct mysql.
database::MySQL mysql;

```

### Connect
```c++
database::Error err = mysql.Connect(user, pass, host, port, dbname);
if(err) 
{
    // handle err
    std::cout << err <<std::endl;
    return 1;
}

```
Call ```database::MySQL::Close()``` to close mysql connection immediately.  
```database::MySQL::Close()``` will also be called when ```database::MySQL``` is being destructed.  

### Query
for select statement,use MySQL::Query

```c++
database::MySQL::QueryResult query_result = mysql.Query("select `id`,`name`,`rank`,`rate` from `user`.`name`");
if (err = query_result.Error())
{
    // handle error
    std::cout << err << std::endl;
}else
{
    while (database::MySQL::Row row = query_result.Next())
    {
        // use legacy api
        std::int64_t id = row[0].Int64();
        // implicit assigment for fluent coding.
        std::string name = row[1];
        int rank = row[2];
        long double rate = row[3];
        std::cout << id << " " << name << " " << rank << " " << rate << std::endl;
    }
}
```
**C assert will be called if the index you set in ```row[index]``` is bigger than row's total fields number**  
**C assert will also be called if you call ```MySQL::QueryResult::Next()``` without checking error or executing no select statement**  

### Execute
for other statement,use MySQL::Execute()
```c++
database::MySQL::ExecuteResult execute_result = mysql.Execute(R"(INSERT INTO `user`.`name`(`id`, `name`) VALUES (0, 'user_name');)");
if (err = execute_result.Error())
{
    //handle error
    std::cout << err << std::endl;
}else{
    std::cout << "affected rows:" << execute_result.AffectedRows() << std::endl;
}
```