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


/* A recommended version! */
namespace dark {

/**
 * @brief Actually you'd better use override
 * to explicitly demostrate the type of the function......
 * Anyway, the behaviors are all the same :(
 * 
*/
struct error : std::exception {
    std::string data;
    error(std::string_view __str) : data(__str) { puts("error()"); }
    ~error() override { puts("~error()"); }
    const char *what() const noexcept override { return data.data(); }
};

/* Too lazy to complete...... */
struct unknown_error { std::string data; };

}

/**
 * @brief A better version, I think......
 * Actually, I don't know why throwing a pointer......
 * 
*/
void f1() {
    throw dark::error("new Exception异常....");
}

void f3() { throw dark::unknown_error {.data = "What the f**k is that?"}; }

void f4() { throw "?"; }

signed main(){
    try {
        try {
            // f1(); // what i recommend
            f2(); // std
            // f3(); // test for unknown error
            f4();
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
    } catch (dark::unknown_error &__exception) {
        std::cout << __exception.data << '\n';
    } catch (...) {
        puts("Program panic!");
        return 1; /* Abnormal exit. */
    }

    return 0; /* Normal case. */
}
