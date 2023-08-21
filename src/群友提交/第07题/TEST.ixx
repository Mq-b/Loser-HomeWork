#include <cstdio>
#include <iostream>
#include <exception>
#include <string>

using namespace std;

class Printer
{
public:
    Printer() {}
	~Printer() {}
    void print(const string_view str) const
	{
		cout << str << endl;
	}
};

class PrinterFactory
{
public:
	PrinterFactory() {}
	~PrinterFactory() {}
	Printer* createPrinter() const
	{
		return new Printer();
	}
};

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
	catch (MyException* e) {
		PrinterFactory pf;
		Printer* p = pf.createPrinter();
		p->print(e->what());
		delete p;
		delete e;
	}
}