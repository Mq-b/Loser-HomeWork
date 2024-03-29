#include <iostream>
class ComponentBase
{
protected:
    static inline std::size_t component_type_count = 0;
};
template <typename T>
class Component : public ComponentBase
{
public:
    static std::size_t component_type_id()
    {
        static  std::size_t type_id = component_type_count++;
        return type_id;
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
// 0 1 1 0 0 2