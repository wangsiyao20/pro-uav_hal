#include "App_PilotLED.h"

#include "Int_PilotLED.h"

PilotLed pilotLed = {1000, AlwaysOff};  // 默认1000ms，，常亮

uint16_t led_count = 0; 

/**
 * @brief LED提示灯
 * 
 */
void App_PilotLED_Modify(void) {

    static uint16_t lastTime = 0;
    // TODO  系统时钟的计数是从上电就开始的，，这里不会从0开始，，前面延时多了会有误差，，可想办法优化
    uint16_t nowTime = HAL_GetTick();

    if(nowTime - lastTime > pilotLed.ledFlashTime) {
        // 时间到，恢复成默认常亮
        pilotLed.status = AlwaysOn;
        lastTime = nowTime;
    }

    HAL_Delay(100); // 对于需要闪烁的，，闪烁时长100ms
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
        /* 前后交替闪烁，先设置初始状态 */
        Int_LED_On(LED1_GPIO_Port, LED1_Pin);
        Int_LED_On(LED2_GPIO_Port, LED2_Pin);
        Int_LED_Off(LED3_GPIO_Port, LED3_Pin);
        Int_LED_Off(LED4_GPIO_Port, LED4_Pin);
        pilotLed.status = AllFlashLight; // 以初始状态开始闪烁
        break;
    case WARNING:
        /* 对角线 交替闪烁 */
        Int_LED_On(LED1_GPIO_Port, LED1_Pin);
        Int_LED_On(LED3_GPIO_Port, LED3_Pin);
        Int_LED_Off(LED2_GPIO_Port, LED2_Pin);
        Int_LED_Off(LED4_GPIO_Port, LED4_Pin);
        pilotLed.status = AllFlashLight; // 以初始状态开始闪烁
        break;
    case DANGEROURS:
        /* 长亮两秒，闪烁一会 */
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





