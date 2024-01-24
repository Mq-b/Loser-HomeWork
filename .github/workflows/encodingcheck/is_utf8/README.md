# is_utf8

Most strings online are in unicode using the UTF-8 encoding. Validating strings
quickly before accepting them is important.

## How to use is_utf8

This is a simple one-source file library to validate UTF-8 strings at high
speeds using SIMD instructions. It works on all platforms (ARM, x64).

Build and link `is_utf8.cpp` with your project. Code usage:

```C++
  #include "is_utf8.h"

  char * mystring = ...
  bool is_it_valid = is_utf8(mystring, thestringlength);
```

It should be able to validate strings using less than 1 cycle per input byte.

## Requirements

- C++11 compatible compiler. We support LLVM clang, GCC, Visual Studio. (Our
  optional benchmark tool requires C++17.)
- For high speed, you should have a recent 64-bit system (e.g., ARM or x64).
- If you rely on CMake, you should use a recent CMake (at least 3.15).
- AVX-512 support require a processor with AVX512-VBMI2 (Ice Lake or better) and
  a recent compiler (GCC 8 or better, Visual Studio 2019 or better, LLVM clang 6
  or better). You need a correspondingly recent assembler such as gas (2.30+) or
  nasm (2.14+): recent compilers usually come with recent assemblers. If you mix
  a recent compiler with an incompatible/old assembler (e.g., when using a
  recent compiler with an old Linux distribution), you may get errors at build
  time because the compiler produces instructions that the assembler does not
  recognize: you should update your assembler to match your compiler (e.g.,
  upgrade binutils to version 2.30 or better under Linux) or use an older
  compiler matching the capabilities of your assembler.

## Build with CMake

```
cmake -B build
cmake --build build
cd build
ctest .
```

Visual Studio users must specify whether they want to build the Release or Debug
version.

To run benchmarks, build and execute the `bench` command.

```
cmake -B build
cmake --build build
./build/benchmarks/bench
```

Instructions are similar for Visual Studio users.

## Real-word usage

This C++ library is part of the JavaScript package
[utf-8-validate](https://github.com/websockets/utf-8-validate). The
utf-8-validate package is routinely downloaded more than
[a million times per week](https://www.npmjs.com/package/utf-8-validate).

If you are using Node JS (19.4.0 or better), you already have access to this
function as
[`buffer.isUtf8(input)`](https://nodejs.org/api/buffer.html#bufferisutf8input).

## Reference

- John Keiser, Daniel Lemire,
  [Validating UTF-8 In Less Than One Instruction Per Byte](https://arxiv.org/abs/2010.03090),
  Software: Practice & Experience 51 (5), 2021

## Want more?

If you want a wide range of fast Unicode function for production use, you can
rely on the simdutf library. It is as simple as the following:

```C++
#include "simdutf.cpp"
#include "simdutf.h"

int main(int argc, char *argv[]) {
  const char *source = "1234";
  // 4 == strlen(source)
  bool validutf8 = simdutf::validate_utf8(source, 4);
  if (validutf8) {
    std::cout << "valid UTF-8" << std::endl;
  } else {
    std::cerr << "invalid UTF-8" << std::endl;
    return EXIT_FAILURE;
  }
}
```

See https://github.com/simdutf/

## License

This library is distributed under the terms of any of the following licenses, at
your option:

- Apache License (Version 2.0) [LICENSE-APACHE](LICENSE-APACHE),
- Boost Software License [LICENSE-BOOST](LICENSE-BOOST), or
- MIT License [LICENSE-MIT](LICENSE-MIT).
