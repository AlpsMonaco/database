#ifndef __DATABASE_PREFIX_H__
#define __DATABASE_PREFIX_H__

#define NAMESPACE_DATABASE_START \
    namespace database           \
    {

#define NAMESPACE_DATABASE_END }

#include <cassert>
#include <iostream>

#define ASSERT(expression, what)            \
    do                                      \
    {                                       \
        if (!(expression))                  \
        {                                   \
            std::cout << what << std::endl; \
            assert(false);                  \
        }                                   \
    } while (0);

#endif