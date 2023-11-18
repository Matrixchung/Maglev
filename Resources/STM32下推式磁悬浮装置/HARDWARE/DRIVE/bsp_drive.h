#ifndef __BSP_DRIVE_H
#define __BSP_DRIVE_H


#include "sys.h"

#define XIN1   PBout(12)
#define XIN2   PBout(13)

#define YIN1   PBout(14)
#define YIN2   PBout(15)



void motor_init(void);

#endif /*__BSP_DRIVE_H*/

