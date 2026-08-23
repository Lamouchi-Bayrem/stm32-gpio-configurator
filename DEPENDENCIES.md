# Build and Tool Dependencies

## Required

- Ubuntu Linux or compatible distribution
- CMake 3.16+
- C++17 compiler
- Qt 6 Widgets development files
- Make or Ninja

Install on Ubuntu:

```bash
sudo apt update
sudo apt install -y \
    build-essential \
    cmake \
    ninja-build \
    qt6-base-dev \
    qt6-base-dev-tools \
    gcc \
    g++
```

## Recommended analysis tools

```bash
sudo apt install -y \
    cppcheck \
    clang \
    clang-tidy \
    clang-format
```

## Build

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel "$(nproc)"
./build/STM32GPIOConfigurator
```

## Note

This is a C++/Qt project, so a Python `requirements.txt` file is not required.
