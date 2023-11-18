#include "control.h"
#define INNER_COIL_CURRENT_FUSE 7000
#define OUTER_COIL_CURRENT_FUSE 8000
#define CURRENT_FUSE_THRESHOLD_TIME (5 * 100) // 5 * 100 * 0.01s = 5s
#define FLOATER_FAIL_SAFE_TIME (0.5 * 100) // 0.5 * 100 * 0.01s = 0.5s
#define X_INSTALL_OFFSET -255 // -255
#define Y_INSTALL_OFFSET -225 // -225
#define TOF_ZERO_OFFSET 10
#define FLOATER_MAX_HEIGHT 70
#define FLOATER_FAIL_SAFE_HEIGHT 14
#define FLOATER_STABLE_CURRENT_THRESHOLD 500
#define FLOATER_STABLE_TIME (1 * 100) // 1 * 100 * 0.1s = 1s
#define X_MODIFIER_RANGE 1200
#define Y_MODIFIER_RANGE 1200
#define OUTER_MODIFIER_CURR_MULTIPLIER 0.67f
#define INNER_MODIFIER_CURR_MULTIPLIER 0.58f
#define MAX_Z_CONTROL_AXIS_MODIFIER 300
#define MAX_UNCONTROL_ANGLE 5
#define HALL_ZERO_SET 2017
#define HALL_ZERO_RANGE 500
float floater_height = 0;
uint8_t control_state = 1;               // 控制状态
uint8_t floater_state = 0;               // 浮子检测 Flag
uint8_t floater_steady = 0;              // 浮子稳定 Flag
static uint16_t floater_steady_tick = 0; // 浮子稳定计时器
uint8_t floater_fail_safe = 0;           // 浮子错误保护 Flag
uint8_t revolution_control = 0;          // 公转控制 Flag
uint8_t height_control = 0;              // 定高 Flag
uint8_t low_power_mode = 0;              // 低功耗模式 Flag
uint8_t position_control = 0;            // 位置控制 Flag
float revolution_r = 0.0f;               // 公转半径
float revolution_speed = 0.0f;           // 公转角速度, 循环周期: 100Hz (0.1s), 单次累加角度: (v * 0.1) / 360, 累加弧度: (v * 0.1) / 360 * DEG_2_RAD
static float revolution_angle_rad = 0.0f;
#define REVOLUTION_SPEED_MULTIPLIER 4.8481368110953599358991410235806e-6f // (DEG_2_RAD * 0.1 / 360)
static uint16_t floater_fail_safe_tick = 0;
uint8_t current_fuse = 0;
static uint16_t current_fuse_tick = 0;
float z_target = 27.0f;
PID_TypeDef X_Axis_PID, Y_Axis_PID, Z_Axis_PID;
PID_TypeDef X_Axis_Outer_PID, Y_Axis_Outer_PID, Z_Axis_Outer_PID;
PID_TypeDef X_Modifier_PID, Y_Modifier_PID;
float negative_gravity = 0.0f;
// FuzzyPID_TypeDef X_Axis_PID, Y_Axis_PID, Z_Axis_PID;
LPFilter_TypeDef Hall_X_Filter, Hall_Y_Filter;
SlidingFilter_TypeDef TOF_Filter;
float Hall_X_Offset = 0;
float Hall_Y_Offset = 0;
float X_Modifier = 0;
float Y_Modifier = 0;
float outer_ring_set_current = 0.0f;
float inner_total_current = 0.0f;
float outer_total_current = 0.0f;
float total_power = 0.0f;
static float hall_x_self_test_value = 0.0f;
static float hall_y_self_test_value = 0.0f;
static float roll_target = 0.0f;
static float pitch_target = 0.0f;
// TIM3, freq = 10 KHz
void control_init(void)
{
    Coil_InstanceInit(&Coil_X_Pos, TIM1, LL_TIM_CHANNEL_CH3, TIM1, LL_TIM_CHANNEL_CH3N, CHANNEL_AMP_X_POS, 1); //
    Coil_InstanceInit(&Coil_X_Neg, TIM1, LL_TIM_CHANNEL_CH2, TIM1, LL_TIM_CHANNEL_CH2N, CHANNEL_AMP_X_NEG, 0); //
    Coil_InstanceInit(&Coil_Y_Pos, TIM8, LL_TIM_CHANNEL_CH1, TIM8, LL_TIM_CHANNEL_CH1N, CHANNEL_AMP_Y_POS, 1); //
    Coil_InstanceInit(&Coil_Y_Neg, TIM1, LL_TIM_CHANNEL_CH1, TIM1, LL_TIM_CHANNEL_CH1N, CHANNEL_AMP_Y_NEG, 0); //
    Coil_InstanceInit(&Coil_LT, TIM8, LL_TIM_CHANNEL_CH3, TIM8, LL_TIM_CHANNEL_CH3N, CHANNEL_AMP_LT, 0);   // ALT_2, PC8 & PB15, TIM8_CH3 & CH3N, ADC1_IN9
    Coil_InstanceInit(&Coil_LB, TIM9, LL_TIM_CHANNEL_CH1, TIM9, LL_TIM_CHANNEL_CH2, CHANNEL_AMP_LB, 1);    // ALT_3, PE5 & PE6, TIM9_CH1 & CH2, ADC1_IN12
    Coil_InstanceInit(&Coil_RT, TIM8, LL_TIM_CHANNEL_CH2, TIM8, LL_TIM_CHANNEL_CH2N, CHANNEL_AMP_RT, 0);   // ALT_1, PC7 & PB14, TIM8_CH2 & CH2N, ADC1_IN8
    Coil_InstanceInit(&Coil_RB, TIM10, LL_TIM_CHANNEL_CH1, TIM11, LL_TIM_CHANNEL_CH1, CHANNEL_AMP_RB, 1);  // ALT_4, PB8 & PB9, TIM10_CH1 & TIM11_CH1, ADC1_IN13
    // 截止频率小于采样频率 10 倍以上, 选 1KHz
    Coil_CurrentPIDInit(&Coil_X_Pos, -450, -50, 0, 0.005f, AT8236_MAX_COMPARE);
    Coil_CurrentPIDInit(&Coil_X_Neg, -450, -50, 0, 0.005f, AT8236_MAX_COMPARE);
    Coil_CurrentPIDInit(&Coil_Y_Pos, 450, 50, 0, 0.005f, AT8236_MAX_COMPARE);
    Coil_CurrentPIDInit(&Coil_Y_Neg, -450, -50, 0, 0.005f, AT8236_MAX_COMPARE);
    Coil_CurrentPIDInit(&Coil_LT, 350, 50, 0, 0.005f, AT8236_MAX_COMPARE);
    Coil_CurrentPIDInit(&Coil_LB, -350, -50, 0, 0.005f, AT8236_MAX_COMPARE);
    Coil_CurrentPIDInit(&Coil_RT, -350, -50, 0, 0.005f, AT8236_MAX_COMPARE);
    Coil_CurrentPIDInit(&Coil_RB, 350, 50, 0, 0.005f, AT8236_MAX_COMPARE);

    // X_Axis_PID = PID_Init(-25.0f, 0, -2.6f, COIL_MAX_CURRENT_MA);    
    // Y_Axis_PID = PID_Init(-25.0f, 0, -1.8f, COIL_MAX_CURRENT_MA);
    // X_Axis_PID = PID_Init(-200.0f, 0, -3.2f, COIL_MAX_CURRENT_MA);    
    // Y_Axis_PID = PID_Init(-200.0f, 0, -2.8f, COIL_MAX_CURRENT_MA);
    // X_Axis_PID = PID_Init(-110.0f, 0, -3.2f, COIL_MAX_CURRENT_MA);    
    // Y_Axis_PID = PID_Init(-110.0f, 0, -2.9f, COIL_MAX_CURRENT_MA);
    X_Axis_PID = PID_Init(-110, 0, -3.2, COIL_MAX_CURRENT_MA);    
    Y_Axis_PID = PID_Init(-110, 0, -2.9, COIL_MAX_CURRENT_MA);
    X_Axis_Outer_PID = PID_Init(50, 0, 2, COIL_MAX_CURRENT_MA);
    Y_Axis_Outer_PID = PID_Init(50, 0, 2, COIL_MAX_CURRENT_MA);
    Z_Axis_Outer_PID = PID_Init(12, 150, 0, 4000);
    X_Modifier_PID = PID_Init(-25, 0, 0, X_MODIFIER_RANGE);
    Y_Modifier_PID = PID_Init(25, 0, 0, Y_MODIFIER_RANGE);
    // Z_Axis_PID = PID_Init(0, 0, 0, COIL_MAX_CURRENT_MA);
    // X_Axis_PID = FuzzyPID_Init(0, 0, 0, COIL_MAX_CURRENT_MA, 2048.0f, 0.1, 0, 0.0002); 
    // Y_Axis_PID = FuzzyPID_Init(0, 0, 0, COIL_MAX_CURRENT_MA, 2048.0f, 0.1, 0, 0.0002); 

    Hall_X_Filter = LPFilter_Init(0.005f);  // 截止频率小于采样频率 10 倍以上, 选 1KHz
    Hall_Y_Filter = LPFilter_Init(0.005f);
    TOF_Filter = SlidingFilter_Init(10);
    // TOF_Filter = LPFilter_Init(0.005f);
    // floater_height = SlidingFilter_GetOutput(&TOF_Filter, (float)(tof_distance - TOF_ZERO_OFFSET));
    control_state = 0;
    for(uint8_t i = 0; i < 100; i++)
    {
        roll_target += imu_roll;
        pitch_target += imu_pitch;
        hall_x_self_test_value += ADC_GetRaw(CHANNEL_HALL_X);
        hall_y_self_test_value += ADC_GetRaw(CHANNEL_HALL_Y);
        delay_ms(1);
    }
    roll_target /= 100.0f;
    pitch_target /= 100.0f;
    hall_x_self_test_value /= 100.0f;
    hall_y_self_test_value /= 100.0f;
    if(_abs(hall_x_self_test_value - HALL_ZERO_SET) >= HALL_ZERO_RANGE || _abs(hall_y_self_test_value - HALL_ZERO_SET) >= HALL_ZERO_RANGE)
    {
        // need recalibrating zero-position by manually switching the resistor
        control_state = 0;
    }
    else control_state = 1; // start control-loop
    LL_TIM_SetCounter(TIM3, 0);
    LL_TIM_EnableIT_UPDATE(TIM3);
    LL_TIM_EnableCounter(TIM3);
    LL_TIM_ClearFlag_UPDATE(TIM3);
}
void control_calculate_power(void)
{
    inner_total_current = (_abs(Coil_X_Pos._adc_current) + _abs(Coil_X_Neg._adc_current) + _abs(Coil_Y_Pos._adc_current) + _abs(Coil_Y_Neg._adc_current)) * 0.001;
    outer_total_current = (_abs(Coil_LT._adc_current) + _abs(Coil_LB._adc_current) + _abs(Coil_RT._adc_current) + _abs(Coil_RB._adc_current)) * 0.001;
    total_power = (ADC_GetVin() * 0.001) * (inner_total_current + outer_total_current); // Watt
}
// place in TIM_IT
static uint8_t timer_2 = 0;
static uint8_t timer_4 = 0;
static uint8_t timer_200 = 0;
static uint8_t timer_20 = 0;
float z_axis_addition_pid_value = 0.0f;
// static float outer_x_result = 0.0f, outer_y_result = 0.0f; // unused
void control_loop(void)
{
    // Inner loop, 20 KHz
    Coil_UpdateCurrent(&Coil_X_Pos);
    Coil_UpdateCurrent(&Coil_X_Neg);
    Coil_UpdateCurrent(&Coil_Y_Pos);
    Coil_UpdateCurrent(&Coil_Y_Neg);
    // Hall_X_Offset = LPFilter_GetOutput(&Hall_X_Filter, (2017.5f - ADC_GetRaw(CHANNEL_HALL_X)));
    // Hall_Y_Offset = LPFilter_GetOutput(&Hall_Y_Filter, (2017.5f - ADC_GetRaw(CHANNEL_HALL_Y)));
    // Hall_X_Offset = (2017.5f - ADC_GetRaw(CHANNEL_HALL_X));
    // Hall_Y_Offset = (2017.5f - ADC_GetRaw(CHANNEL_HALL_Y));
    if(timer_200 >= 199)
    {
        // 100 Hz = 0.01 s
        if((_abs(Coil_X_Pos._adc_current) + _abs(Coil_X_Neg._adc_current) + _abs(Coil_Y_Pos._adc_current) + _abs(Coil_Y_Neg._adc_current)) >= INNER_COIL_CURRENT_FUSE ||
           (_abs(Coil_LT._adc_current) + _abs(Coil_LB._adc_current) + _abs(Coil_RT._adc_current) + _abs(Coil_RB._adc_current)) >= OUTER_COIL_CURRENT_FUSE)
        {
            if(current_fuse_tick++ >= CURRENT_FUSE_THRESHOLD_TIME) 
            {
                // current_fuse = 1; // temporary disable
                // control_state = 0;
            }
        }
        else if(current_fuse && !floater_state && !floater_fail_safe)
        {
            current_fuse_tick = 0;
            current_fuse = 0;
            control_state = 1;
        }
        if(floater_state && !floater_fail_safe)
        {
            if(_abs(Coil_X_Pos._adc_current) <= FLOATER_STABLE_CURRENT_THRESHOLD && _abs(Coil_Y_Pos._adc_current) <= FLOATER_STABLE_CURRENT_THRESHOLD)
            {
                if(floater_steady_tick++ >= FLOATER_STABLE_TIME)
                {
                    floater_steady = 1;
                }
            }
            else
            {
                floater_steady_tick = 0;
                floater_steady = 0;
            }
        }
        else
        {
            floater_steady_tick = 0;
            floater_steady = 0;
        }
        if((float)(tof_distance - TOF_ZERO_OFFSET) > FLOATER_MAX_HEIGHT) floater_height = FLOATER_MAX_HEIGHT + 1;
        else floater_height = SlidingFilter_GetOutput(&TOF_Filter, (float)(tof_distance - TOF_ZERO_OFFSET));
        // floater_height = LPFilter_GetOutput(&TOF_Filter, (float)floater_raw);
        if(TOF_Filter._data_count == TOF_Filter.window_size)
        {
            if(floater_height <= FLOATER_FAIL_SAFE_HEIGHT)
            {
                if(floater_fail_safe_tick++ >= FLOATER_FAIL_SAFE_TIME)
                {
                    control_state = 0;
                    floater_fail_safe = 1;
                    floater_steady = 0;
                }
            }
            else
            {
                floater_fail_safe_tick = 0;
            }
            if(floater_height <= FLOATER_MAX_HEIGHT)
            {
                floater_state = 1;
                // X_Axis_PID.enable_integral = 1;
                // Y_Axis_PID.enable_integral = 1;
                Z_Axis_Outer_PID.enable_integral = 1;
            }
            else if(floater_state)
            {
                floater_state = 0;
                floater_steady = 0;
                // X_Axis_PID.enable_integral = 0;
                // Y_Axis_PID.enable_integral = 0;
                Z_Axis_Outer_PID.enable_integral = 0;
                Z_Axis_Outer_PID._integral_prev = 0;
            }
        }
        if(control_state && floater_state)
        {
            if(_abs(imu_pitch) >= MAX_UNCONTROL_ANGLE || _abs(imu_roll) >= MAX_UNCONTROL_ANGLE) // 如果倾斜角大于设定值, 则强制转入角度控制模式
            {
                position_control = 0;
                revolution_control = 0;
            }
            else if(revolution_control && position_control) 
            {
                position_control = 1; // 防止多个 Flag 同时出现
                revolution_control = 0;
            }
            if(revolution_control)
            {
                // arm_sin_f32 / arm_cos_f32: radian
                revolution_angle_rad += revolution_speed * REVOLUTION_SPEED_MULTIPLIER;
                if(revolution_angle_rad > CIRCLE_RAD) revolution_angle_rad -= CIRCLE_RAD;
                if(revolution_angle_rad < 0) revolution_angle_rad += CIRCLE_RAD;
                X_Modifier = revolution_r * arm_cos_f32(revolution_angle_rad);
                Y_Modifier = revolution_r * arm_sin_f32(revolution_angle_rad);
            }
            else if(!position_control)
            {
                X_Modifier = PID_GetOutput(&X_Modifier_PID, (roll_target - imu_roll));
                if(_abs(roll_target - imu_roll) <= 105)
                {
                    negative_gravity = 0;
                }
                else
                {
                    X_Modifier = PID_GetOutput(&X_Modifier_PID, 0.9f * (180.0f - (roll_target - imu_roll)));
                    negative_gravity = 1.0f + (((_abs((roll_target - imu_roll) - 90.0f)) / 90.0f));
                }
                    
                Y_Modifier = PID_GetOutput(&Y_Modifier_PID, (pitch_target - imu_pitch));
                revolution_angle_rad = 0;
            }
            // Coil_LT.set_current += outer_ring_set_current;
            // Coil_LB.set_current -= outer_ring_set_current;
            // Coil_RT.set_current -= outer_ring_set_current;
            // Coil_RB.set_current += outer_ring_set_current;
            // Coil_X_Pos.set_current += outer_ring_set_current;
            // Coil_X_Neg.set_current += outer_ring_set_current;
            // Coil_Y_Pos.set_current -= outer_ring_set_current;
            // Coil_Y_Neg.set_current -= outer_ring_set_current;
        }
        else if(!control_state)
        {
            revolution_control = 0;
            position_control = 0;
            // outer_ring_set_current = 0;
        }
        timer_200 = 0;
    }
    else timer_200++;
    if(timer_20 >= 19)
    {
        if(control_state && !low_power_mode && height_control && floater_state && !revolution_control && !position_control && (_abs(X_Modifier) <= MAX_Z_CONTROL_AXIS_MODIFIER && _abs(Y_Modifier) <= MAX_Z_CONTROL_AXIS_MODIFIER)) 
            outer_ring_set_current = PID_GetOutput(&Z_Axis_Outer_PID, (z_target - floater_height));
        // else outer_ring_set_current = 0; // commented for outside control in ui.c
        timer_20 = 0;
    }
    else timer_20++;
    if(timer_4 >= 3)
    {
        // 5 KHz
        if(!low_power_mode)
        {
            Coil_UpdateCurrent(&Coil_LT);
            Coil_UpdateCurrent(&Coil_LB);
            Coil_UpdateCurrent(&Coil_RT);
            Coil_UpdateCurrent(&Coil_RB);
            if(control_state && floater_state)
            {
                // Coil_LT.set_current = -outer_x_result - outer_y_result;
                // Coil_LB.set_current = outer_x_result - outer_y_result;
                // Coil_RT.set_current = -outer_x_result + outer_y_result;
                // Coil_RB.set_current = outer_x_result + outer_y_result;

                // if(X_Modifier < 0) // rolled right, we need to pull at LT/LB, and push at RT/RB
                // {
                //     // < 0 !!!
                //     Coil_LT.set_current += X_Modifier * OUTER_CURR_TO_MODIFIER_MULTIPLIER;
                //     Coil_LB.set_current -= X_Modifier * OUTER_CURR_TO_MODIFIER_MULTIPLIER;
                //     Coil_RT.set_current += X_Modifier * OUTER_CURR_TO_MODIFIER_MULTIPLIER;
                //     Coil_RB.set_current -= X_Modifier * OUTER_CURR_TO_MODIFIER_MULTIPLIER;
                // }
                // else // rolled left, we need to push at LT/LB, and pull at RT/RB
                // {
                //     // >= 0 !!!
                //     Coil_LT.set_current += X_Modifier * OUTER_CURR_TO_MODIFIER_MULTIPLIER;
                //     Coil_LB.set_current -= X_Modifier * OUTER_CURR_TO_MODIFIER_MULTIPLIER;
                //     Coil_RT.set_current += X_Modifier * OUTER_CURR_TO_MODIFIER_MULTIPLIER;
                //     Coil_RB.set_current -= X_Modifier * OUTER_CURR_TO_MODIFIER_MULTIPLIER;
                // }
                // if(Y_Modifier < 0) // pitched down, we need to pull at LT/RT, and push at LB/RB
                // {
                //     // < 0 !!!
                //     Coil_LT.set_current += Y_Modifier * OUTER_CURR_TO_MODIFIER_MULTIPLIER;
                //     Coil_RT.set_current -= Y_Modifier * OUTER_CURR_TO_MODIFIER_MULTIPLIER;
                //     Coil_LB.set_current += Y_Modifier * OUTER_CURR_TO_MODIFIER_MULTIPLIER;
                //     Coil_RB.set_current -= Y_Modifier * OUTER_CURR_TO_MODIFIER_MULTIPLIER;
                // }
                // else // pitched up, we need to push at LT/RT, and pull at LB/RB
                // {
                //     Coil_LT.set_current += Y_Modifier * OUTER_CURR_TO_MODIFIER_MULTIPLIER;
                //     Coil_RT.set_current -= Y_Modifier * OUTER_CURR_TO_MODIFIER_MULTIPLIER;
                //     Coil_LB.set_current += Y_Modifier * OUTER_CURR_TO_MODIFIER_MULTIPLIER;
                //     Coil_RB.set_current -= Y_Modifier * OUTER_CURR_TO_MODIFIER_MULTIPLIER;
                // }
                if(revolution_control || position_control)
                {
                    // outer_x_result = PID_GetOutput(&X_Axis_Outer_PID, (X_INSTALL_OFFSET + X_Modifier - Hall_X_Offset));
                    // outer_y_result = PID_GetOutput(&Y_Axis_Outer_PID, (Y_INSTALL_OFFSET + Y_Modifier - Hall_Y_Offset));
                    // Coil_LT.set_current = -outer_x_result - outer_y_result;
                    // Coil_LB.set_current = outer_x_result - outer_y_result;
                    // Coil_RT.set_current = -outer_x_result + outer_y_result;
                    // Coil_RB.set_current = outer_x_result + outer_y_result;
                    Coil_LT.set_current = 0;
                    Coil_LB.set_current = 0;
                    Coil_RT.set_current = 0;
                    Coil_RB.set_current = 0;
                    Coil_SetOutput(&Coil_LT, 0);
                    Coil_SetOutput(&Coil_LB, 0);
                    Coil_SetOutput(&Coil_RT, 0);
                    Coil_SetOutput(&Coil_RB, 0);
                }
                else
                {
                    // outer_x_result = PID_GetOutput(&X_Axis_Outer_PID, (X_INSTALL_OFFSET + X_Modifier - Hall_X_Offset));
                    // outer_y_result = PID_GetOutput(&Y_Axis_Outer_PID, (Y_INSTALL_OFFSET + Y_Modifier - Hall_Y_Offset));
                    // Coil_LT.set_current = -outer_x_result - outer_y_result + outer_ring_set_current;
                    // Coil_LB.set_current = outer_x_result - outer_y_result - outer_ring_set_current;
                    // Coil_RT.set_current = -outer_x_result + outer_y_result - outer_ring_set_current;
                    // Coil_RB.set_current = outer_x_result + outer_y_result + outer_ring_set_current;
                    Coil_LT.set_current = outer_ring_set_current;
                    Coil_LB.set_current = -outer_ring_set_current;
                    Coil_RT.set_current = -outer_ring_set_current;
                    Coil_RB.set_current = outer_ring_set_current;
                    if(!negative_gravity)
                    {
                        Coil_LT.set_current += (X_Modifier + Y_Modifier) * OUTER_MODIFIER_CURR_MULTIPLIER;
                        Coil_LB.set_current += (-X_Modifier + Y_Modifier) * OUTER_MODIFIER_CURR_MULTIPLIER;
                        Coil_RT.set_current += (X_Modifier - Y_Modifier) * OUTER_MODIFIER_CURR_MULTIPLIER;
                        Coil_RB.set_current += (-X_Modifier - Y_Modifier) * OUTER_MODIFIER_CURR_MULTIPLIER;
                    }
                    else if(negative_gravity < 1.3f)
                    {
                        // all pull
                        Coil_LT.set_current += -1200 * 1.78f * negative_gravity;
                        Coil_LB.set_current -= -1200 * 1.78f * negative_gravity;
                        Coil_RT.set_current -= -1200 * 0.8f * negative_gravity;
                        Coil_RB.set_current += -1200 * 0.8f * negative_gravity;
                    }
                    else if(negative_gravity < 1.4f)
                    {
                        // all pull
                        Coil_LT.set_current += -1200 * 1.82f * negative_gravity;
                        Coil_LB.set_current -= -1200 * 1.82f * negative_gravity;
                        Coil_RT.set_current -= -1200 * 1.0f * negative_gravity;
                        Coil_RB.set_current += -1200 * 1.0f * negative_gravity;
                    }
                    else if(negative_gravity < 1.46f)
                    {
                        // all pull
                        Coil_LT.set_current += -1200 * 2.2f * negative_gravity;
                        Coil_LB.set_current -= -1200 * 2.2f * negative_gravity;
                        Coil_RT.set_current -= -1200 * 1.1f * negative_gravity;
                        Coil_RB.set_current += -1200 * 1.1f * negative_gravity;
                    }
                    else if(negative_gravity < 1.53f)
                    {
                        Coil_LT.set_current += -1200 * 3.5f * negative_gravity;
                        Coil_LB.set_current -= -1200 * 3.5f * negative_gravity;
                        Coil_RT.set_current -= -1200 * 1.3f * negative_gravity;
                        Coil_RB.set_current += -1200 * 1.3f * negative_gravity;
                    }
                    else
                    {
                        Coil_LT.set_current += -1200 * 3.6f * negative_gravity;
                        Coil_LB.set_current -= -1200 * 3.6f * negative_gravity;
                        Coil_RT.set_current -= -1200 * 1.4f * negative_gravity;
                        Coil_RB.set_current += -1200 * 1.4f * negative_gravity;
                    }
                    Coil_CalCurrentPID(&Coil_LT);
                    Coil_CalCurrentPID(&Coil_LB);
                    Coil_CalCurrentPID(&Coil_RT);
                    Coil_CalCurrentPID(&Coil_RB);
                }
                // Coil_CalCurrentPID(&Coil_LT);
                // Coil_CalCurrentPID(&Coil_LB);
                // Coil_CalCurrentPID(&Coil_RT);
                // Coil_CalCurrentPID(&Coil_RB);
            }
        }
        
        timer_4 = 0;
    }
    else timer_4++;
    if(timer_2 >= 1)
    {
        // process outer PID, 10 KHz
        // Hall output max = 4095, min = 60, so middle value is (4095 - 60) / 2 = 2017.5f
        // Hall_X_Offset = (2017.5f - ADC_GetRaw(CHANNEL_HALL_X));
        // Hall_Y_Offset = (2017.5f - ADC_GetRaw(CHANNEL_HALL_Y));
        Hall_X_Offset = LPFilter_GetOutput(&Hall_X_Filter, (hall_x_self_test_value - ADC_GetRaw(CHANNEL_HALL_X)));
        Hall_Y_Offset = LPFilter_GetOutput(&Hall_Y_Filter, (hall_y_self_test_value - ADC_GetRaw(CHANNEL_HALL_Y)));
        // Hall_X_Offset = ADC_GetRaw(CHANNEL_HALL_X);
        // Hall_Y_Offset = ADC_GetRaw(CHANNEL_HALL_Y);
        // if(floater_state) z_axis_addition_pid_value = PID_GetOutput(&Z_Axis_PID, (z_target - tof_distance));
        // else z_axis_addition_pid_value = 0;
        if(control_state && floater_state)
        {
            Coil_Y_Pos.set_current = PID_GetOutput(&Y_Axis_PID, (Y_INSTALL_OFFSET + Y_Modifier - Hall_Y_Offset));
            Coil_Y_Neg.set_current = -Coil_Y_Pos.set_current;

            Coil_X_Pos.set_current = PID_GetOutput(&X_Axis_PID, (X_INSTALL_OFFSET + X_Modifier - Hall_X_Offset));
            Coil_X_Neg.set_current = -Coil_X_Pos.set_current;

            // Coil_X_Pos.set_current += outer_ring_set_current;
            // Coil_X_Neg.set_current += outer_ring_set_current;
            // Coil_Y_Pos.set_current -= outer_ring_set_current;
            // Coil_Y_Neg.set_current -= outer_ring_set_current;
            if(!revolution_control)
            {
                if(negative_gravity)
                {
                    if(negative_gravity < 1.3f)
                    {
                        Coil_X_Pos.set_current += -1200 * 0.5f * negative_gravity;
                        Coil_X_Neg.set_current += -1200 * 0.5f * negative_gravity;
                        Coil_Y_Pos.set_current -= -1200 * 0.5f * negative_gravity;
                        Coil_Y_Neg.set_current -= -1200 * 0.5f * negative_gravity;
                    }
                    else
                    {
                        Coil_X_Pos.set_current += -1200 * 0.7f * negative_gravity;
                        Coil_X_Neg.set_current += -1200 * 0.7f * negative_gravity;
                        Coil_Y_Pos.set_current -= -1200 * 0.7f * negative_gravity;
                        Coil_Y_Neg.set_current -= -1200 * 0.7f * negative_gravity;
                    }

                }
                else
                {
                    if(X_Modifier < 0) // rolled right, we need to pull at X_Neg/Y_Pos/Y_Neg, and push at X_Pos
                    {
                        // < 0 !!
                        Coil_X_Pos.set_current -= X_Modifier * INNER_MODIFIER_CURR_MULTIPLIER;
                        Coil_X_Neg.set_current += X_Modifier * INNER_MODIFIER_CURR_MULTIPLIER;
                        Coil_Y_Pos.set_current -= X_Modifier * INNER_MODIFIER_CURR_MULTIPLIER;
                        Coil_Y_Neg.set_current -= X_Modifier * INNER_MODIFIER_CURR_MULTIPLIER;
                    }
                    else // rolled left, we need to pull at X_Pos/Y_Pos/Y_Neg, and push at X_Neg
                    {
                        // > 0 !!
                        Coil_X_Pos.set_current -= X_Modifier * INNER_MODIFIER_CURR_MULTIPLIER;
                        Coil_X_Neg.set_current += X_Modifier * INNER_MODIFIER_CURR_MULTIPLIER;
                        Coil_Y_Pos.set_current += X_Modifier * INNER_MODIFIER_CURR_MULTIPLIER;
                        Coil_Y_Neg.set_current += X_Modifier * INNER_MODIFIER_CURR_MULTIPLIER;
                    }
                    if(Y_Modifier < 0) // pitched down, we need to pull at Y_Pos/X_Pos/X_Neg, and push at Y_Neg
                    {
                        // < 0 !!
                        Coil_X_Pos.set_current += Y_Modifier * INNER_MODIFIER_CURR_MULTIPLIER;
                        Coil_X_Neg.set_current += Y_Modifier * INNER_MODIFIER_CURR_MULTIPLIER;
                        Coil_Y_Pos.set_current -= Y_Modifier * INNER_MODIFIER_CURR_MULTIPLIER;
                        Coil_Y_Neg.set_current += Y_Modifier * INNER_MODIFIER_CURR_MULTIPLIER;
                    }
                    else // pitched up, we need to pull at Y_Neg/X_Pos/X_Neg, and push at Y_Pos
                    {
                        // > 0 !!
                        Coil_X_Pos.set_current -= Y_Modifier * INNER_MODIFIER_CURR_MULTIPLIER;
                        Coil_X_Neg.set_current -= Y_Modifier * INNER_MODIFIER_CURR_MULTIPLIER;
                        Coil_Y_Pos.set_current -= Y_Modifier * INNER_MODIFIER_CURR_MULTIPLIER;
                        Coil_Y_Neg.set_current += Y_Modifier * INNER_MODIFIER_CURR_MULTIPLIER;
                    }
                    // Coil_X_Pos.set_current += -1200 * 0.3f * negative_gravity;
                    // Coil_X_Neg.set_current += -1200 * 0.3f * negative_gravity;
                    // Coil_Y_Pos.set_current -= -1200 * 0.3f * negative_gravity;
                    // Coil_Y_Neg.set_current -= -1200 * 0.3f * negative_gravity;
                }
            }
            
            // modification area
            // Coil_X_Pos.set_current -= z_axis_addition_pid_value;
            // Coil_X_Neg.set_current += z_axis_addition_pid_value;

            // modification area
            // Coil_Y_Pos.set_current += z_axis_addition_pid_value;
            // Coil_Y_Neg.set_current += z_axis_addition_pid_value;
        }
        else
        {
            Coil_X_Pos.set_current = 0;
            Coil_X_Neg.set_current = 0;
            Coil_Y_Pos.set_current = 0;
            Coil_Y_Neg.set_current = 0;
            Coil_LT.set_current = 0;
            Coil_LB.set_current = 0;
            Coil_RT.set_current = 0;
            Coil_RB.set_current = 0;
            outer_ring_set_current = 0;
            Coil_SetOutput(&Coil_X_Pos, 0);
            Coil_SetOutput(&Coil_X_Neg, 0);
            Coil_SetOutput(&Coil_Y_Pos, 0);
            Coil_SetOutput(&Coil_Y_Neg, 0);
            Coil_SetOutput(&Coil_LT, 0);
            Coil_SetOutput(&Coil_LB, 0);
            Coil_SetOutput(&Coil_RT, 0);
            Coil_SetOutput(&Coil_RB, 0);
        }
        timer_2 = 0;
    }
    else timer_2++;
    if(control_state)
    {
        Coil_CalCurrentPID(&Coil_X_Pos);
        Coil_CalCurrentPID(&Coil_X_Neg);
        Coil_CalCurrentPID(&Coil_Y_Pos);
        Coil_CalCurrentPID(&Coil_Y_Neg);
    }

}