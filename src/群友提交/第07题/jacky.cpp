#include <cstdio>
#include <iostream>
#include <stdexcept>

struct MyException : std::exception {
    const char* data{};

    explicit MyException(const char* s)
        : data(s)
    {
        std::puts("MyException()");
    }

    ~MyException() override
    {
        std::puts("~MyException()");
    }

    [[nodiscard]] const char* what() const noexcept override
    {
        return data;
    }
};

void f2()
{
    throw new MyException("new Exception异常....");
}

int main()
{
    try {
        f2();
    } catch (std::exception* except) {
        std::puts(except->what());
        delete except;
        except = nullptr;
    }
    return 0;
}
