#ifndef __DATABASE_ERROR_H__
#define __DATABASE_ERROR_H__

#include "prefix.h"
#include <string>

NAMESPACE_DATABASE_START

class Error
{
public:
    explicit Error() : err_code_(0), err_msg_() {}
    explicit Error(int err_code, const std::string& err_msg)
        : err_code_(err_code),
          err_msg_(err_msg)
    {
    }
    ~Error() {}

    operator bool() { return err_code_ != 0; }
    int Code() { return err_code_; }
    std::string Message() { return err_msg_; }

    operator bool() const { return bool(const_cast<Error&>(*this)); }
    int Code() const { return const_cast<Error&>(*this).Code(); }
    std::string Message() const { return const_cast<Error&>(*this).Message(); }

    friend std::ostream& operator<<(std::ostream& os, const Error& error)
    {
        os << error.Code() << " - " << error.Message();
        return os;
    }

protected:
    int err_code_;
    std::string err_msg_;
};

NAMESPACE_DATABASE_END

#endif