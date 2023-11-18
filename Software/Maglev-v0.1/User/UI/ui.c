#include "ui.h"
enum UI_States state = UI_STATE_STATUS;
uint8_t cursor = 0;
int8_t cursor_selected = 0;
// 1206

void UI_Show(void)
{
    OLED_Clear(0);
    switch(state)
    {
        case UI_STATE_STATUS:
            OLED_DrawString(26, 0, SMALL_FONT, 1, ">> Status <<");
            if(ble_state) OLED_DrawIcon(114, 0, ICON_BLUETOOTH, MODE_NORMAL);
            OLED_printf(0, 13, SMALL_FONT, MODE_NORMAL, "H_X: %d, Y: %d", (uint16_t)ADC_GetRaw(CHANNEL_HALL_X), (uint16_t)ADC_GetRaw(CHANNEL_HALL_Y));
            OLED_printf(0, 26, SMALL_FONT, MODE_NORMAL, "TOF:%d,tgt:%d,zctrl:%d",(uint8_t)floater_height, (uint8_t)z_target, height_control);
            OLED_printf(0, 39, SMALL_FONT, MODE_NORMAL, "FLT st:%d,stdy:%d,f_s:%d", floater_state, floater_steady, floater_fail_safe);
            OLED_printf(0, 52, SMALL_FONT, MODE_NORMAL, "lp:%d,p:%.1f,r:%.1f", low_power_mode, imu_pitch, imu_roll);
            break;
        case UI_STATE_SETTINGS:
            // start = 13
            OLED_DrawString(21, 0, SMALL_FONT, 1, ">> Settings <<");
            if(ble_state) OLED_DrawIcon(114, 0, ICON_BLUETOOTH, MODE_NORMAL);
            if(cursor < 4) // page one
            {
                OLED_printf(0, 13, SMALL_FONT, cursor == 0 ? MODE_REVERSE : MODE_NORMAL, "Low-power mode: %d", low_power_mode);
                OLED_printf(0, 26, SMALL_FONT, cursor == 1 ? MODE_REVERSE : MODE_NORMAL, "Z-Axis control: %d", height_control);
                OLED_printf(0, 39, SMALL_FONT, cursor == 2 ? MODE_REVERSE : MODE_NORMAL, "Z-target: %d", (uint8_t)z_target);
                OLED_printf(0, 52, SMALL_FONT, cursor == 3 ? MODE_REVERSE : MODE_NORMAL, "outer_curr: %d", (int16_t)outer_ring_set_current);
            }
            else if(cursor < 8) // page two
            {
                OLED_printf(0, 13, SMALL_FONT, cursor == 4 ? MODE_REVERSE : MODE_NORMAL, "Revolution control: %d", revolution_control);
                OLED_printf(0, 26, SMALL_FONT, cursor == 5 ? MODE_REVERSE : MODE_NORMAL, "rev_radius: %d", (int16_t)revolution_r);
                OLED_printf(0, 39, SMALL_FONT, cursor == 6 ? MODE_REVERSE : MODE_NORMAL, "rev_W: %d", (int16_t)revolution_speed);
                OLED_printf(0, 52, SMALL_FONT, cursor == 7 ? MODE_REVERSE : MODE_NORMAL, "Position control: %d", position_control);
            }
            else if(cursor < 12) // page three
            {
                OLED_printf(0, 13, SMALL_FONT, cursor == 8 ? MODE_REVERSE : MODE_NORMAL, "X_Modifier: %d", (int16_t)X_Modifier);
                OLED_printf(0, 26, SMALL_FONT, cursor == 9 ? MODE_REVERSE : MODE_NORMAL, "Y_Modifier: %d", (int16_t)Y_Modifier);
            }
            break;
    }
    OLED_RefreshGram();
}

