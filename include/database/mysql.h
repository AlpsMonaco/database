#ifndef __DATABASE_MYSQL_H__
#define __DATABASE_MYSQL_H__

#include "prefix.h"
#include "mysql/include/mysql.h"
#include "data_wrapper.h"
#include <string>
#include <string_view>

NAMESPACE_DATABASE_START
class MySQL
{
public:
    class Row
    {
    public:
        static constexpr std::string_view zero_value = "";
        class Item
        {
        public:
            Item(const char* raw_data);
            ~Item();

            template <typename T>
            operator T()
            {
                return DataWrapper<T>(raw_data_)();
            }

            int AsInt();
            double AsDouble();
            float AsFloat();
            std::string AsString();
            const char* Data();

        protected:
            const char* raw_data_;
        };

        Row(::MYSQL_ROW mysql_row, size_t field_count);
        ~Row();

        Item Get(size_t index);
        Item operator[](size_t index);
        operator bool();

    protected:
        ::MYSQL_ROW mysql_row_;
        size_t field_count_;
    };

    class Result
    {
    public:
        Result(int errcode, const std::string& error);
        Result(::MYSQL_RES* mysql_res);
        ~Result();

        int Errno();
        std::string Error();
        size_t FieldCount();
        Row Next();
        void Free();

    protected:
        int errno_;
        std::string error_;
        ::MYSQL_RES* mysql_res_;
        size_t field_count_;
    };

    MySQL();
    ~MySQL();
    bool Connect(const std::string& user, const std::string& pass,
                 const std::string& host, unsigned short port,
                 const std::string& database);
    int Errno();
    std::string Error();
    Result Query(const std::string& sql);

protected:
    ::MYSQL mysql_;
    bool is_open_;
};

NAMESPACE_DATABASE_END

#endif
