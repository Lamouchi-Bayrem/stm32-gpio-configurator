# STM32 GPIO Configurator — Software Requirements Specification

**Project:** STM32 GPIO Configurator  
**Repository:** `Lamouchi-Bayrem/stm32-gpio-configurator`  
**Document version:** 1.0  
**Status:** Initial baseline  
**Target application:** Qt 6 desktop application for Linux  

---

## 1. Purpose

STM32 GPIO Configurator is a desktop engineering tool that allows a user to configure STM32 GPIO pins through a graphical interface, preview the resulting register values, and export HAL-free, MISRA-oriented embedded C source files.

The application is intended for education, prototyping, portfolio demonstration, and embedded-software tooling practice. It is not an STMicroelectronics product and is not a replacement for STM32CubeMX.

## 2. Project objectives

The application shall:

1. Provide a clear graphical interface for configuring STM32 GPIO pins.
2. Support multiple manually curated STM32 board profiles.
3. Calculate GPIO register fields from the user configuration.
4. Generate readable, deterministic, register-level embedded C code.
5. Export reusable driver files and project-specific configuration files.
6. Generate code that is suitable for strict compiler and static-analysis workflows.
7. Include host-side unit tests for driver logic.
8. Provide quality-assurance and MISRA-compliance planning resources.
9. Warn users that all generated values require verification against the exact MCU and board documentation.

## 3. Scope

### 3.1 Included in version 1

- Qt 6 Widgets desktop GUI
- STM32F4 GPIO register model
- Multiple GPIO-pin selection
- GPIO input, output, alternate-function, and analog modes
- Push-pull and open-drain selection
- GPIO speed selection
- Pull-up, pull-down, and no-pull selection
- Initial output-state selection
- Register-value preview
- Bare-metal C driver generation
- MISRA-oriented generated API
- Unit-test generation
- Strict compiler-analysis script
- Cppcheck integration through the generated script when available
- Compliance-plan template

### 3.2 Outside the current scope

- Formal MISRA certification
- Automatic parsing of every ST document
- Complete STM32 family coverage
- Startup assembly generation
- Vector-table generation
- Linker-script generation
- System-clock tree generation
- Firmware flashing
- Debugger control
- Full alternate-function database
- Peripheral configuration other than GPIO
- Electrical-rule validation

## 4. Stakeholders

- Embedded-systems students
- Firmware developers
- STM32 learners
- Embedded software tool developers
- Code reviewers
- Static-analysis engineers
- Technical recruiters evaluating the portfolio project

## 5. Supported environment

### 5.1 Development environment

- Operating system: Ubuntu Linux or compatible Linux distribution
- C++ standard: C++17
- GUI framework: Qt 6 Widgets
- Build system: CMake 3.16 or newer
- Build backend: Make or Ninja
- Host compiler: GCC or Clang with C++17 support

### 5.2 Generated-code environment

- Language: ISO C11
- Target style: freestanding embedded C
- Architecture focus: STM32F4 Arm Cortex-M devices
- Driver style: direct register access, without STM32 HAL
- Host-test compiler: GCC or Clang

## 6. Build dependencies

Required Ubuntu packages:

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

Recommended quality tools:

```bash
sudo apt install -y \
    cppcheck \
    clang \
    clang-tidy \
    clang-format
```

The project does not require a Python `requirements.txt` because the application is implemented in C++ and Qt, not Python.

## 7. Functional requirements

### FR-001 — Application startup

The application shall start as a desktop window and display all GPIO configuration controls without requiring a project file.

### FR-002 — Board selection

The application shall provide a board/MCU selection control.

The initial release shall include:

- NUCLEO-F401RE
- NUCLEO-F411RE
- NUCLEO-F446RE
- STM32F407G-DISC1

### FR-003 — Board defaults

When a board is selected, the application shall:

- Load the board's MCU identifier.
- Load the applicable documentation profile.
- Load the GPIO and RCC address profile.
- Select a known demonstration LED port and pin when available.
- display a documentation-verification warning.

### FR-004 — GPIO-port selection

The application shall allow selection of GPIO ports from GPIOA through GPIOI.

The application shall not claim that every displayed port exists on every selected MCU unless device-specific port availability has been validated.

### FR-005 — Multiple-pin selection

The user shall be able to select one or more pins from 0 through 15.

The application shall not export code if no pin is selected.

### FR-006 — Mode selection

The application shall provide the following GPIO modes:

- Input
- Output
- Alternate function
- Analog

### FR-007 — Output-type selection

The application shall provide:

- Push-pull
- Open-drain

### FR-008 — Speed selection

