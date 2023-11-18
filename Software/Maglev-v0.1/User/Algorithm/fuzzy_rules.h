#ifndef _FUZZY_RULES_H
#define _FUZZY_RULES_H

#define FUZZY_SEG_NUMS 7
#define NB 0
#define NM 1
#define NS 2
#define ZO 3
#define PS 4
#define PM 5
#define PB 6

#include "stdint.h"

const static uint8_t Kp_FUZZY_RULE[FUZZY_SEG_NUMS][FUZZY_SEG_NUMS] = 
{
    {PB,PB,PB,PM,PM,ZO,ZO},
    {PB,PB,PB,PM,PS,ZO,ZO},
    {PB,PM,PM,PS,ZO,NS,NM},
    {PM,PM,PS,ZO,NS,NM,NM},
    {PS,PS,ZO,NS,NM,NM,NM},
    {ZO,ZO,NS,NM,NM,NB,NB},
    {ZO,ZO,NS,NM,NB,NB,NB}
};
const static uint8_t Ki_FUZZY_RULE[FUZZY_SEG_NUMS][FUZZY_SEG_NUMS] = 
{
    {NB,NB,NM,NM,NS,ZO,ZO},
    {NB,NM,NM,NS,NS,ZO,ZO},
    {NB,NM,NS,NS,ZO,PS,PS},
    {NM,NM,NS,ZO,PS,PS,PM},
    {NM,NS,ZO,PS,PS,PM,PB},
    {ZO,ZO,PS,PM,PM,PB,PB},
    {ZO,ZO,PS,PM,PM,PB,PB}
};
const static uint8_t Kd_FUZZY_RULE[FUZZY_SEG_NUMS][FUZZY_SEG_NUMS] = 
{
    {PS,NS,NS,NB,NB,NM,PS},
    {PS,NS,NB,NM,NM,NS,ZO},
    {ZO,NS,NS,NM,NM,NS,ZO},
    {ZO,NS,NM,NS,NS,NS,ZO},
    {ZO,ZO,ZO,ZO,ZO,ZO,ZO},
    {PB,NS,PS,PS,PS,PS,PB},
    {PB,PB,PM,PM,PS,PS,PB}
};
#endif