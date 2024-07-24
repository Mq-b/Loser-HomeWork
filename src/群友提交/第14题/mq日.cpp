#include <iostream>

namespace ss
{
	int a = 0;
	namespace s
	{
		int& x = a;
	}
}

int main()
{
	::ss::s::x = 100;
	// todo..
	std::cout << ss::a << '\n';
}