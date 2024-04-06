#ifndef __APP_PILOTLED_H__
#define __APP_PILOTLED_H__

#include "main.h"


/* LED��ʾ�Ƶ�ǰ״̬ */
typedef enum {
    AlwaysOn = 0,     //��������Ĭ��״̬
    AlwaysOff,    //����
    AllFlashLight,//���ж���˸
    AlternateFlash,//������˸
    WARNING,       //�澯:
    DANGEROURS,    //Σ�� 
    GET_OFFSET
} Enum_Led_Tip ;

/* LED */
typedef struct {
    uint16_t ledFlashTime;      // LED��ʾ����˸ʱ������ms
    Enum_Led_Tip status;        // LED��ʾ�Ƶ�ǰ״̬
} PilotLed ;

extern PilotLed pilotLed;

void App_PilotLED_Modify(void);

#endif


