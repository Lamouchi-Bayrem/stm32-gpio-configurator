#!/usr/bin/env bash
set -eu
CC=${CC:-gcc}
FLAGS="-std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wundef -Werror"
$CC $FLAGS -ICore/Inc -fsyntax-only Core/Src/gpio.c Core/Src/gpio_config.c
$CC $FLAGS -ICore/Inc Core/Src/gpio.c tests/test_gpio.c -o tests/test_gpio
./tests/test_gpio
if command -v cppcheck >/dev/null 2>&1; then cppcheck --std=c11 --enable=warning,style,performance,portability --suppress=missingIncludeSystem -ICore/Inc Core/Src; fi
echo "All available checks passed."
