#ifndef __INT_PWM_H__
#define __INT_PWM_H__

#include "main.h"

void Int_PWM_Init(void);

void Int_PWM_SetPWM_Left0(int pwa);

void Int_PWM_SetPWM_Left1(int pwb);

void Int_PWM_SetPWM_Right0(int pwc);

void Int_PWM_SetPWM_Right1(int pwd);

#endif


