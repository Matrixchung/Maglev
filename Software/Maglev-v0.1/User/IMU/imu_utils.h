#ifndef _IMU_UTILS_H
#define _IMU_UTILS_H

#include "math_utils.h"
#include "delay.h"

void MadgwickAHRSupdate_6(float gx, float gy, float gz, float ax, float ay, float az, float *pitch, float *roll);

#endif