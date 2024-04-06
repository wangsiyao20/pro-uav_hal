#ifndef __APP_PILOTLED_H__
#define __APP_PILOTLED_H__

#include "main.h"


/* LED提示灯当前状态 */
typedef enum {
    AlwaysOn = 0,     //常亮，，默认状态
    AlwaysOff,    //常暗
    AllFlashLight,//所有都闪烁
    AlternateFlash,//交替闪烁
    WARNING,       //告警:
    DANGEROURS,    //危险 
    GET_OFFSET
} Enum_Led_Tip ;

/* LED */
typedef struct {
    uint16_t ledFlashTime;      // LED提示灯闪烁时长，，ms
    Enum_Led_Tip status;        // LED提示灯当前状态
} PilotLed ;

extern PilotLed pilotLed;

void App_PilotLED_Modify(void);

#endif


