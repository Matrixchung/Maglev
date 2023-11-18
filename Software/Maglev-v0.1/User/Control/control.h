#ifndef _CONTROL_H
#define _CONTROL_H

#include "main.h"
#include "coil.h"
// #include "fuzzy_pid.h"
#include "sliding_filter.h"

extern uint8_t control_state, floater_state, floater_steady, floater_fail_safe, current_fuse, low_power_mode;
extern uint8_t revolution_control, height_control, position_control;
extern float revolution_speed, revolution_r;
extern float floater_height;
extern float z_target;
extern PID_TypeDef X_Axis_PID, Y_Axis_PID, Z_Axis_PID;
extern PID_TypeDef X_Axis_Outer_PID, Y_Axis_Outer_PID, Z_Axis_Outer_PID;
extern PID_TypeDef X_Modifier_PID, Y_Modifier_PID;
// extern FuzzyPID_TypeDef X_Axis_PID, Y_Axis_PID, Z_Axis_PID;
extern float Hall_X_Offset, Hall_Y_Offset, X_Modifier, Y_Modifier;
extern float outer_ring_set_current;
extern float negative_gravity;
extern float inner_total_current, outer_total_current, total_power;

void control_init(void);
void control_calculate_power(void);
void control_loop(void);

#endif