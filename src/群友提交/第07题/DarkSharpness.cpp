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


signed main(){
    try {
        f2(); // std
    } catch(std::exception *__exception) {
        /* NEVER catch the derived class unless necessary. */
        puts(__exception->what());
        delete __exception;
    } catch(std::exception &__exception) {
        /* Use reference type to catch the virtual class. */
        puts(__exception.what());
        puts("This version is what I recommend.");
    }  catch(...) {
        puts("Unknown error!");
        throw ; /* It will throw the error caught. */
    }
    return 0; /* Normal case. */
}
