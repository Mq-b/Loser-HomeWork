#include <iostream>
#include <stdexcept>
#include <exception>

struct MyException :std::exception {
	const char* data{};
	MyException(const char* s) :data(s) { puts("MyException()"); }
	~MyException() { puts("~MyException()"); }
	const char* what()const noexcept { return data; }
};
void f2() {
	throw new MyException("new Exception异常....");
}
int main(){
    try {
        f2();
    } catch (MyException *e) {
        std::cerr << e->what() << std::endl;
        delete e;
    }
}
