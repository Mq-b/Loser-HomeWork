#include <iostream>
struct MyException :std::exception {
 const char* data{};
 MyException(const char* s) :data(s) { puts("MyException()"); }
 ~MyException() { puts("~MyException()"); }
 const char* what()const noexcept { return data; }
};
void f2() {
 throw new MyException("new Excepti...");
}
int main(){
    // f2();
    try {
        f2();
    } catch (std::exception* e) {
        std::cout << e->what() << std::endl;
        delete e;
    }
}