The application shall provide:

- Low
- Medium
- Fast
- High

### FR-009 — Pull selection

The application shall provide:

- No pull
- Pull-up
- Pull-down

### FR-010 — Initial output level

The application shall provide:

- Low
- High

The generated driver shall preload the requested output state before changing the pin to output mode, to reduce output glitches.

### FR-011 — Register preview

The application shall calculate and display selected-field values for:

- `MODER`
- `OTYPER`
- `OSPEEDR`
- `PUPDR`
- `ODR`

The preview shall use a fixed-width hexadecimal representation.

### FR-012 — Deterministic generation

Selected pins shall be emitted in ascending numerical order so that repeated exports of the same configuration produce stable output.

### FR-013 — Driver export

The application shall export:

```text
Core/Inc/gpio.h
Core/Src/gpio.c
```

The driver shall provide operations to:

- Initialize a GPIO pin
- Write a GPIO output
- Toggle a GPIO output
- Read a GPIO input

### FR-014 — Configuration export

The application shall export:

```text
Core/Inc/gpio_config.h
Core/Src/gpio_config.c
```

The configuration source shall:

- Enable the selected GPIO peripheral clock.
- Perform a read-back after clock activation.
- Define one configuration object for each selected pin.
- Initialize pins through the reusable GPIO driver API.
- Return an explicit status code.

### FR-015 — Readable generated identifiers

Generated code shall use descriptive identifiers such as:

```c
GPIO_MODE_OUTPUT
GPIO_OUTPUT_PUSH_PULL
GPIO_SPEED_HIGH
GPIO_PULL_NONE
GPIO_LEVEL_LOW
```

Generated code shall avoid unclear identifiers such as `GPIO_MODE_1` when a descriptive name is available.

### FR-016 — Generated tests

The application shall export:

```text
tests/test_gpio.c
```

The test shall use a simulated GPIO register structure in host memory and shall not dereference physical STM32 peripheral addresses.

### FR-017 — Generated analysis script

The application shall export:

```text
analysis/run-analysis.sh
```

The script shall:

- Compile generated C with strict warning flags.
- Treat compiler warnings as errors.
- Build and execute host unit tests.
- Run Cppcheck when it is installed.
- Stop when a mandatory command fails.

### FR-018 — Generated compliance resource

The application shall export:

```text
compliance/MISRA_COMPLIANCE_PLAN.md
```

The file shall explain that analyzer-friendly output is not automatically a formal MISRA-compliance claim.

### FR-019 — Export errors

The application shall display an actionable error message when:

- The export directory cannot be created.
- A template cannot be found.
- A generated file cannot be written.
- No pin is selected.
- A board profile is invalid.

### FR-020 — Export completion

After successful export, the application shall display the selected output directory and the generated project contents.

## 8. Generated driver requirements

### GDR-001 — Fixed-width types

The generated driver shall use fixed-width integer types from `<stdint.h>`.

### GDR-002 — Volatile registers

Memory-mapped GPIO registers shall be declared with `volatile` qualification.

### GDR-003 — Parameter validation

Public driver functions shall validate:

- Null pointers
- Pin range
- Mode range
- Output-type range
- Speed range
- Pull range
- Output-level range

### GDR-004 — Status codes

Public driver functions shall return an explicit status code.

Expected statuses include:

- Success
- Null pointer
- Invalid pin
- Invalid configuration

### GDR-005 — Atomic output update

GPIO output set/reset operations shall use `BSRR` rather than a non-atomic read-modify-write operation on `ODR`.

### GDR-006 — Register preservation

Initialization shall modify only the selected pin's fields and preserve unrelated register fields.

### GDR-007 — No dynamic allocation

Generated embedded code shall not use dynamic memory allocation.

### GDR-008 — No recursion

Generated embedded code shall not use recursion.

### GDR-009 — No standard I/O dependency

Generated embedded code shall not depend on hosted standard input/output functions.

### GDR-010 — Hardware isolation

Target-specific peripheral addresses and clock masks shall be isolated in generated configuration code rather than spread throughout the reusable driver.

## 9. Quality and MISRA-oriented requirements

### QR-001 — Language baseline

Generated code shall compile as ISO C11 with extensions disabled where supported.

### QR-002 — Strict warnings

Generated code shall compile cleanly with:

```text
-Wall
-Wextra
-Wpedantic
-Wconversion
-Wsign-conversion
-Wshadow
-Wundef
-Werror
```

### QR-003 — Explicit conversions

Conversions between enumeration, integer, signed, and unsigned types shall be explicit where ambiguity may occur.

### QR-004 — Constant qualification

