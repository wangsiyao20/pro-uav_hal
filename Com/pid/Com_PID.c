#include "Com_PID.h"


/**
 * @brief 重置PID数据
 * 
 * @param pidObject 
 * @param len 
 */
void ResetPID(PidObject** pidObject, uint8_t len) {
    for (uint8_t i = 0; i < len; i++)
    {
        pidObject[i] -> desired = 0;
        pidObject[i] -> prevError = 0;
        pidObject[i] -> out = 0;
        pidObject[i] -> integ = 0;
    }
    
}

/**
 * @brief 更新PID
 * 
 * @param pid 
 * @param dt 
 */
void UpdatePID(PidObject * pid, float dt) {

    // 计算偏差值
    float temp_err = pid -> desired - pid->measured;    // 期望值-实际值
    // 计算积分：偏差累积和
    pid->integ += temp_err * dt;
    // 计算微分：偏差的变化率
    float dri = (temp_err - pid->prevError) / dt;
    // 结果保存到out里，，本次偏差值保存到“上次偏差”字段
    pid->out = pid->kp * temp_err + pid->ki * pid->integ + pid->kd * dri;
    pid->prevError = temp_err;
}


/**
 * @brief 串行PID计算，， 内环输出作为最后输出
 * 
 * @param pidAngle 角度，， 作外环
 * @param pidRate 角速度，， 作内环
 * @param dt 很小的变化的时间，，变化率用
 */
void CasecadePID(PidObject * pidAngle, PidObject * pidRate, float dt) {
    /* 1、角度外环进行PID处理 */
    UpdatePID(pidAngle,dt);
    /* 2、外环的输出，赋值给内环的期望值 */
    pidRate -> desired = pidAngle ->out;
    /* 3、对内环进行PID计算 */
    UpdatePID(pidRate,dt);
}

