#ifndef __APP_MPU6050_QUATERNION_H__
#define __APP_MPU6050_QUATERNION_H__


#include "main.h"

/*
    原始数据
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
    四元数解算后的欧拉角
*/
typedef struct {
    float pitch;    // 俯仰角
    float roll;     // 横滚角
    float yaw;      // 偏航角
} St_Angle;

extern St_Mpu mpu6050;
extern St_Angle mpuAngle;

void App_Mpu6050_Quaternion_Init(void);

St_Angle App_Mpu6050_Quaternion_GetAngle(void);

#endif


