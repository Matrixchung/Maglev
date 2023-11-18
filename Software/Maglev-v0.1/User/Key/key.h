#ifndef _KEY_H
#define _KEY_H

#include "main.h"
#define KEY_SCAN_INTERVAL_MS 10
#define KEY_DEBOUNCE_MS      100
#define KEY_LONG_PRESS_MS    650
enum KEY_STATE
{
    NOT_PRESSED = 0,
    DEBOUNCED = 1,
    PRESSED = 2,
    LONG_PRESSED = 3,
    LONG_PRESS_RELEASED = 4
};
typedef void (*Key_Callback)();
typedef struct Key_TypeDef
{
    GPIO_TypeDef *GPIO_Port;
    uint32_t GPIO_Pin;
    uint8_t active_state;
    enum KEY_STATE state;
    uint8_t debounce_cnt;
    uint16_t long_press_cnt;
    Key_Callback callback;
}Key_TypeDef;

__STATIC_INLINE void Key_RegisterCallback(Key_TypeDef *key, Key_Callback cb)
{
    key->callback = cb;
}

extern Key_TypeDef Key_Onboard, Key_EC11;

void Key_InstanceInit(Key_TypeDef *key, GPIO_TypeDef *port, uint32_t pin, uint8_t active);
void Key_InstanceUpdate(Key_TypeDef *key);

#endif