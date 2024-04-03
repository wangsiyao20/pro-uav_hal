#include "App_Mpu6050.h"
#include "Int_Mpu6050.h"
#include "Com_Filter.h"
#include "stdio.h"
#include "math.h"

#define PI 3.1415926


/* 用来接收6轴的原始值 */
short ax = 0, ay = 0, az = 0;
short gx = 0, gy = 0, gz = 0;


extern float angle;     // 卡尔曼滤波后的角度
float Kalman_Angle_Pitch;     // 卡尔曼滤波后的俯仰角
float Kalman_Angle_Roll;     // 卡尔曼滤波后的横滚角
float Kalman_Angle_Yaw;     // 卡尔曼滤波后的偏航角

float Acc_Angle_Pitch = 0;  // 通过加速度计算出的俯仰角
float Acc_Angle_Roll = 0;   // 通过加速度计算出的横滚角
float Acc_Angle_Yaw = 0;    // 通过加速度计算出的偏航角

float GryoX = 0;        // X轴角速度
float GryoY = 0;        // Y轴角速度
float GryoZ = 0;        // Z轴角速度


/**
 * @brief MPU6050初始化
 * 
 */
void App_Mpu6050_Init(void) {
    Int_MPU6050_Init();
}


/**
 * @description: 通过MPU原始数据计算出角度（通过arctan求角度）
 * @return {*}
 */
void App_MPU6050_GetAngle(void) 
{
    /* 1、读取MPU的原始数据 */
    Int_MPU6050_GetAccl(&ax, &ay, &az);
    Int_MPU6050_GetGyro(&gx, &gy, &gz);

    /* 2、通过加速度计算角度 tanA= X轴加速度/Z轴加速度 ==》 反正切 arctan求出角度A */
    /* atan2得到反正切的 弧度， 角度=弧度*180/PI */
    // Acc_Angle_Pitch = (float)(atan2(ax, az) * 180 / PI);
    Acc_Angle_Pitch = (float)(atan2(ax, az) * 180 / PI);    // math.h函数里的atan2
    Acc_Angle_Roll = (float)(atan2(ay, az) * 180 / PI);
    Acc_Angle_Yaw = (float)(atan2(ax, ay) * 180 / PI);

    /* 3、换算Y轴的角速度 == 原始Y/ 16.4 ===》 +- 2000 °/s 量程 */
    GryoX = -gx / 16.4;         // X轴角速度
    GryoY = -gy / 16.4;         // Y轴角速度
    GryoZ = gz / 16.4;          // Z轴角速度

    /* 4、将两个数据，经过卡尔曼滤波融合计算，得到更准确的角度信息 */
    /* 卡尔曼滤波是一种递归运算,, 一些魔法值会被改变,不能两个不同变量计算时重复使用 */
    // Com_Filter_Kalman(Acc_Angle_Pitch, GryoY);
    // Kalman_Angle_Pitch = angle;

    // Com_Filter_Kalman(Acc_Angle_Roll, GryoX);
    // Kalman_Angle_Roll = angle;

    Com_Filter_Kalman(Acc_Angle_Yaw, GryoZ);
    Kalman_Angle_Yaw = angle;

    // TODO test...
    // printf("三个原始加速度为: %d\t\t%d\t\t%d\r\n", ax, ay, az);
    // printf("三个原始角速度为: %d\t\t%d\t\t%d\r\n", gx, gy, gz);
    // printf("卡尔曼前的角度：%f\r\n", Acc_Angle_Pitch);
    printf("俯仰角: %f\t\t横滚角: %f\t\t偏航角: %f\r\n", Kalman_Angle_Pitch, Kalman_Angle_Roll, Kalman_Angle_Yaw);

}

/**
 * @brief 读取ID
 * 
 */
void App_Mpu6050_Test(void) {
    printf("ID=%x\r\n", Int_MPU6050_GetID());
}

