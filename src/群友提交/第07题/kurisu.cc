#include <iostream>
#include <stdio.h>

struct MyException :std::exception {
    const char* data{};
    MyException(const char* s) :data(s) { puts("MyException()"); }
    ~MyException() { puts("~MyException()"); }
    const char* what()const noexcept { return data; }
};

void f2() {
    throw new MyException("new Exception异常....");
}

struct except_guard{
	except_guard(std::exception *& ptr):_ptr(ptr){}
	~except_guard(){
		delete _ptr;
		_ptr=nullptr;
	}
private:
	std::exception *&_ptr;
};

int main(){
    try {
        f2();
    } catch (std::exception* e) {
		auto guard=except_guard(e);
		std::cerr<<e->what()<<'\n';
    }
}
