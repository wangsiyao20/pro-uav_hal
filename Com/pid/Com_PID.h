#ifndef __COM_PID_H__
#define __COM_PID_H__

#include "main.h"

typedef volatile struct
{
    float desired;   // ����
    float prevError; // �ϴ�ƫ��
    float integ;     // �������ۼ�ֵ
    float kp;        // p����
    float ki;        // i����
    float kd;        // d����
    float measured;  // ʵ�ʲ���ֵ
    float out;       // pid�ó��Ľ��
} PidObject;


void ResetPID(PidObject** pidObject, uint8_t len);

void UpdatePID(PidObject *pid, float dt);

void CasecadePID(PidObject *pidAngle, PidObject *pidRate, float dt);

#endif

