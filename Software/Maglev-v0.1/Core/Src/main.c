/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ui.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t timer_1ms = 0;
uint8_t timer_10ms = 0;
uint8_t timer_50ms = 0;
uint16_t timer_500ms = 0;
int16_t temp = 0;

uint16_t rxLen = 0;
char buffer[128] = {0};
char command_buffer[MAX_COMMAND_WORD_LENGTH] = {0};
float arg_buffer[MAX_FLOAT_ARG_COUNT] = {0.0f};
uint8_t arg_buffer_len = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void EC11_Callback(void);
void Key_Onboard_Callback(void);
void Key_EC11_Callback(void);
void UART_CommandHandler(UART_InstanceTypeDef *uart_handler);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#if (__FPU_USED == 0) || (__FPU_PRESENT == 0)
  #warning "FPU is not used"
#endif
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM8_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_USART3_UART_Init();
  MX_SPI1_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  MX_TIM9_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */
  DWT_Delay_Init();
  // LL Driver turns off SysTick_Handler() interrupt without SysTick_CTRL_TICKINT_Msk set.
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_TICKINT_Msk |
                  SysTick_CTRL_ENABLE_Msk;
  delay_ms(300);
  ADC_HandlerInit();
  UART_InstanceInit(&UART1_Handler, USART1, DMA2, LL_DMA_STREAM_2, DMA2, LL_DMA_STREAM_7, 256);
  TOF_Sensor_Init();
  Encoder_Init(&EC11_Encoder, TIM4, LL_TIM_CHANNEL_CH1);
  Encoder_RegisterCallback(&EC11_Encoder, EC11_Callback);
  Key_InstanceInit(&Key_Onboard, KEY_ONBOARD_GPIO_Port, KEY_ONBOARD_Pin, 0);
  Key_InstanceInit(&Key_EC11, KEY_EC11_GPIO_Port, KEY_EC11_Pin, 0);
  Key_RegisterCallback(&Key_Onboard, Key_Onboard_Callback);
  Key_RegisterCallback(&Key_EC11, Key_EC11_Callback);
  OLED_Init();
  BLE_Init();
  ICM20602_Init();
  control_init();
  if(ble_state) UART_printf(&UART_BLE_Handler, 1, "Maglev System initialized.\n");
  // UART_printf(&UART1_Handler, 1, "Hello World!");
  // uint16_t tempLen = 0;
  // uint8_t buf[128] = {0};
  /* USER CODE END 2 */
  uint8_t last_floater_state = 0;
  uint8_t last_floater_steady = 0;
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while(1)
  {
    // BLE stuffs
    if(ble_state)
    {
      if(floater_state != last_floater_state)
      {
        if(floater_state) UART_printf(&UART_BLE_Handler, 1, "Floater has been put.\n");
        else UART_printf(&UART_BLE_Handler, 1, "Floater has been removed.\n");
        last_floater_state = floater_state;
      }
      if(floater_steady != last_floater_steady)
      {
        if(floater_steady) UART_printf(&UART_BLE_Handler, 1, "Floater steady.\n");
        else UART_printf(&UART_BLE_Handler, 1, "Floater unsteady.\n");
        last_floater_steady = floater_steady;
      }
      if(UART_BLE_Handler.rx_finished_flag)
      {
        UART_CommandHandler(&UART_BLE_Handler);
        UART_BLE_Handler.rx_finished_flag = 0;
      }
    }
 
    if(UART1_Handler.rx_finished_flag)
    {
      UART_CommandHandler(&UART1_Handler);
      UART1_Handler.rx_finished_flag = 0;
    }
    if(timer_1ms)
    {
      ICM20602_UpdateData(); // 1 KHz
      ICM20602_UpdateAHRS();
      if(timer_500ms > 0 && !(timer_500ms % 249)) // 250ms
      {
        if(current_fuse) LL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
        timer_500ms += 1;
      }
      else if(timer_500ms >= 499)
      {
        if(floater_fail_safe) LL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
        timer_500ms = 0;
      }
      else timer_500ms++;
      if(timer_50ms >= 49)
      {
        control_calculate_power();
        // OLED_printf(0, 0, SMALL_FONT, "H: %d    ", (uint16_t)floater_height);
        // OLED_printf(0, 14, SMALL_FONT, "temp: %d C", (int8_t)ADC_GetCoreTemp());
        // OLED_printf(0, 28, SMALL_FONT, "r: %d, p: %d ", (int16_t)imu_roll, (int16_t)imu_pitch);
        // OLED_printf(0, 42, SMALL_FONT, "%dEC: %d  ", EC11_Encoder.direction, EC11_Encoder.value);
        // OLED_RefreshGram();
        UI_Show();
        timer_50ms = 0;
      }
      else timer_50ms++;
      if(timer_10ms >= 9)
      {
        if(control_state && !low_power_mode) 
        {
          LL_GPIO_SetOutputPin(LED_2_GPIO_Port, LED_2_Pin);
          if(floater_state) LL_GPIO_SetOutputPin(LED_1_GPIO_Port, LED_1_Pin);
          else LL_GPIO_ResetOutputPin(LED_1_GPIO_Port, LED_1_Pin);
        }
        else 
        {
          if(!(floater_fail_safe || current_fuse)) LL_GPIO_ResetOutputPin(LED_2_GPIO_Port, LED_2_Pin); // not triggered by fail safe or current fuse
          LL_GPIO_ResetOutputPin(LED_1_GPIO_Port, LED_1_Pin);
        }
        vofa_add_float(0, floater_height);
        vofa_add_float(1, Hall_X_Offset);
        vofa_add_float(2, Hall_Y_Offset);
        vofa_add_float(3, ADC_GetRaw(CHANNEL_HALL_Z));
        vofa_add_float(4, Coil_X_Pos._adc_current);
        vofa_add_float(5, Coil_X_Neg._adc_current);
        vofa_add_float(6, Coil_Y_Pos._adc_current);
        vofa_add_float(7, Coil_Y_Neg._adc_current);
        vofa_add_float(8, Coil_LT._adc_current);
        vofa_add_float(9, Coil_LB._adc_current);
        vofa_add_float(10, Coil_RT._adc_current);
        vofa_add_float(11, Coil_RB._adc_current);
        vofa_add_float(12, (float)Coil_X_Pos.value);
        vofa_add_float(13, (float)Coil_X_Neg.value);
        vofa_add_float(14, (float)Coil_Y_Pos.value);
        vofa_add_float(15, (float)Coil_Y_Neg.value);
        vofa_add_float(16, (float)Coil_LT.value);
        vofa_add_float(17, (float)Coil_LB.value);
        vofa_add_float(18, (float)Coil_RT.value);
        vofa_add_float(19, (float)Coil_RB.value);
        vofa_add_float(20, Coil_X_Pos.set_current);
        vofa_add_float(21, Coil_X_Neg.set_current);
        vofa_add_float(22, Coil_Y_Pos.set_current);
        vofa_add_float(23, Coil_Y_Neg.set_current);
        vofa_add_float(24, Coil_LT.set_current);
        vofa_add_float(25, Coil_LB.set_current);
        vofa_add_float(26, Coil_RT.set_current);
        vofa_add_float(27, Coil_RB.set_current);
        vofa_add_float(28, imu_roll);
        vofa_add_float(29, imu_pitch);
        vofa_add_float(30, negative_gravity);
        // vofa_add_float(28, X_Axis_PID.P);
        // vofa_add_float(29, X_Axis_PID.I);
        // vofa_add_float(30, X_Axis_PID.D);
        // vofa_add_float(31, Y_Axis_PID.P);
        // vofa_add_float(32, Y_Axis_PID.I);
        // vofa_add_float(33, Y_Axis_PID.D);
        vofa_add_float(31, Z_Axis_Outer_PID.P);
        vofa_add_float(32, Z_Axis_Outer_PID.I);
        vofa_add_float(33, Z_Axis_Outer_PID.D);
        vofa_add_float(34, z_target);
        vofa_add_float(35, X_Modifier);
        vofa_add_float(36, Y_Modifier);
        vofa_send_data();
        Key_InstanceUpdate(&Key_Onboard);
        Key_InstanceUpdate(&Key_EC11);
        timer_10ms = 0;
      }
      else timer_10ms++;
      timer_1ms = 0;
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_5)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(168000000);
  LL_SetSystemCoreClock(168000000);
}

