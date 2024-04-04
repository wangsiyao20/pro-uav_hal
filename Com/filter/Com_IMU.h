#ifndef __IMU_H
#define __IMU_H

#include "main.h"
#include "App_Mpu6050_Quaternion.h"

#define DEG2RAD 0.017453293f // ��ת���Ȧ�/180
#define RAD2DEG 57.29578f    // ����ת��180/��
#define squa(Sq) (((float)Sq) * ((float)Sq))

typedef struct
{
    float yaw;
    float pitch;
    float roll;
    float yaw_mag; // �����ɴ����Ƶĳ����ĽǶ�
    float Cos_Roll;
    float Sin_Roll;
    float Cos_Pitch;
    float Sin_Pitch;
    float Cos_Yaw;
    float Sin_Yaw;
} _st_IMU;

typedef struct V
{
    float x;
    float y;
    float z;
} MPUDA;

extern float yaw_control;
extern float Yaw_Correct;
extern _st_IMU IMU;
extern float GetAccz(void);
extern void GetAngle(const St_Mpu *pMpu, St_Angle *pAngE, float dt);
extern MPUDA Gravity, Acc, Gyro, AccGravity;

#endif
