#ifndef DELAY_DELAY_H_
#define DELAY_DELAY_H_

#include "main.h"

#define USE_DWT_COUNTER // For Real-time OS

#ifdef USE_DWT_COUNTER
uint8_t DWT_Delay_Init(void);
// In STM32F103, a clock cycle is 1/72MHz = 13.88ns
// so delay one nanosecond is only available in mcu with over 1000MHz(1GHz) clock speed
// otherwise, the minimum delay time is a clock cycle = 13.88ns
// STM32G4(170MHz) a clock cycle is 1/170MHz = 5.88ns
__STATIC_INLINE void delay_cycle(volatile uint32_t cycle)
{
    uint32_t start_cycle = DWT->CYCCNT;
    while((DWT->CYCCNT - start_cycle) < cycle);
}
__STATIC_INLINE void delay_us(volatile uint32_t us)
{
    uint32_t start_cycle = DWT->CYCCNT;
    us *= (SystemCoreClock / 1000000);
    while((DWT->CYCCNT - start_cycle) < us); // DWT->CYCCNT is a 32-bit register, and it takes account of overflow
}
__STATIC_INLINE uint32_t get_micros(void)
{
    return DWT->CYCCNT / (SystemCoreClock / 1000000);
}
#else
void delay_us(uint32_t us);
#endif
void delay_ms(uint32_t ms);

#endif /* DELAY_DELAY_H_ */
