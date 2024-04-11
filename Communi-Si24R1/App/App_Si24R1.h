#ifndef __APP_SI24R1_H__
#define __APP_SI24R1_H__

#include "stm32f1xx_hal.h"
#include "main.h"
#include "stdio.h"
#include "Com_PID.h"

/* 解锁的不同状态 */
#define WAITTING_1 1        // 这个阶段判断油门是否最低，如果是，变成第二阶段
#define WAITTING_2 2        // 这个阶段判断油门是否最高，如果是，变成第三阶段
#define WAITTING_3 3        // 这个阶段判断油门是否最低，如果是，变成第四阶段
#define WAITTING_4 4        // 这个阶段表示解锁阶段
#define PROCESS    5        // 开始

#define ENMERGENCY 0        // 紧急状态
#define EXIT 20             // 退出

/* 限幅 */
#define LIMIT(x, y, z) x=x<y?y:(x>z?z:x)

/* 欧拉角，，角度 */
extern PidObject pidPitch;
extern PidObject pidRoll;
extern PidObject pidYaw;
/* 三个轴的角速度 */
extern PidObject pidRateX;
extern PidObject pidRateY;
extern PidObject pidRateZ;

extern uint8_t status;  // 正在进行的状态，，最上面的宏定义

struct _Rc
{
    int16_t THR;    // 油门：左摇杆上下
    int16_t YAW;    // 偏航：左摇杆左右
    int16_t ROL;    // 横滚：右摇杆左右
    int16_t PIT;    // 俯仰：右摇杆上下
};

extern struct _Rc remote;

void App_Si24R1_Receive(void);

// void App_Si24R1_RC_Unlock(void);

void App_Si24R1_RC_Analysis(void);

void App_Si24R1_PID_Control(float dt);

#endif



