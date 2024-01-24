#include "is_utf8.h"
#include "simdutf.h"
#include <assert.h>
#include <chrono>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

#ifdef _MSC_VER
#define never_inline __declspec(noinline)
#else
#define never_inline __attribute__((noinline))
#endif

// generate a string having at least length N
// can exceed by up to 3 chars, returns the actual length
size_t populate_utf8(char *data, size_t N) {
  size_t i = 0;
  for (; i < N;) {
    int w = rand() & 0xFF;
    if (w < 0x80) {
      data[i++] = 0x20; // w;
    } else if (w < 0xE0) {
      data[i++] = 0xC2 + rand() % (0xDF - 0xC2 + 1);
      data[i++] = 0x80 + rand() % (0xBF - 0x80 + 1);
    } else if (w == 0xE0) {
      data[i++] = w;
      data[i++] = 0xA0 + rand() % (0xBF - 0xA0 + 1);
      data[i++] = 0x80 + rand() % (0xBF - 0x80 + 1);
    } else if (w <= 0xEC) {
      data[i++] = w;
      data[i++] = 0x80 + rand() % (0xBF - 0x80 + 1);
      data[i++] = 0x80 + rand() % (0xBF - 0x80 + 1);
    } else if (w == 0xED) {
      data[i++] = w;
      data[i++] = 0x80 + rand() % (0x9F - 0x80 + 1);
      data[i++] = 0x80 + rand() % (0xBF - 0x80 + 1);
    } else if (w <= 0xEF) {
      data[i++] = w;
      data[i++] = 0x80 + rand() % (0xBF - 0x80 + 1);
      data[i++] = 0x80 + rand() % (0xBF - 0x80 + 1);
    } else if (w < 0xF0) {
      data[i++] = 0xF1 + rand() % (0xF3 - 0xF1 + 1);
      data[i++] = 0x80 + rand() % (0xBF - 0x80 + 1);
      data[i++] = 0x80 + rand() % (0xBF - 0x80 + 1);
      data[i++] = 0x80 + rand() % (0xBF - 0x80 + 1);
    } else if (w == 0xF0) {
      data[i++] = w;
      data[i++] = 0x90 + rand() % (0xBF - 0x90 + 1);
      data[i++] = 0x80 + rand() % (0xBF - 0x80 + 1);
      data[i++] = 0x80 + rand() % (0xBF - 0x80 + 1);
    } else if (w <= 0xF3) {
      data[i++] = 0xF1 + rand() % (0xF3 - 0xF1 + 1);
      data[i++] = 0x80 + rand() % (0xBF - 0x80 + 1);
      data[i++] = 0x80 + rand() % (0xBF - 0x80 + 1);
      data[i++] = 0x80 + rand() % (0xBF - 0x80 + 1);
    } else if (w == 0xF4) {
      data[i++] = w;
      data[i++] = 0x80 + rand() % (0x8F - 0x80 + 1);
      data[i++] = 0x80 + rand() % (0xBF - 0x80 + 1);
      data[i++] = 0x80 + rand() % (0xBF - 0x80 + 1);
    }
  }
  data[i] = '\0';
  return i;
}

// copied in part from Guava
static never_inline bool basic_validate_utf8(const char *b, size_t length) {
  const unsigned char *bytes = (const unsigned char *)b;
  for (size_t index = 0;;) {
    unsigned char byte1;

    do { // fast ASCII Path
      if (index >= length) {
        return true;
      }
      byte1 = bytes[index++];
    } while (byte1 < 0x80);
    if (byte1 < 0xE0) {
      // Two-byte form.
      if (index == length) {
        return false;
      }
      if (byte1 < 0xC2 || bytes[index++] > 0xBF) {
        return false;
      }
    } else if (byte1 < 0xF0) {
      // Three-byte form.
      if (index + 1 >= length) {
        return false;
      }
      unsigned char byte2 = bytes[index++];
      if (byte2 > 0xBF
          // Overlong? 5 most significant bits must not all be zero.
          || (byte1 == 0xE0 && byte2 < 0xA0)
          // Check for illegal surrogate codepoints.
          || (byte1 == 0xED && 0xA0 <= byte2)
          // Third byte trailing-byte test.
          || bytes[index++] > 0xBF) {
        return false;
      }
    } else {

      // Four-byte form.
      if (index + 2 >= length) {
        return false;
      }
      int byte2 = bytes[index++];
      if (byte2 > 0xBF
          // Check that 1 <= plane <= 16. Tricky optimized form of:
          // if (byte1 > (byte) 0xF4
          //     || byte1 == (byte) 0xF0 && byte2 < (byte) 0x90
          //     || byte1 == (byte) 0xF4 && byte2 > (byte) 0x8F)
          || (((byte1 << 28) + (byte2 - 0x90)) >> 30) != 0
          // Third byte trailing-byte test
          || bytes[index++] > 0xBF
          // Fourth byte trailing-byte test
          || bytes[index++] > 0xBF) {
        return false;
      }
    }
  }
}


bool zerobuffer_bench(size_t N) {
  printf("zero buffer \n");
  printf("string size = %zu \n", N);
  char *input = new char[N]{};
  volatile bool isgood{true};

  {
    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    uint64_t threshold = 500000000;
    for (; finish - start < threshold;) {
      count++;
      isgood &= basic_validate_utf8(input, N);
      finish = nano();
    }
    double t = (N * count) / double(finish - start);

    printf("basic_validate_utf8   %f GB/s\n", t);
  }

  {
    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    uint64_t threshold = 500000000;
    for (; finish - start < threshold;) {
      count++;
      isgood &= simdutf::validate_utf8(input, N);
      finish = nano();
    }
    double t = (N * count) / double(finish - start);

    printf("simdutf               %f GB/s\n", t);
  }

  {
    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    uint64_t threshold = 500000000;
    for (; finish - start < threshold;) {
      count++;
      isgood &= is_utf8(input, N);
      finish = nano();
    }
    double t = (N * count) / double(finish - start);

    printf("is_utf8               %f GB/s\n", t);
  }
  delete[] input;
  printf("\n");
  return isgood;
}

bool bench(size_t N) {
  printf("random UTF-8\n");
  printf("string size = %zu \n", N);
  char *input = new char[N];
  populate_utf8(input, N);
  volatile bool isgood{true};

  {
    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    uint64_t threshold = 500000000;
    for (; finish - start < threshold;) {
      count++;
      isgood &= basic_validate_utf8(input, N);
      finish = nano();
    }
    double t = (N * count) / double(finish - start);

    printf("basic_validate_utf8   %f GB/s\n", t);
  }

  {
    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    uint64_t threshold = 500000000;
    for (; finish - start < threshold;) {
      count++;
      isgood &= simdutf::validate_utf8(input, N);
      finish = nano();
    }
    double t = (N * count) / double(finish - start);

    printf("simdutf               %f GB/s\n", t);
  }

  {
    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    uint64_t threshold = 500000000;
    for (; finish - start < threshold;) {
      count++;
      isgood &= is_utf8(input, N);
      finish = nano();
    }
    double t = (N * count) / double(finish - start);

    printf("is_utf8               %f GB/s\n", t);
  }
  delete[] input;
  printf("\n");
  return isgood;
}

int main() {
  return (bench(40096) & bench(100000) & bench(50000))
  & (zerobuffer_bench(40096) & zerobuffer_bench(100000) & zerobuffer_bench(50000))
  ? EXIT_SUCCESS : EXIT_FAILURE;
}