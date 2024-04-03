#include "App_Battery.h"
#include "adc.h"

uint16_t adcValue_Buff[5] = {0};

/**
 * @brief ������ѹ�ɼ�
 * 
 */
void App_Battery_Start(void) {
    // ����ADC
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcValue_Buff, 5);
}


/**
 * @brief ��ص�ѹ���
 * 
 */
float App_Battery_VolCheck(void) {
    static float bat_val = 4000;    // ��4000 ��ʼ�����֣���ƽ���ؽӽ���ȷ��ѹ
    bat_val += 0.2 * ((adcValue_Buff[0] * 3300 / 4095 * 2) - bat_val);
    // һ�׵�ͨ�˲����� Ҳ��������
    // bat_val = 0.2 * (adcValue_Buff[0] * 3300 / 4095 * 2) + 0.8 * bat_val;
    return bat_val / 1000;
}

