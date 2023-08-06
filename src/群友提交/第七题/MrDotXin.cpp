/*  第七题
    群ID : 还没想好名字
*/
#include <iostream>
#include <exception>

struct MyException : std::exception {
    const char * data {};
    explicit MyException(const char * s) : data(s) {puts("MyException()"); }
    ~MyException() { puts("~MyException()"); }
    const char * what() const noexcept {return data; }
};

void f2() {
    throw new MyException("new Exception异常...");
}

int main() {
    try { f2(); }
    catch (MyException * exec) {
        puts(exec->what());
        delete exec;
    }

    return 0;
}