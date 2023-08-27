#include <stddef.h>

class ComponentBase {
protected:
    static inline size_t derive_counter__ = 0;
};

template <typename T>
class Component : public ComponentBase {
public:
    static auto id() {
        return private_counter_;
    }

private:
    static inline size_t private_counter_ = ComponentBase::derive_counter__++;
};

class A : public Component<A> {};

class B : public Component<B> {};

class C : public Component<C> {};

#include <iostream>

int main(int argc, char *argv[]) {
    std::cout << "A::id() = " << A::id() << std::endl;
    std::cout << "B::id() = " << B::id() << std::endl;
    std::cout << "B::id() = " << B::id() << std::endl;
    std::cout << "A::id() = " << A::id() << std::endl;
    std::cout << "A::id() = " << A::id() << std::endl;
    std::cout << "C::id() = " << C::id() << std::endl;
    return 0;
}
