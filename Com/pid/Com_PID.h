#ifndef __COM_PID_H__
#define __COM_PID_H__

#include "main.h"

typedef volatile struct
{
    float desired;   // 期望
    float prevError; // 上次偏差
    float integ;     // 误差积分累加值
    float kp;        // p参数
    float ki;        // i参数
    float kd;        // d参数
    float measured;  // 实际测量值
    float out;       // pid得出的结果
} PidObject;


void ResetPID(PidObject** pidObject, uint8_t len);

void UpdatePID(PidObject *pid, float dt);

void CasecadePID(PidObject *pidAngle, PidObject *pidRate, float dt);

#endif

