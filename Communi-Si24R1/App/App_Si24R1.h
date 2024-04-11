#ifndef __APP_SI24R1_H__
#define __APP_SI24R1_H__

#include "stm32f1xx_hal.h"
#include "main.h"
#include "stdio.h"
#include "Com_PID.h"

/* �����Ĳ�ͬ״̬ */
#define WAITTING_1 1        // ����׶��ж������Ƿ���ͣ�����ǣ���ɵڶ��׶�
#define WAITTING_2 2        // ����׶��ж������Ƿ���ߣ�����ǣ���ɵ����׶�
#define WAITTING_3 3        // ����׶��ж������Ƿ���ͣ�����ǣ���ɵ��Ľ׶�
#define WAITTING_4 4        // ����׶α�ʾ�����׶�
#define PROCESS    5        // ��ʼ

#define ENMERGENCY 0        // ����״̬
#define EXIT 20             // �˳�

/* �޷� */
#define LIMIT(x, y, z) x=x<y?y:(x>z?z:x)

/* ŷ���ǣ����Ƕ� */
extern PidObject pidPitch;
extern PidObject pidRoll;
extern PidObject pidYaw;
/* ������Ľ��ٶ� */
extern PidObject pidRateX;
extern PidObject pidRateY;
extern PidObject pidRateZ;

extern uint8_t status;  // ���ڽ��е�״̬����������ĺ궨��

struct _Rc
{
    int16_t THR;    // ���ţ���ҡ������
    int16_t YAW;    // ƫ������ҡ������
    int16_t ROL;    // �������ҡ������
    int16_t PIT;    // ��������ҡ������
};

extern struct _Rc remote;

void App_Si24R1_Receive(void);

// void App_Si24R1_RC_Unlock(void);

void App_Si24R1_RC_Analysis(void);

void App_Si24R1_PID_Control(float dt);

#endif



