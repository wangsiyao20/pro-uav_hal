#include "App_Battery.h"
#include "adc.h"

uint16_t adcValue_Buff[5] = {0};

/**
 * @brief 开启电压采集
 * 
 */
void App_Battery_Start(void) {
    // 启动ADC
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcValue_Buff, 5);
}


/**
 * @brief 电池电压检测
 * 
 */
float App_Battery_VolCheck(void) {
    static float bat_val = 4000;    // 从4000 开始作积分，，平滑地接近正确电压
    bat_val += 0.2 * ((adcValue_Buff[0] * 3300 / 4095 * 2) - bat_val);
    // 一阶低通滤波，， 也可以如下
    // bat_val = 0.2 * (adcValue_Buff[0] * 3300 / 4095 * 2) + 0.8 * bat_val;
    return bat_val / 1000;
}

