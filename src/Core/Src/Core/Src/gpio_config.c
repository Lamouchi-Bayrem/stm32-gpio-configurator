#include "gpio_config.h"
#include <stdint.h>

#define GPIO_BASE_ADDRESS (0X40020000UL)
#define RCC_BASE_ADDRESS (0X40023800UL)
#define RCC_AHB1ENR_OFFSET (0x30UL)
#define RCC_GPIO_ENABLE_MASK (0X00000008UL)
#define GPIO_SELECTED ((GpioRegisters_t *)(GPIO_BASE_ADDRESS + (3UL * 0x400UL)))
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE_ADDRESS + RCC_AHB1ENR_OFFSET))

static const GpioPinConfig_t gpioConfig0 =
{
    4U,
    GPIO_MODE_OUTPUT,
    GPIO_OUTPUT_PUSH_PULL,
    GPIO_SPEED_LOW,
    GPIO_PULL_NONE,
    GPIO_LEVEL_LOW
};

GpioStatus_t Gpio_ConfigGenerated(void)
{
    GpioStatus_t status = GPIO_STATUS_OK;
    RCC_AHB1ENR |= RCC_GPIO_ENABLE_MASK;
    (void)RCC_AHB1ENR;
    if (status == GPIO_STATUS_OK)
    {
        status = Gpio_InitPin(GPIO_SELECTED, &gpioConfig0);
    }
    return status;
}
