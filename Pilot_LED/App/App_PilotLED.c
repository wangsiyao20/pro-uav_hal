#include "App_PilotLED.h"

#include "Int_PilotLED.h"

PilotLed pilotLed = {1000, AlwaysOff};  // Ĭ��1000ms��������

uint16_t led_count = 0; 

/**
 * @brief LED��ʾ��
 * 
 */
void App_PilotLED_Modify(void) {

    static uint16_t lastTime = 0;
    // TODO  ϵͳʱ�ӵļ����Ǵ��ϵ�Ϳ�ʼ�ģ������ﲻ���0��ʼ����ǰ����ʱ���˻�����������취�Ż�
    uint16_t nowTime = HAL_GetTick();

    if(nowTime - lastTime > pilotLed.ledFlashTime) {
        // ʱ�䵽���ָ���Ĭ�ϳ���
        pilotLed.status = AlwaysOn;
        lastTime = nowTime;
    }

    HAL_Delay(100); // ������Ҫ��˸�ģ�����˸ʱ��100ms
    switch (pilotLed.status)
    {
    case AlwaysOn:
        Int_LED_On(LED1_GPIO_Port, LED1_Pin);
        Int_LED_On(LED2_GPIO_Port, LED2_Pin);
        Int_LED_On(LED3_GPIO_Port, LED3_Pin);
        Int_LED_On(LED4_GPIO_Port, LED4_Pin);
        break;
    case AlwaysOff:
        Int_LED_Off(LED1_GPIO_Port, LED1_Pin);
        Int_LED_Off(LED2_GPIO_Port, LED2_Pin);
        Int_LED_Off(LED3_GPIO_Port, LED3_Pin);
        Int_LED_Off(LED4_GPIO_Port, LED4_Pin);
        break;
    case AllFlashLight:
        Int_LED_Toggle(LED1_GPIO_Port, LED1_Pin);
        Int_LED_Toggle(LED2_GPIO_Port, LED2_Pin);
        Int_LED_Toggle(LED3_GPIO_Port, LED3_Pin);
        Int_LED_Toggle(LED4_GPIO_Port, LED4_Pin);
        break;
    case AlternateFlash:
        /* ǰ������˸�������ó�ʼ״̬ */
        Int_LED_On(LED1_GPIO_Port, LED1_Pin);
        Int_LED_On(LED2_GPIO_Port, LED2_Pin);
        Int_LED_Off(LED3_GPIO_Port, LED3_Pin);
        Int_LED_Off(LED4_GPIO_Port, LED4_Pin);
        pilotLed.status = AllFlashLight; // �Գ�ʼ״̬��ʼ��˸
        break;
    case WARNING:
        /* �Խ��� ������˸ */
        Int_LED_On(LED1_GPIO_Port, LED1_Pin);
        Int_LED_On(LED3_GPIO_Port, LED3_Pin);
        Int_LED_Off(LED2_GPIO_Port, LED2_Pin);
        Int_LED_Off(LED4_GPIO_Port, LED4_Pin);
        pilotLed.status = AllFlashLight; // �Գ�ʼ״̬��ʼ��˸
        break;
    case DANGEROURS:
        /* �������룬��˸һ�� */
        led_count++;
        if (led_count < 10)
        {
            Int_LED_On(LED1_GPIO_Port, LED1_Pin);
            Int_LED_On(LED2_GPIO_Port, LED2_Pin);
            Int_LED_On(LED3_GPIO_Port, LED3_Pin);
            Int_LED_On(LED4_GPIO_Port, LED4_Pin);
        }
        else if (led_count < 12)
        {
            Int_LED_Toggle(LED1_GPIO_Port, LED1_Pin);
            Int_LED_Toggle(LED2_GPIO_Port, LED2_Pin);
            Int_LED_Toggle(LED3_GPIO_Port, LED3_Pin);
            Int_LED_Toggle(LED4_GPIO_Port, LED4_Pin);
        }
        else
        {
            led_count = 0;
        }
        break;

    default:
        pilotLed.status = AlwaysOff;
        break;
    }

}





