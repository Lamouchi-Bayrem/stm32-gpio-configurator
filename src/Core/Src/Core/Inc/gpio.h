#ifndef GPIO_H
#define GPIO_H
#include <stdint.h>
typedef struct { volatile uint32_t MODER; volatile uint32_t OTYPER; volatile uint32_t OSPEEDR; volatile uint32_t PUPDR; volatile uint32_t IDR; volatile uint32_t ODR; volatile uint32_t BSRR; volatile uint32_t LCKR; volatile uint32_t AFRL; volatile uint32_t AFRH; } GpioRegisters_t;
typedef enum { GPIO_STATUS_OK=0, GPIO_STATUS_NULL_POINTER, GPIO_STATUS_INVALID_PIN, GPIO_STATUS_INVALID_CONFIGURATION } GpioStatus_t;
typedef enum { GPIO_MODE_INPUT=0, GPIO_MODE_OUTPUT=1, GPIO_MODE_ALTERNATE=2, GPIO_MODE_ANALOG=3 } GpioMode_t;
typedef enum { GPIO_OUTPUT_PUSH_PULL=0, GPIO_OUTPUT_OPEN_DRAIN=1 } GpioOutputType_t;
typedef enum { GPIO_SPEED_LOW=0, GPIO_SPEED_MEDIUM=1, GPIO_SPEED_FAST=2, GPIO_SPEED_HIGH=3 } GpioSpeed_t;
typedef enum { GPIO_PULL_NONE=0, GPIO_PULL_UP=1, GPIO_PULL_DOWN=2 } GpioPull_t;
typedef enum { GPIO_LEVEL_LOW=0, GPIO_LEVEL_HIGH=1 } GpioLevel_t;
typedef struct { uint8_t pin; GpioMode_t mode; GpioOutputType_t outputType; GpioSpeed_t speed; GpioPull_t pull; GpioLevel_t initialLevel; } GpioPinConfig_t;
GpioStatus_t Gpio_InitPin(GpioRegisters_t *gpio,const GpioPinConfig_t *config);
GpioStatus_t Gpio_WritePin(GpioRegisters_t *gpio,uint8_t pin,GpioLevel_t level);
GpioStatus_t Gpio_TogglePin(GpioRegisters_t *gpio,uint8_t pin);
GpioStatus_t Gpio_ReadPin(const GpioRegisters_t *gpio,uint8_t pin,GpioLevel_t *level);
#endif
