<div align="center">

# STM32 GPIO Configurator

### A Qt-based desktop tool that generates clean, HAL-free, MISRA-oriented STM32F4 GPIO code

Configure GPIO pins visually, inspect register values, and export analyzer-friendly embedded C with unit tests and quality-assurance resources.

[![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus)](https://isocpp.org/)
[![Qt](https://img.shields.io/badge/Qt-6-41CD52?logo=qt&logoColor=white)](https://www.qt.io/)
[![C](https://img.shields.io/badge/Embedded_C-C11-A8B9CC?logo=c)](https://www.iso.org/standard/74528.html)
[![STM32](https://img.shields.io/badge/STM32-Bare--Metal-03234B?logo=stmicroelectronics)](https://www.st.com/stm32)
[![CMake](https://img.shields.io/badge/CMake-Build-064F8C?logo=cmake)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

</div>

---

## Why this project matters

Embedded developers often move between MCU documentation, register maps, board schematics, configuration tools, and static analyzers. **STM32 GPIO Configurator** brings the first stages of that workflow into one desktop application.

The project demonstrates more than GUI development:

- Register-level STM32 programming
- Memory-mapped peripheral access
- GPIO clock and register configuration
- Embedded C code generation
- Qt and modern C++ desktop development
- CMake-based project organization
- MISRA-oriented coding practices
- Static-analysis and unit-test integration
- Engineering-tool development similar to internal tools used in embedded companies

> This project is educational and independent. It is not affiliated with, endorsed by, or a replacement for STM32CubeMX or STMicroelectronics tools.

## Core features

- Visual STM32 GPIO configuration
- Multiple-pin selection
- Input, output, alternate-function, and analog modes
- Push-pull and open-drain output types
- Low, medium, fast, and high-speed options
- No-pull, pull-up, and pull-down configuration
- Initial output-level selection
- Live register preview for:
  - `MODER`
  - `OTYPER`
  - `OSPEEDR`
  - `PUPDR`
  - `ODR`
- HAL-free embedded C generation
- MISRA-oriented API and generated configuration
- Parameter validation and explicit status codes
- Atomic GPIO set/reset through `BSRR`
- Exported host unit tests
- Strict compiler-analysis script
- Cppcheck support when installed
- Compliance-plan template for engineering review

## Supported board profiles

- NUCLEO-F401RE
- NUCLEO-F411RE
- NUCLEO-F446RE
- STM32F407G-DISC1

The profiles are manually curated from the corresponding STM32 reference manuals and board documentation. Always verify the exact MCU part number, package, board revision, alternate-function table, electrical limits, and silicon errata before using generated code on hardware.

## Architecture

```text
User configuration
       |
       v
Qt 6 Widgets GUI
       |
       v
Validation and register calculation
       |
       v
MISRA-oriented code generator
       |
       +----> Register preview
       |
       +----> gpio.h / gpio.c
       +----> gpio_config.h / gpio_config.c
       +----> Host unit tests
       +----> Analysis script
       +----> Compliance documentation
```

## Generated project structure

```text
generated-project/
├── Core/
│   ├── Inc/
│   │   ├── gpio.h
│   │   └── gpio_config.h
│   └── Src/
│       ├── gpio.c
│       └── gpio_config.c
├── tests/
│   └── test_gpio.c
├── analysis/
│   └── run-analysis.sh
├── compliance/
│   └── MISRA_COMPLIANCE_PLAN.md
└── README.md
```

## Example generated configuration

```c
static const GpioPinConfig_t ledConfig =
{
    5U,
    GPIO_MODE_OUTPUT,
    GPIO_OUTPUT_PUSH_PULL,
    GPIO_SPEED_LOW,
    GPIO_PULL_NONE,
    GPIO_LEVEL_LOW
};
```

The generated driver provides operations such as:

```c
GpioStatus_t Gpio_InitPin(
    GpioRegisters_t * gpio,
    const GpioPinConfig_t * config
);

GpioStatus_t Gpio_WritePin(
    GpioRegisters_t * gpio,
    uint8_t pin,
    GpioLevel_t level
);

GpioStatus_t Gpio_ReadPin(
    const GpioRegisters_t * gpio,
    uint8_t pin,
    GpioLevel_t * level
);
```

## Build and run

### Ubuntu dependencies

```bash
sudo apt update
sudo apt install -y \
    build-essential \
    cmake \
    ninja-build \
    qt6-base-dev \
    qt6-base-dev-tools \
    gcc \
    cppcheck \
    clang-tidy \
    clang-format
```

### Clone

```bash
git clone https://github.com/Lamouchi-Bayrem/stm32-gpio-configurator.git
cd stm32-gpio-configurator
```

### Configure and compile

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel "$(nproc)"
```

### Run

```bash
./build/STM32GPIOConfigurator
```

## Test exported code

After exporting a GPIO project, enter the generated directory and run:

```bash
chmod +x analysis/run-analysis.sh
./analysis/run-analysis.sh
```

The script performs strict C11 compilation, builds and executes the host GPIO unit tests, and runs Cppcheck when available.

A manual strict compilation can also be performed with:

```bash
gcc \
    -std=c11 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -Wconversion \
    -Wsign-conversion \
    -Wshadow \
    -Wundef \
    -Werror \
    -ICore/Inc \
    -fsyntax-only \
    Core/Src/gpio.c \
    Core/Src/gpio_config.c
```

## MISRA and safety note

The tool generates **MISRA-oriented** and analyzer-friendly code. This does not automatically establish formal MISRA compliance.

A formal compliance claim requires, at minimum:

- An agreed MISRA baseline
- A guideline-enforcement plan
- Analyzer and compiler configuration records
- Manual review
- Documented and approved deviations
- Implementation-defined behavior records
- Unit and integration test evidence
- Verification on the exact hardware target

Memory-mapped peripheral-address conversions and other hardware-specific constructs may require controlled deviations depending on the selected analyzer and project policy.

## Roadmap

- [ ] STM32F0, F1, F3, F7, G0, G4, H7, L0, and L4 families
- [ ] Alternate-function database and pin-conflict detection
- [ ] MCU package and pinout visualization
- [ ] EXTI and interrupt generation
- [ ] Save/load configuration files
- [ ] Register bitfield visualization
- [ ] CMSIS-compatible output option
- [ ] GCC ARM project generation
- [ ] Linker script and startup-template integration
- [ ] PC-lint Plus, Helix QAC, and Polyspace adapters
- [ ] Continuous integration with automated host tests
- [ ] Companion Embedded Code Quality Assistant integration

## Portfolio value

This project highlights practical experience in:

```text
Embedded C          Qt / C++          STM32 bare-metal
Register programming Code generation   Static analysis
Unit testing         CMake              Software quality
```

It is designed as a portfolio project for embedded-software, firmware, tooling, automotive, IoT, and embedded-Linux engineering roles.

## Contributing

Contributions, issue reports, board profiles, test cases, and code-quality improvements are welcome.

1. Fork the repository.
2. Create a feature branch.
3. Add or update tests.
4. Run the compiler and analyzer checks.
5. Open a pull request with a clear technical description.

## Author

**Bayrem Lamouchi**

- GitHub: [@Lamouchi-Bayrem](https://github.com/Lamouchi-Bayrem)
- Project: [stm32-gpio-configurator](https://github.com/Lamouchi-Bayrem/stm32-gpio-configurator)

## License

Add an open-source license before distributing the project. The badge currently assumes the MIT License; if you choose another license, update both the `LICENSE` file and badge.

---

<div align="center">

If this project is useful, consider giving it a star and sharing your feedback.

**Built for learning, engineering quality, and professional embedded-software portfolios.**

</div>
