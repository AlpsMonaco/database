#ifndef __DATABASE_ERROR_H__
#define __DATABASE_ERROR_H__

#include "prefix.h"
#include <string>

NAMESPACE_DATABASE_START

class Error
{
public:
    Error(int err_code, const std::string& err_msg)
        : err_code_(err_code),
          err_msg_(err_msg)
    {
    }
    ~Error() {}

    operator bool() { return err_code_ != 0; }
    int Code() { return err_code_; }
    std::string Message() { return err_msg_; }

protected:
    int err_code_;
    std::string err_msg_;
};

NAMESPACE_DATABASE_END

#endif