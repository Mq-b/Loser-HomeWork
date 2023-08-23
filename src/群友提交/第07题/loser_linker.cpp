#include <cstdio>
#include <memory>
#include <stdexcept>

struct MyException : std::exception {
    char const * data{ };

    MyException(const char * str) 
        :data{ str } { 
        puts("MyException()"); 
    }
    ~MyException() { 
        puts("~MyException()"); 
    }

    char const * what() const noexcept override { return data; }
};

void f2() {
    throw new MyException("new Exception异常....");
}

int main() {
    try {
        f2();
    }
    catch (MyException* e) {
        auto resource = std::unique_ptr<MyException>(e);

        puts(resource->what());
    }
}