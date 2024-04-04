#ifndef __APP_MPU6050_QUATERNION_H__
#define __APP_MPU6050_QUATERNION_H__


#include "main.h"

/*
    ԭʼ����
*/
typedef struct {
    int16_t accX;
    int16_t accY;
    int16_t accZ;
    int16_t gyroX;
    int16_t gyroY;
    int16_t gyroZ;
} St_Mpu;

/*
    ��Ԫ��������ŷ����
*/
typedef struct {
    float pitch;    // ������
    float roll;     // �����
    float yaw;      // ƫ����
} St_Angle;

extern St_Mpu mpu6050;
extern St_Angle mpuAngle;

void App_Mpu6050_Quaternion_Init(void);

St_Angle App_Mpu6050_Quaternion_GetAngle(void);

#endif


