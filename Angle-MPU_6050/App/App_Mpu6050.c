#include "App_Mpu6050.h"
#include "Int_Mpu6050.h"
#include "Com_Filter.h"
#include "stdio.h"
#include "math.h"

#define PI 3.1415926


/* ��������6���ԭʼֵ */
short ax = 0, ay = 0, az = 0;
short gx = 0, gy = 0, gz = 0;


extern float angle;     // �������˲���ĽǶ�
float Kalman_Angle_Pitch;     // �������˲���ĸ�����
float Kalman_Angle_Roll;     // �������˲���ĺ����
float Kalman_Angle_Yaw;     // �������˲����ƫ����

float Acc_Angle_Pitch = 0;  // ͨ�����ٶȼ�����ĸ�����
float Acc_Angle_Roll = 0;   // ͨ�����ٶȼ�����ĺ����
float Acc_Angle_Yaw = 0;    // ͨ�����ٶȼ������ƫ����

float GryoX = 0;        // X����ٶ�
float GryoY = 0;        // Y����ٶ�
float GryoZ = 0;        // Z����ٶ�


/**
 * @brief MPU6050��ʼ��
 * 
 */
void App_Mpu6050_Init(void) {
    Int_MPU6050_Init();
}


/**
 * @description: ͨ��MPUԭʼ���ݼ�����Ƕȣ�ͨ��arctan��Ƕȣ�
 * @return {*}
 */
void App_MPU6050_GetAngle(void) 
{
    /* 1����ȡMPU��ԭʼ���� */
    Int_MPU6050_GetAccl(&ax, &ay, &az);
    Int_MPU6050_GetGyro(&gx, &gy, &gz);

    /* 2��ͨ�����ٶȼ���Ƕ� tanA= X����ٶ�/Z����ٶ� ==�� ������ arctan����Ƕ�A */
    /* atan2�õ������е� ���ȣ� �Ƕ�=����*180/PI */
    // Acc_Angle_Pitch = (float)(atan2(ax, az) * 180 / PI);
    Acc_Angle_Pitch = (float)(atan2(ax, az) * 180 / PI);    // math.h�������atan2
    Acc_Angle_Roll = (float)(atan2(ay, az) * 180 / PI);
    Acc_Angle_Yaw = (float)(atan2(ax, ay) * 180 / PI);

    /* 3������Y��Ľ��ٶ� == ԭʼY/ 16.4 ===�� +- 2000 ��/s ���� */
    GryoX = -gx / 16.4;         // X����ٶ�
    GryoY = -gy / 16.4;         // Y����ٶ�
    GryoZ = gz / 16.4;          // Z����ٶ�

    /* 4�����������ݣ������������˲��ںϼ��㣬�õ���׼ȷ�ĽǶ���Ϣ */
    /* �������˲���һ�ֵݹ�����,, һЩħ��ֵ�ᱻ�ı�,����������ͬ��������ʱ�ظ�ʹ�� */
    // Com_Filter_Kalman(Acc_Angle_Pitch, GryoY);
    // Kalman_Angle_Pitch = angle;

    // Com_Filter_Kalman(Acc_Angle_Roll, GryoX);
    // Kalman_Angle_Roll = angle;

    Com_Filter_Kalman(Acc_Angle_Yaw, GryoZ);
    Kalman_Angle_Yaw = angle;

    // TODO test...
    // printf("����ԭʼ���ٶ�Ϊ: %d\t\t%d\t\t%d\r\n", ax, ay, az);
    // printf("����ԭʼ���ٶ�Ϊ: %d\t\t%d\t\t%d\r\n", gx, gy, gz);
    // printf("������ǰ�ĽǶȣ�%f\r\n", Acc_Angle_Pitch);
    printf("������: %f\t\t�����: %f\t\tƫ����: %f\r\n", Kalman_Angle_Pitch, Kalman_Angle_Roll, Kalman_Angle_Yaw);

}

/**
 * @brief ��ȡID
 * 
 */
void App_Mpu6050_Test(void) {
    printf("ID=%x\r\n", Int_MPU6050_GetID());
}

