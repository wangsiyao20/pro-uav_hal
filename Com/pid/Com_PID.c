#include "Com_PID.h"


/**
 * @brief ����PID����
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
 * @brief ����PID
 * 
 * @param pid 
 * @param dt 
 */
void UpdatePID(PidObject * pid, float dt) {

    // ����ƫ��ֵ
    float temp_err = pid -> desired - pid->measured;    // ����ֵ-ʵ��ֵ
    // ������֣�ƫ���ۻ���
    pid->integ += temp_err * dt;
    // ����΢�֣�ƫ��ı仯��
    float dri = (temp_err - pid->prevError) / dt;
    // ������浽out�������ƫ��ֵ���浽���ϴ�ƫ��ֶ�
    pid->out = pid->kp * temp_err + pid->ki * pid->integ + pid->kd * dri;
    pid->prevError = temp_err;
}


/**
 * @brief ����PID���㣬�� �ڻ������Ϊ������
 * 
 * @param pidAngle �Ƕȣ��� ���⻷
 * @param pidRate ���ٶȣ��� ���ڻ�
 * @param dt ��С�ı仯��ʱ�䣬���仯����
 */
void CasecadePID(PidObject * pidAngle, PidObject * pidRate, float dt) {
    /* 1���Ƕ��⻷����PID���� */
    UpdatePID(pidAngle,dt);
    /* 2���⻷���������ֵ���ڻ�������ֵ */
    pidRate -> desired = pidAngle ->out;
    /* 3�����ڻ�����PID���� */
    UpdatePID(pidRate,dt);
}