Read-only configuration objects and pointer parameters shall use `const` where applicable.

### QR-005 — Controlled hardware deviations

The compliance documentation shall identify memory-mapped address conversion as a target-specific item requiring review and, depending on the selected analyzer and project policy, a controlled deviation.

### QR-006 — No false compliance claim

The application and generated documentation shall use terms such as:

- MISRA-oriented
- Analyzer-friendly
- Prepared for static analysis

They shall not claim certified or formal MISRA compliance solely because automated checks pass.

### QR-007 — Traceability

Each generated project shall identify:

- Selected board
- Selected MCU
- Documentation profile
- Selected port
- Selected pins
- Selected GPIO configuration

### QR-008 — Reproducibility

A fixed application version and identical user configuration shall produce functionally equivalent generated files.

## 10. Non-functional requirements

### NFR-001 — Usability

A user familiar with basic STM32 GPIO concepts shall be able to create and export a configuration without editing application source code.

### NFR-002 — Maintainability

GUI code, board profiles, generation logic, reusable templates, tests, and compliance documentation shall be organized into separate files or modules.

### NFR-003 — Portability

The desktop application shall avoid Linux-specific behavior except where invoking optional analysis scripts. The generated C driver shall avoid hosted operating-system dependencies.

### NFR-004 — Performance

Register preview shall update without perceptible delay for up to 16 selected pins.

### NFR-005 — Reliability

The application shall not silently ignore file-system failures, invalid selections, or missing templates.

### NFR-006 — Security

The application shall not execute source files from the selected export directory. It shall only generate files and launch explicitly configured analysis commands.

### NFR-007 — Documentation

The repository shall contain clear build, run, export, testing, and limitation instructions.

### NFR-008 — Source control

Generated build directories, local IDE files, and test executables shall be excluded through `.gitignore`.

## 11. Verification and acceptance criteria

The first release is acceptable when all of the following pass.

### AC-001 — GUI build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
```

The build shall complete without compiler errors.

### AC-002 — GUI startup

```bash
./build/STM32GPIOConfigurator
```

The application shall open and display all configuration controls.

### AC-003 — PA5 output preview

For NUCLEO-F401RE, GPIOA pin 5, output mode, push-pull, low speed, no pull, initial low, the selected-field `MODER` preview shall contain:

```text
0x00000400UL
```

### AC-004 — Multi-pin preview

For GPIOD pins 12, 13, 14, and 15 in output mode, the selected-field `MODER` preview shall contain:

```text
0x55000000UL
```

### AC-005 — Export structure

An export shall contain the driver, configuration, tests, analysis script, compliance plan, and README in the documented directory structure.

### AC-006 — Strict generated-code compilation

The exported project shall pass:

```bash
chmod +x analysis/run-analysis.sh
./analysis/run-analysis.sh
```

### AC-007 — Unit tests

The generated host unit-test executable shall exit with status `0`.

### AC-008 — Empty selection

Attempting to export with no selected pins shall be rejected with a clear message.

### AC-009 — Missing template

If the template directory is unavailable, export shall fail safely and identify the missing path.

### AC-010 — Static analysis

When Cppcheck is installed, the generated script shall execute it and present any findings without suppressing them silently.

## 12. Planned future requirements

- Complete device-specific GPIO-port validation
- STM32F0/F1/F3/F7/G0/G4/H7/L0/L4 profiles
- Alternate-function configuration and AFR register generation
- Pin-conflict detection
- Board-pinout visualization
- EXTI configuration
- Project save/load format
- CMSIS-compatible generated output
- ARM GCC startup and linker templates
- Continuous-integration workflow
- Integration with Embedded Code Quality Assistant
- Optional adapters for PC-lint Plus, Helix QAC, Coverity, and Polyspace

## 13. Constraints and disclaimers

1. Board profiles are manually curated and may contain errors.
2. The exact STM32 part number, package, board revision, reference manual, datasheet, board schematic, and errata remain the authoritative engineering sources.
3. Generated host tests verify driver logic; they do not validate electrical behavior or physical hardware.
4. Passing strict compiler checks and static analysis does not establish functional safety certification.
5. Generated code must be integrated into a complete embedded project containing startup code, a vector table, linker script, system initialization, and a suitable flashing/debugging configuration.

## 14. Definition of done

A feature is complete when:

- Its requirement is implemented.
- The GUI builds successfully.
- Relevant generated output compiles with strict warnings.
- Host unit tests pass.
- Error paths have been tested.
- Documentation has been updated.
- No known analyzer finding is hidden without review or justification.
- The feature has been committed with a descriptive Git message.
