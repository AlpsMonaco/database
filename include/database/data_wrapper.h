#ifndef __DATABASE_DATA_WRAPPER_H__
#define __DATABASE_DATA_WRAPPER_H__

#include "prefix.h"
#include <type_traits>
#include <string>

NAMESPACE_DATABASE_START

template <typename T, typename = void>
struct DataWrapper
{
    DataWrapper(const char* raw_data) : raw_data_(raw_data) {}
    ~DataWrapper() {}
    T operator()() { return raw_data_; }

protected:
    const char* raw_data_;
};

template <typename T>
struct DataWrapper<T,
                   typename std::enable_if<std::is_integral<T>::value>::type>
{
    DataWrapper(const char* raw_data) : raw_data_(raw_data) {}
    ~DataWrapper() {}
    T operator()() { return ::atoll(raw_data_); }

protected:
    const char* raw_data_;
};

template <typename T>
struct DataWrapper<T,
                   typename std::enable_if<std::is_floating_point<T>::value>::type>
{
    DataWrapper(const char* raw_data) : raw_data_(raw_data) {}
    ~DataWrapper() {}
    T operator()() { return ::strtold(raw_data_, NULL); }

protected:
    const char* raw_data_;
};

NAMESPACE_DATABASE_END

#endif