void UI_ButtonEvents(enum UI_Events event)
{
    switch(event)
    {
        case BUTTON_LEFT:
            if(state == UI_STATE_STATUS) state = UI_STATE_SETTINGS;
            else if(state == UI_STATE_SETTINGS)
            {
                if(cursor_selected > 0)
                {
                    switch(cursor_selected)
                    {
                        case 2: // z-target
                            z_target -= 1.0f;
                            break;
                        case 3:
                            outer_ring_set_current -= 100.0f;
                            break;
                        case 5:
                            revolution_r -= 50.0f;
                            break;
                        case 6:
                            revolution_speed -= 1000;
                            break;
                        case 8:
                            X_Modifier -= 100;
                            break;
                        case 9:
                            Y_Modifier -= 100;
                            break;
                        default: break;
                    }
                }
                else
                {
                    if(cursor > 0) cursor--;
                    // else cursor = UI_MAX_CURSOR;
                }
            }
            // if(state > 0) state--;
            // else state = UI_STATE_MAX_INDEX;
            break;
        case BUTTON_RIGHT:
            if(state == UI_STATE_STATUS) state = UI_STATE_SETTINGS;
            else
            {
                if(state == UI_STATE_SETTINGS)
                {
                    if(cursor_selected > 0)
                    {
                        switch(cursor_selected)
                        {
                            case 2: // z-target
                                z_target += 1.0f;
                                break;
                            case 3:
                                outer_ring_set_current += 100.0f;
                                break;
                            case 5:
                                revolution_r += 50.0f;
                                break;
                            case 6:
                                revolution_speed += 1000;
                                break;
                            case 8:
                                X_Modifier += 100;
                                break;
                            case 9:
                                Y_Modifier += 100;
                                break;
                            default: break;
                        }
                    }
                    else
                    {
                        if(cursor < UI_MAX_CURSOR) cursor++;
                        // else cursor = 0;
                    }
                }
            }
            // if(state < UI_STATE_MAX_INDEX) state++;
            // else state = 0;
            break;
        case BUTTON_PRESS:
            if(state == UI_STATE_STATUS) state = UI_STATE_SETTINGS;
            else if(state == UI_STATE_SETTINGS)
            {
                switch(cursor)
                {
                    case 0: // low power mode
                        low_power_mode = !low_power_mode;
                        if(low_power_mode)
                        {
                            Coil_LT.set_current = 0;
                            Coil_LB.set_current = 0;
                            Coil_RT.set_current = 0;
                            Coil_RB.set_current = 0;
                            Coil_SetOutput(&Coil_LT, 0);
                            Coil_SetOutput(&Coil_LB, 0);
                            Coil_SetOutput(&Coil_RT, 0);
                            Coil_SetOutput(&Coil_RB, 0);
                            ICM20602_SetSleep(1);
                        }
                        else
                        {
                            ICM20602_SetSleep(0);
                        }
                        cursor_selected = -1;
                        break;
                    case 1: // z-axis control
                        height_control = !height_control;
                        if(!height_control) outer_ring_set_current = 0;
                        cursor_selected = -1;
                        break;
                    case 2: // z-target
                        if(cursor_selected != 2) cursor_selected = 2;
                        else cursor_selected = -1; // exit editing mode
                        break;
                    case 3: // outer_ring_set_current
                        if(cursor_selected != 3) cursor_selected = 3;
                        else cursor_selected = -1;
                        break;
                    case 4: // revolution_control
                        revolution_control = !revolution_control;
                        if(position_control && revolution_control) position_control = 0;
                        break;
                    case 5: // revolution radius
                        if(cursor_selected != 5) cursor_selected = 5;
                        else cursor_selected = -1;
                        break;
                    case 6: // revolution speed
                        if(cursor_selected != 6) cursor_selected = 6;
                        else cursor_selected = -1;
                        break;
                    case 7: // position control
                        position_control = !position_control;
                        if(position_control) X_Modifier = Y_Modifier = 0;
                        if(position_control && revolution_control) revolution_control = 0;
                        break;
                    case 8: // X_Modifier
                        if(cursor_selected != 8) cursor_selected = 8;
                        else cursor_selected = -1;
                        break;
                    case 9: // Y_Modifier
                        if(cursor_selected != 9) cursor_selected = 9;
                        else cursor_selected = -1;
                        break;    
                    default: break;
                }
            }
            break;
        case BUTTON_LONG_PRESS:
            if(state < UI_STATE_MAX_INDEX) state++;
            else state = 0;
            cursor_selected = -1;
            break;
        default: break;
    }
}