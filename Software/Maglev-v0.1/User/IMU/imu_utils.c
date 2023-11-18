#include "imu_utils.h"
#define BETA 0.1f
#define GYRO_K (2.664624812205083e-4f) // (1.0f/65.5f/RAD_2_DEG) // 65.5f is based on ICM20602 settings
#define ACC_K  (0.0001220703125f) // (1.0f/8192.0f) // based on ICM20602 settings
float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
void MadgwickAHRSupdate_6(float gx, float gy, float gz, float ax, float ay, float az, float *pitch, float *roll) 
{
    static uint32_t madgwick_timer = 0;
	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;
    float Ts = (get_micros() - madgwick_timer) * 1e-6f;
    if(Ts <= 0 || Ts > 0.5f) Ts = 1e-3f;
    madgwick_timer = get_micros();
    ax *= ACC_K;
    ay *= ACC_K;
    az *= ACC_K;
	gx *= GYRO_K;
	gy *= GYRO_K;
	gz *= GYRO_K;
	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
	qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
	qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
	qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);
	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) 
    {
		// Normalise accelerometer measurement
		recipNorm = fast_invsqrt(ax * ax + ay * ay + az * az);
		// ax *= recipNorm;
		// ay *= recipNorm;
		// az *= recipNorm;
        arm_mult_f32(&ax, &recipNorm, &ax, 1);
        arm_mult_f32(&ay, &recipNorm, &ay, 1);
        arm_mult_f32(&az, &recipNorm, &az, 1);
		// Auxiliary variables to avoid repeated arithmetic
		_2q0 = 2.0f * q0;
		_2q1 = 2.0f * q1;
		_2q2 = 2.0f * q2;
		_2q3 = 2.0f * q3;
		_4q0 = 4.0f * q0;
		_4q1 = 4.0f * q1;
		_4q2 = 4.0f * q2;
		_8q1 = 8.0f * q1;
		_8q2 = 8.0f * q2;
		// q0q0 = q0 * q0;
		// q1q1 = q1 * q1;
		// q2q2 = q2 * q2;
		// q3q3 = q3 * q3;
        arm_power_f32(&q0, 1, &q0q0);
        arm_power_f32(&q1, 1, &q1q1);
        arm_power_f32(&q2, 1, &q2q2);
        arm_power_f32(&q3, 1, &q3q3);
		// Gradient decent algorithm corrective step
		s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
		s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
		s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
		s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;
		recipNorm = fast_invsqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		// s0 *= recipNorm;
		// s1 *= recipNorm;
		// s2 *= recipNorm;
		// s3 *= recipNorm;
        arm_mult_f32(&s0, &recipNorm, &s0, 1);
        arm_mult_f32(&s1, &recipNorm, &s1, 1);
        arm_mult_f32(&s2, &recipNorm, &s2, 1);
        arm_mult_f32(&s3, &recipNorm, &s3, 1);
		// Apply feedback step
		qDot1 -= BETA * s0;
		qDot2 -= BETA * s1;
		qDot3 -= BETA * s2;
		qDot4 -= BETA * s3;
	}
	// Integrate rate of change of quaternion to yield quaternion
	q0 += qDot1 * Ts;
	q1 += qDot2 * Ts;
	q2 += qDot3 * Ts;
	q3 += qDot4 * Ts;
	// Normalise quaternion
	recipNorm = fast_invsqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	// q0 *= recipNorm;
	// q1 *= recipNorm;
	// q2 *= recipNorm;
	// q3 *= recipNorm;
    arm_mult_f32(&q0, &recipNorm, &q0, 1);
    arm_mult_f32(&q1, &recipNorm, &q1, 1);
    arm_mult_f32(&q2, &recipNorm, &q2, 1);
    arm_mult_f32(&q3, &recipNorm, &q3, 1);
    
	*pitch = -asinf(-2.0f * (q1*q3 - q0*q2)) * RAD_2_DEG;
	*roll = -atan2f(q0*q1 + q2*q3, 0.5f - q1*q1 - q2*q2) * RAD_2_DEG;
	// Yaw = atan2(q1*q2 + q0*q3, 0.5f - q2*q2 - q3*q3)* RAD_2_DEG;
}