#include "Int_PilotLED.h"


void Int_LED_On(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_RESET);
}

void Int_LED_Off(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_SET);
}

void Int_LED_Toggle(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    HAL_GPIO_TogglePin(GPIOx,GPIO_Pin);
}

