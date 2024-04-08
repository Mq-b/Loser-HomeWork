//
// loser_homework / brian.cpp
// Created by brian on 2024 Apr 03.
//
#include<iostream>

class ComponentBase {
protected:
  static inline std::size_t component_type_count = 0;
};

template<typename T>
class Component : public ComponentBase {
public:
  static size_t component_type_id() {
    static size_t unique_id = component_type_count++;
    return unique_id;
  }
};

class A : public Component<A> {};

class B : public Component<B> {};

class C : public Component<C> {};

int main() {
  std::cout << A::component_type_id() << std::endl;
  std::cout << B::component_type_id() << std::endl;
  std::cout << B::component_type_id() << std::endl;
  std::cout << A::component_type_id() << std::endl;
  std::cout << A::component_type_id() << std::endl;
  std::cout << C::component_type_id() << std::endl;
}