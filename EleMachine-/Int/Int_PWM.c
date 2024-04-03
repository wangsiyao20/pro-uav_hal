#include "Int_PWM.h"
#include "tim.h"

/**
 * @brief ≥ı ºªØ
 * 
 */
void Int_PWM_Init(void) {
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
}

void Int_PWM_SetPWM_Left0(int pwa) {
    __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, pwa);
}
void Int_PWM_SetPWM_Left1(int pwb) {
    __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, pwb);
}
void Int_PWM_SetPWM_Right0(int pwc) {
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, pwc);
}
void Int_PWM_SetPWM_Right1(int pwd) {
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, pwd);
}


