#include<iostream>
#include <map>
class ComponentBase {
protected:
    static inline size_t component_type_count = 0;
};

template<typename T>
class Component : public ComponentBase {
public:
    //todo...
    //使用任意方式更改当前模板类，使得对于任意类型X，若其继承自Component

    //则X::component_type_id()会得到一个独一无二的size_t类型的id（对于不同的X类型返回的值应不同）
    //要求：不能使用std::type_info（禁用typeid关键字），所有id从0开始连续。

    static size_t component_type_id() {
        static size_t type_id = component_type_count++;
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