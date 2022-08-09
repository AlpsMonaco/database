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

MySQL::Row::Item::Item(const char* raw_data)
    : raw_data_(raw_data)
{
}

MySQL::Row::Item::~Item()
{
}

MySQL::Row::Item MySQL::Row::Get(size_t index)
{
    return Item(index < field_count_ ? mysql_row_[index] : zero_value.data());
}

MySQL::Row::Item MySQL::Row::operator[](size_t index)
{
    return Get(index);
}

int MySQL::Row::Item::AsInt()
{
    return atoi(raw_data_);
}

double MySQL::Row::Item::AsDouble()
{
    return strtod(raw_data_, NULL);
}

float MySQL::Row::Item::AsFloat()
{
    return strtof(raw_data_, NULL);
}

std::string MySQL::Row::Item::AsString()
{
    return std::string(raw_data_);
}

const char* MySQL::Row::Item::Data()
{
    return raw_data_;
}

MySQL::MySQL() : mysql_(), is_open_(false)
{
    mysql_init(&mysql_);
}

MySQL::~MySQL()
{
    if (is_open_) mysql_close(&mysql_);
}

int MySQL::Errno()
{
    return mysql_errno(&mysql_);
}

std::string MySQL::Error()
{
    return mysql_error(&mysql_);
}

bool MySQL::Connect(const std::string& user, const std::string& pass,
                    const std::string& host, unsigned short port,
                    const std::string& database)
{
    is_open_ = ::mysql_real_connect(&mysql_, host.c_str(),
                                    user.c_str(), pass.c_str(),
                                    database.c_str(), port,
                                    NULL, 0) != NULL;
    return is_open_;
}

MySQL::Result MySQL::Query(const std::string& sql)
{
    if (::mysql_query(&mysql_, sql.c_str()))
        return Result(Errno(), Error());
    ::MYSQL_RES* res_ptr = mysql_store_result(&mysql_);
    if (res_ptr == NULL)
        return Result(Errno(), Error());
    return Result(res_ptr);
}

MySQL::Result::Result(int errcode, const std::string& error)
    : errno_(errno),
      error_(error),
      mysql_res_(nullptr),
      field_count_(0)
{
}

MySQL::Result::Result(::MYSQL_RES* mysql_res)
    : errno_(0),
      error_(),
      mysql_res_(mysql_res),
      field_count_(mysql_num_fields(mysql_res))
{
}

void MySQL::Result::Free()
{
    if (mysql_res_ != NULL)
    {
        ::mysql_free_result(mysql_res_);
        mysql_res_ = NULL;
    }
}

MySQL::Row MySQL::Result::Next()
{
    return Row(mysql_fetch_row(mysql_res_), field_count_);
}

MySQL::Result::~Result()
{
    Free();
}

size_t MySQL::Result::FieldCount()
{
    return field_count_;
}

int MySQL::Result::Errno()
{
    return errno_;
}

std::string MySQL::Result::Error()
{
    return error_;
}

NAMESPACE_DATABASE_END