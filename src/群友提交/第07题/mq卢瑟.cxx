#include <iostream>
#include <memory>

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
	try {
		f2();
	}
	catch (std::exception *e)
	{
		std::cout << std::unique_ptr<std::exception>(e)->what() << '\n';
	}
}
