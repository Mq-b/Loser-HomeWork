#include <iostream>

class ComponentBase {
protected:
    static inline size_t component_type_count = 0;
};

template <typename T>
class Component : public ComponentBase {
public:
    inline static size_t myID = component_type_count++;
    static size_t component_type_id()
    {
        return myID;
    }
};

class A : public Component<A> {
};

class B : public Component<B> {
};

class C : public Component<C> {
};

int main()
{
    std::cout << A::component_type_id() << std::endl;
    std::cout << B::component_type_id() << std::endl;
    std::cout << B::component_type_id() << std::endl;
    std::cout << A::component_type_id() << std::endl;
    std::cout << A::component_type_id() << std::endl;
    std::cout << C::component_type_id() << std::endl;
}
