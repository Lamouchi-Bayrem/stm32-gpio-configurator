#include "gpio.h"
#include <assert.h>
#include <string.h>
int main(void){GpioRegisters_t gpio;const GpioPinConfig_t config={5U,GPIO_MODE_OUTPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_SPEED_HIGH,GPIO_PULL_UP,GPIO_LEVEL_HIGH};GpioLevel_t level=GPIO_LEVEL_LOW;(void)memset(&gpio,0,sizeof(gpio));assert(Gpio_InitPin(&gpio,&config)==GPIO_STATUS_OK);assert((gpio.MODER&(3UL<<10U))==(1UL<<10U));assert(gpio.BSRR==(1UL<<5U));gpio.IDR=(1UL<<5U);assert(Gpio_ReadPin(&gpio,5U,&level)==GPIO_STATUS_OK);assert(level==GPIO_LEVEL_HIGH);assert(Gpio_InitPin(NULL,&config)==GPIO_STATUS_NULL_POINTER);return 0;}
