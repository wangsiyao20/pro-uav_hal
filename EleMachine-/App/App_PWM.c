#include "App_PWM.h"
#include "Int_PWM.h"


/**
 * @brief 初始化
 * 
 */
void App_PWM_Init(void) {
    Int_PWM_Init();
}

/**
 * @brief 设置pwm方波
 * 
 * @param pwa 
 * @param pwb 
 * @param pwc 
 * @param pwd 
 */
void App_PWM_SetPWM(int pwa, int pwb, int pwc, int pwd) {
    Int_PWM_SetPWM_Left0(pwa);
    Int_PWM_SetPWM_Left1(pwb);
    Int_PWM_SetPWM_Right0(pwc);
    Int_PWM_SetPWM_Right1(pwd);
}


