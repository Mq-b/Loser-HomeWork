#include <bits/stdc++.h>
struct MyException : std::exception {
    const char* data{};
    MyException(const char* s) :data(s) { puts("MyException()"); }
    ~MyException() { puts("~MyException()"); }
    const char* what() const noexcept { return data; }
};
void f2() {
    throw new MyException("new Exception异常....");
}
int main() {
    try {
        std::invoke(f2);
    }
    catch (const MyException* &e) {
        puts(e->what());
        delete e;
    }
    catch(...){
        puts("others exception.");
    }

    return 0;
}
