#include<iostream>
class ComponentBase {
protected:
    static inline std::size_t component_type_count = 0;
};
template<typename T>
class Component : public ComponentBase {
public:
    static std::size_t component_type_id() {
        static std::size_t id = component_type_count++;
        return id;
    }
};

class A : public Component<A>
{};
class B : public Component<B>
{};
class C : public Component<C>
{};
int main()
{
    std::cout << A::component_type_id() << std::endl;
    std::cout << B::component_type_id() << std::endl;
    std::cout << B::component_type_id() << std::endl;
    std::cout << A::component_type_id() << std::endl;
    std::cout << A::component_type_id() << std::endl;
    std::cout << C::component_type_id() << std::endl;
}