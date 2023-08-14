#include <iostream>

struct MyException :std::exception {
    const char* data{};
    MyException(const char* s) :data(s) { puts("MyException()"); }
    ~MyException() { puts("~MyException()"); }
    const char* what()const noexcept { return data; }
};
void f2() {
    throw new MyException("new Exception异常....");
}

int main() {
    try
    {
        f2();
    }
    catch (MyException* e)
    {
        if (e)
        {
            puts(e->what());
            delete e;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "recive exception\n";
    }
    catch (...)
    {
        std::cout << "recive ...\n";
    }
}