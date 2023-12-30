import std;

class ComponentBase {
protected:
    static inline size_t component_type_count = 0;
};

template <typename T>
class Component : public ComponentBase {
public:
    static auto component_type_id() {
        static size_t component_type_id = component_type_count++;
        return component_type_id;
    }
};

class A : public Component<A> {};

class B : public Component<B> {};

class C : public Component<C> {};

#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "A::id() = " << A::component_type_id() << std::endl;
    std::cout << "B::id() = " << B::component_type_id() << std::endl;
    std::cout << "B::id() = " << B::component_type_id() << std::endl;
    std::cout << "A::id() = " << A::component_type_id() << std::endl;
    std::cout << "A::id() = " << A::component_type_id() << std::endl;
    std::cout << "C::id() = " << C::component_type_id() << std::endl;
    return 0;
}