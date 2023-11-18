#ifndef _UI_H
#define _UI_H

#include "oled.h"

#define UI_MAX_CURSOR 9 // [0, UI_MAX_CURSOR]
enum UI_Events
{
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_PRESS,
    BUTTON_LONG_PRESS,
};

#define UI_STATE_MAX_INDEX 1
enum UI_States
{
    UI_STATE_STATUS = 0,
    UI_STATE_SETTINGS = 1,
};

void UI_Show(void);
void UI_ButtonEvents(enum UI_Events event);

#endif