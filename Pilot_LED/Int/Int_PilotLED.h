#ifndef __INT_PILOTLED_H__
#define __INT_PILOTLED_H__

#include "main.h"

void Int_LED_On(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

void Int_LED_Off(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

void Int_LED_Toggle(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);


#endif


