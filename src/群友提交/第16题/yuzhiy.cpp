#include <iostream>

#define BY_NAME(func)   \
[](auto&&... args)      \
{                       \
    return func(std::forward<decltype(args)>(args)...); \
}