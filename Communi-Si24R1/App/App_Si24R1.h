#ifndef __APP_SI24R1_H__
#define __APP_SI24R1_H__


#include "main.h"
#include "stdio.h"

/* 解锁的不同状态 */
#define WAITTING_1 1        // 这个阶段判断油门是否最低，如果是，变成第二阶段
#define WAITTING_2 2        // 这个阶段判断油门是否最高，如果是，变成第三阶段
#define WAITTING_3 3        // 这个阶段判断油门是否最低，如果是，变成第四阶段
#define WAITTING_4 4        // 这个阶段表示解锁阶段
#define PROCESS    5        // 开始

#define ENMERGENCY 0        // 紧急状态
#define EXIT 20             // 退出

/* 限幅 */
#define LIMIT(x, y, z) x<y?y:(x>z?z:x)


struct _Rc
{
    int16_t THR;    // 油门：左摇杆上下
    int16_t YAW;    // 偏航：左摇杆左右
    int16_t ROL;    // 横滚：右摇杆左右
    int16_t PIT;    // 俯仰：右摇杆上下
};

extern struct _Rc remote;

void App_Si24R1_Receive(void);

void App_Si24R1_RC_Unlock(void);

void App_Si24R1_RC_Analysis(void);

#endif



