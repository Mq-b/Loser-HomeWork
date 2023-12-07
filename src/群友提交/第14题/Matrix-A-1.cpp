#include<iostream>
#include<cstdint>
#include<vector>
#include<fstream>
#include<filesystem>

namespace ssy {
    int a = 0;
}

[[msvc::noinline, gnu::noinline]]void test(std::uintptr_t main) {
    std::uint8_t* addr{ nullptr };
    if(*(std::uint8_t*)main == 0xe9)
        addr = (std::uint8_t*)main + *(uint8_t*)(main + 1) + *(uint8_t*)(main + 2) * 0x100 + 5;
    else
        addr = (std::uint8_t*)main;
    for (std::size_t idx{ 0 }; idx < 0x64; idx++) {
        if (addr[idx] == 0xe8 && addr[idx + 5] == 0x8b) { // call is 0xe8 and mov is 0x8b
            int* addr_a{ (int*)(addr + idx + 5 + 6 + addr[idx + 8] * 0x100 + addr[idx + 7]) };
            *addr_a = 100;
            break;
        }
    }
}
int main() {
    test((std::uintptr_t)main);
    std::cout << ssy::a << '\n';
}