/* USER CODE BEGIN 4 */
void UART_CommandHandler(UART_InstanceTypeDef *uart_handler)
{
  rxLen = UART_GetRxLen(uart_handler);
  if(rxLen > 0)
  {
    UART_GetRxFifo(uart_handler, buffer, rxLen);
    if(buffer[rxLen-1] == '\r' || buffer[rxLen-1] == ';' || buffer[rxLen-1] == '\n')
    {
      getCommand(buffer, rxLen, command_buffer);
      splitData_f(buffer, rxLen, arg_buffer, &arg_buffer_len, ' ');
      if(arg_buffer_len == 0)
      {
        if(strcmp(command_buffer, "control_state") == 0)
        {
          control_state = !control_state;
          if(control_state) LL_GPIO_SetOutputPin(LED_2_GPIO_Port, LED_2_Pin);
          else LL_GPIO_ResetOutputPin(LED_2_GPIO_Port, LED_2_Pin);
        }
      }
      else if(arg_buffer_len == 1)
      {
        if(strcmp(command_buffer, "x_pos_compare") == 0)      Coil_SetOutput(&Coil_X_Pos, (int16_t)arg_buffer[0]);
        else if(strcmp(command_buffer, "x_neg_compare") == 0) Coil_SetOutput(&Coil_X_Neg, (int16_t)arg_buffer[0]);
        else if(strcmp(command_buffer, "y_pos_compare") == 0) Coil_SetOutput(&Coil_Y_Pos, (int16_t)arg_buffer[0]);
        else if(strcmp(command_buffer, "y_neg_compare") == 0) Coil_SetOutput(&Coil_Y_Neg, (int16_t)arg_buffer[0]);
        else if(strcmp(command_buffer, "lt_compare") == 0)    Coil_SetOutput(&Coil_LT, (int16_t)arg_buffer[0]);
        else if(strcmp(command_buffer, "lb_compare") == 0)    Coil_SetOutput(&Coil_LB, (int16_t)arg_buffer[0]);
        else if(strcmp(command_buffer, "rt_compare") == 0)    Coil_SetOutput(&Coil_RT, (int16_t)arg_buffer[0]);
        else if(strcmp(command_buffer, "rb_compare") == 0)    Coil_SetOutput(&Coil_RB, (int16_t)arg_buffer[0]);
        else if(strcmp(command_buffer, "x_pos_current") == 0) Coil_X_Pos.set_current = arg_buffer[0];
        else if(strcmp(command_buffer, "x_pos_p") == 0)       Coil_X_Pos.current_pid.P = arg_buffer[0];
        else if(strcmp(command_buffer, "x_pos_i") == 0)       Coil_X_Pos.current_pid.I = arg_buffer[0];
        else if(strcmp(command_buffer, "x_pos_d") == 0)       Coil_X_Pos.current_pid.D = arg_buffer[0];
        else if(strcmp(command_buffer, "x_neg_current") == 0) Coil_X_Neg.set_current = arg_buffer[0];
        else if(strcmp(command_buffer, "x_neg_p") == 0)       Coil_X_Neg.current_pid.P = arg_buffer[0];
        else if(strcmp(command_buffer, "x_neg_i") == 0)       Coil_X_Neg.current_pid.I = arg_buffer[0];
        else if(strcmp(command_buffer, "x_neg_d") == 0)       Coil_X_Neg.current_pid.D = arg_buffer[0];
        else if(strcmp(command_buffer, "y_pos_current") == 0) Coil_Y_Pos.set_current = arg_buffer[0];
        else if(strcmp(command_buffer, "y_pos_p") == 0)       Coil_Y_Pos.current_pid.P = arg_buffer[0];
        else if(strcmp(command_buffer, "y_pos_i") == 0)       Coil_Y_Pos.current_pid.I = arg_buffer[0];
        else if(strcmp(command_buffer, "y_pos_d") == 0)       Coil_Y_Pos.current_pid.D = arg_buffer[0];
        else if(strcmp(command_buffer, "y_neg_current") == 0) Coil_Y_Neg.set_current = arg_buffer[0];
        else if(strcmp(command_buffer, "y_neg_p") == 0)       Coil_Y_Neg.current_pid.P = arg_buffer[0];
        else if(strcmp(command_buffer, "y_neg_i") == 0)       Coil_Y_Neg.current_pid.I = arg_buffer[0];
        else if(strcmp(command_buffer, "y_neg_d") == 0)       Coil_Y_Neg.current_pid.D = arg_buffer[0];
        else if(strcmp(command_buffer, "x_axis_p") == 0)      X_Axis_PID.P = arg_buffer[0];
        else if(strcmp(command_buffer, "x_axis_i") == 0)      X_Axis_PID.I = arg_buffer[0];
        else if(strcmp(command_buffer, "x_axis_d") == 0)      X_Axis_PID.D = arg_buffer[0];
        else if(strcmp(command_buffer, "y_axis_p") == 0)      Y_Axis_PID.P = arg_buffer[0];
        else if(strcmp(command_buffer, "y_axis_i") == 0)      Y_Axis_PID.I = arg_buffer[0];
        else if(strcmp(command_buffer, "y_axis_d") == 0)      Y_Axis_PID.D = arg_buffer[0];
        else if(strcmp(command_buffer, "z_axis_p") == 0)      Z_Axis_PID.P = arg_buffer[0];
        else if(strcmp(command_buffer, "z_axis_i") == 0)      Z_Axis_PID.I = arg_buffer[0];
        else if(strcmp(command_buffer, "z_axis_d") == 0)      Z_Axis_PID.D = arg_buffer[0];
        else if(strcmp(command_buffer, "z_target") == 0)      z_target = arg_buffer[0];
        else if(strcmp(command_buffer, "x_modifier") == 0)    X_Modifier = arg_buffer[0];
        else if(strcmp(command_buffer, "y_modifier") == 0)    Y_Modifier = arg_buffer[0];
        else if(strcmp(command_buffer, "lt_current") == 0)    Coil_LT.set_current = arg_buffer[0];
        else if(strcmp(command_buffer, "lt_p") == 0)          Coil_LT.current_pid.P = arg_buffer[0];
        else if(strcmp(command_buffer, "lt_i") == 0)          Coil_LT.current_pid.I = arg_buffer[0];
        else if(strcmp(command_buffer, "lt_d") == 0)          Coil_LT.current_pid.D = arg_buffer[0];
        else if(strcmp(command_buffer, "lb_current") == 0)    Coil_LB.set_current = arg_buffer[0];
        else if(strcmp(command_buffer, "lb_p") == 0)          Coil_LB.current_pid.P = arg_buffer[0];
        else if(strcmp(command_buffer, "lb_i") == 0)          Coil_LB.current_pid.I = arg_buffer[0];
        else if(strcmp(command_buffer, "lb_d") == 0)          Coil_LB.current_pid.D = arg_buffer[0];
        else if(strcmp(command_buffer, "rt_current") == 0)    Coil_RT.set_current = arg_buffer[0];
        else if(strcmp(command_buffer, "rt_p") == 0)          Coil_RT.current_pid.P = arg_buffer[0];
        else if(strcmp(command_buffer, "rt_i") == 0)          Coil_RT.current_pid.I = arg_buffer[0];
        else if(strcmp(command_buffer, "rt_d") == 0)          Coil_RT.current_pid.D = arg_buffer[0];
        else if(strcmp(command_buffer, "rb_current") == 0)    Coil_RB.set_current = arg_buffer[0];
        else if(strcmp(command_buffer, "rb_p") == 0)          Coil_RB.current_pid.P = arg_buffer[0];
        else if(strcmp(command_buffer, "rb_i") == 0)          Coil_RB.current_pid.I = arg_buffer[0];
        else if(strcmp(command_buffer, "rb_d") == 0)          Coil_RB.current_pid.D = arg_buffer[0];
        else if(strcmp(command_buffer, "outer_current") == 0) outer_ring_set_current = arg_buffer[0];
        else if(strcmp(command_buffer, "outer_x_p") == 0)     X_Axis_Outer_PID.P = arg_buffer[0];
        else if(strcmp(command_buffer, "outer_x_i") == 0)     X_Axis_Outer_PID.I = arg_buffer[0];
        else if(strcmp(command_buffer, "outer_x_d") == 0)     X_Axis_Outer_PID.D = arg_buffer[0];
        else if(strcmp(command_buffer, "outer_y_p") == 0)     Y_Axis_Outer_PID.P = arg_buffer[0];
        else if(strcmp(command_buffer, "outer_y_i") == 0)     Y_Axis_Outer_PID.I = arg_buffer[0];
        else if(strcmp(command_buffer, "outer_y_d") == 0)     Y_Axis_Outer_PID.D = arg_buffer[0];
        else if(strcmp(command_buffer, "outer_z_p") == 0)     Z_Axis_Outer_PID.P = arg_buffer[0];
        else if(strcmp(command_buffer, "outer_z_i") == 0)     Z_Axis_Outer_PID.I = arg_buffer[0];
        else if(strcmp(command_buffer, "outer_z_d") == 0)     Z_Axis_Outer_PID.D = arg_buffer[0];
        else if(strcmp(command_buffer, "x_modifier_p") == 0)  X_Modifier_PID.P = arg_buffer[0];
        else if(strcmp(command_buffer, "x_modifier_i") == 0)  X_Modifier_PID.I = arg_buffer[0];
        else if(strcmp(command_buffer, "x_modifier_d") == 0)  X_Modifier_PID.D = arg_buffer[0];
        else if(strcmp(command_buffer, "y_modifier_p") == 0)  Y_Modifier_PID.P = arg_buffer[0];
        else if(strcmp(command_buffer, "y_modifier_i") == 0)  Y_Modifier_PID.I = arg_buffer[0];
        else if(strcmp(command_buffer, "y_modifier_d") == 0)  Y_Modifier_PID.D = arg_buffer[0];
        else if(strcmp(command_buffer, "x_modifier") == 0)    X_Modifier = arg_buffer[0];
        else if(strcmp(command_buffer, "y_modifier") == 0)    Y_Modifier = arg_buffer[0];
        else if(strcmp(command_buffer, "height_control") == 0)height_control = (uint8_t)arg_buffer[0];
        else if(strcmp(command_buffer, "pos_control") == 0)   position_control = (uint8_t)arg_buffer[0];
        else if(strcmp(command_buffer, "rev_control") == 0)   revolution_control = (uint8_t)arg_buffer[0];
        else if(strcmp(command_buffer, "rev_r") == 0)         revolution_r = arg_buffer[0];
        else if(strcmp(command_buffer, "rev_speed") == 0)     revolution_speed = arg_buffer[0];
      }
    }
  }
}
void EC11_Callback(void)
{
  // LL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
  // temp = EC11_Encoder.value * 100;
  // if(temp > 8400) temp = 8400;
  // if(temp < -8400) temp = -8400;
  // Coil_SetOutput(&Coil_Main, temp);
  if(EC11_Encoder.direction == 0) UI_ButtonEvents(BUTTON_LEFT);
  else UI_ButtonEvents(BUTTON_RIGHT);
}
void Key_Onboard_Callback(void)
{
  if(Key_Onboard.state == PRESSED) LL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
  else if(Key_Onboard.state == LONG_PRESSED)
  {
    LL_GPIO_SetOutputPin(LED_1_GPIO_Port, LED_1_Pin);
    LL_GPIO_SetOutputPin(LED_2_GPIO_Port, LED_2_Pin);
  }
  else
  {
    LL_GPIO_ResetOutputPin(LED_1_GPIO_Port, LED_1_Pin);
    LL_GPIO_ResetOutputPin(LED_2_GPIO_Port, LED_2_Pin);
  }
}
void Key_EC11_Callback(void)
{
  // if(Key_EC11.state == PRESSED) LL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
  // else if(Key_EC11.state == LONG_PRESSED)
  // {
  //   LL_GPIO_SetOutputPin(LED_1_GPIO_Port, LED_1_Pin);
  //   LL_GPIO_SetOutputPin(LED_2_GPIO_Port, LED_2_Pin);
  // }
  // else
  // {
  //   LL_GPIO_ResetOutputPin(LED_1_GPIO_Port, LED_1_Pin);
  //   LL_GPIO_ResetOutputPin(LED_2_GPIO_Port, LED_2_Pin);
  // }
  if(Key_EC11.state == PRESSED) UI_ButtonEvents(BUTTON_PRESS);
  else if(Key_EC11.state == LONG_PRESSED) UI_ButtonEvents(BUTTON_LONG_PRESS);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
