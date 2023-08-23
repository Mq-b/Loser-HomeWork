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
    //ʹ�����ⷽʽ���ĵ�ǰģ���࣬ʹ�ö�����������X������̳���Component

    //��X::component_type_id()��õ�һ����һ�޶���size_t���͵�id�����ڲ�ͬ��X���ͷ��ص�ֵӦ��ͬ��
    //Ҫ�󣺲���ʹ��std::type_info������typeid�ؼ��֣�������id��0��ʼ������

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