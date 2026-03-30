# Base64

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![libsodium](https://img.shields.io/badge/libsodium-required-green.svg)](https://doc.libsodium.org/)

A lightweight C++17 wrapper around libsodium for Base64 encoding and decoding.

## Description

This library provides a simple `Base64` class with static methods to encode binary data or text to Base64 and decode Base64 strings back to binary or text. It uses the original Base64 variant from libsodium (no padding issues, standard alphabet).

## Requirements

- C++17 compiler
- [libsodium](https://doc.libsodium.org/) (must be installed and findable by CMake)

## Installation

### Using CMake FetchContent

Add the following to your `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
    base64
    GIT_REPOSITORY https://github.com/ania-7abc/base64.git
    GIT_TAG v1.0.3
)

# Optionally, control whether to build shared libraries (default OFF)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

FetchContent_MakeAvailable(base64)

# Link against the library
target_link_libraries(your_target PRIVATE base64)
```

The library exports the target `base64`. Dependencies on libsodium are handled automatically.

## Usage

Include the header:

```cpp
#include <base64/base64.hpp>
```

### Binary Data

```cpp
std::vector<uint8_t> data = {0x48, 0x65, 0x6c, 0x6c, 0x6f};
std::string encoded = Base64::encode(data);
// encoded = "SGVsbG8="

std::vector<uint8_t> decoded = Base64::decode(encoded);
// decoded == {0x48, 0x65, 0x6c, 0x6c, 0x6f}
```

### Text Strings

```cpp
std::string text = "Hello, World!";
std::string encoded = Base64::encode(text);
// encoded = "SGVsbG8sIFdvcmxkIQ=="

std::string decoded = Base64::decode_to_string(encoded);
// decoded == "Hello, World!"
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
