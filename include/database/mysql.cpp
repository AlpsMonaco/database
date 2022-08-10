#include "mysql.h"

NAMESPACE_DATABASE_START

MySQL::Row::Row(::MYSQL_ROW mysql_row, size_t field_count)
    : mysql_row_(mysql_row),
      field_count_(field_count)
{
}

MySQL::Row::~Row()
{
}

MySQL::Row::operator bool()
{
    return mysql_row_ != NULL;
}

size_t MySQL::Row::FieldCount()
{
    return field_count_;
}

MySQL::Row::Data::Data(const char* raw_data)
    : raw_data_(raw_data)
{
}

MySQL::Row::Data::~Data()
{
}

MySQL::Row::Data MySQL::Row::Get(size_t index)
{
    ASSERT(index < field_count_, "index is larger than field count");
    return Data(mysql_row_[index]);
}

MySQL::Row::Data MySQL::Row::operator[](size_t index)
{
    return Get(index);
}

int MySQL::Row::Data::Int()
{
    return atoi(raw_data_);
}

std::int64_t MySQL::Row::Data::Int64()
{
    return atoll(raw_data_);
}

double MySQL::Row::Data::Double()
{
    return strtod(raw_data_, NULL);
}

float MySQL::Row::Data::Float()
{
    return strtof(raw_data_, NULL);
}

std::string MySQL::Row::Data::String()
{
    return std::string(raw_data_);
}

const char* MySQL::Row::Data::Raw()
{
    return raw_data_;
}

MySQL::MySQL() : mysql_(), is_open_(false)
{
    mysql_init(&mysql_);
}

MySQL::~MySQL()
{
    Close();
}

void MySQL::Close()
{
    if (is_open_)
    {
        mysql_close(&mysql_);
        is_open_ = false;
    }
}

Error MySQL::Connect(const std::string& user, const std::string& pass,
                     const std::string& host, unsigned short port,
                     const std::string& database)
{
    is_open_ = ::mysql_real_connect(&mysql_, host.c_str(),
                                    user.c_str(), pass.c_str(),
                                    database.c_str(), port,
                                    NULL, 0) != NULL;
    if (!is_open_)
        return Error(GetErrorCode(), GetErrorMessage());
    return Error();
}

MySQL::QueryResult MySQL::Query(const std::string& sql)
{
    if (::mysql_query(&mysql_, sql.c_str()))
        return QueryResult(Error(GetErrorCode(), GetErrorMessage()));
    ::MYSQL_RES* res_ptr = mysql_store_result(&mysql_);
    if (res_ptr == NULL)
        return QueryResult(Error(GetErrorCode(), GetErrorMessage()));
    return QueryResult(res_ptr);
}

MySQL::ExecuteResult MySQL::Execute(const std::string& sql)
{
    if (::mysql_query(&mysql_, sql.c_str()))
        return ExecuteResult(Error(GetErrorCode(), GetErrorMessage()));
    return ExecuteResult(mysql_affected_rows(&mysql_));
}

const database::Error& MySQL::ExecuteResult::Error()
{
    return error_;
}

MySQL::ExecuteResult::ExecuteResult(const database::Error& error)
    : error_(error),
      affected_rows_(0)
{
}

MySQL::ExecuteResult::ExecuteResult(size_t affected_rows)
    : error_(),
      affected_rows_(affected_rows)
{
}

MySQL::ExecuteResult::~ExecuteResult()
{
}

size_t MySQL::ExecuteResult::AffectedRows()
{
    return affected_rows_;
}

int MySQL::GetErrorCode()
{
    return mysql_errno(&mysql_);
}

std::string MySQL::GetErrorMessage()
{
    return mysql_error(&mysql_);
}

MySQL::QueryResult::QueryResult(const database::Error& error)
    : error_(error),
      mysql_res_(NULL),
      field_count_(0),
      rows_count_(0)
{
}

MySQL::QueryResult::QueryResult(::MYSQL_RES* mysql_res)
    : error_(),
      mysql_res_(mysql_res),
      field_count_(mysql_num_fields(mysql_res)),
      rows_count_(mysql_num_rows(mysql_res))
{
}

size_t MySQL::QueryResult::RowsCount()
{
    return rows_count_;
}

void MySQL::QueryResult::Free()
{
    if (mysql_res_ != NULL)
    {
        ::mysql_free_result(mysql_res_);
        mysql_res_ = NULL;
    }
}

MySQL::Row MySQL::QueryResult::Next()
{
    ASSERT(mysql_res_ != NULL, "prior query was error or no sql select statement");
    return Row(mysql_fetch_row(mysql_res_), field_count_);
}

MySQL::QueryResult::~QueryResult()
{
    Free();
}

const database::Error& MySQL::QueryResult::Error()
{
    return error_;
}

size_t MySQL::QueryResult::FieldCount()
{
    return field_count_;
}

NAMESPACE_DATABASE_END