#ifndef __DATABASE_MYSQL_H__
#define __DATABASE_MYSQL_H__

#include "prefix.h"
#include "error.h"
#include "mysql/include/mysql.h"
#include "data_wrapper.h"
#include <string_view>

NAMESPACE_DATABASE_START
class MySQL
{
public:
    class Row
    {
    public:
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

            std::int64_t Int64();
            int Int();
            double Double();
            float Float();
            std::string String();
            std::string_view StringView();
            const char* Data();

        protected:
            const char* raw_data_;
        };

        Row(::MYSQL_ROW mysql_row, size_t field_count);
        ~Row();

        Item Get(size_t index);
        Item operator[](size_t index);
        operator bool();
        size_t FieldCount();

    protected:
        ::MYSQL_ROW mysql_row_;
        size_t field_count_;
    };

    class QueryResult
    {
    public:
        explicit QueryResult(const database::Error& error);
        explicit QueryResult(::MYSQL_RES* mysql_res);
        ~QueryResult();

        size_t FieldCount();
        size_t RowsCount();
        Row Next();
        void Free();
        const database::Error& Error();

    protected:
        database::Error error_;
        ::MYSQL_RES* mysql_res_;
        size_t field_count_;
        size_t rows_count_;
    };

    class ExecuteResult
    {
    public:
        explicit ExecuteResult(const database::Error& error);
        explicit ExecuteResult(size_t mysql_affected_rows);
        ~ExecuteResult();

        size_t AffectedRows();
        const database::Error& Error();

    protected:
        database::Error error_;
        size_t affected_rows_;
    };

    MySQL();
    ~MySQL();
    Error Connect(const std::string& user, const std::string& pass,
                  const std::string& host, unsigned short port,
                  const std::string& database);
    QueryResult Query(const std::string& sql);
    ExecuteResult Execute(const std::string& sql);
    void Close();

protected:
    int GetErrorCode();
    std::string GetErrorMessage();

    ::MYSQL mysql_;
    bool is_open_;
};

NAMESPACE_DATABASE_END

#endif
