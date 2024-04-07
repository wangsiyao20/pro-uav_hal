#include "App_Mpu6050_Quaternion.h"
#include "Int_Mpu6050.h"
#include "Com_Kalman.h"
#include "Com_IMU.h"

/* MPU6050���ھ�ֹ״̬��ƫ��ֵ */
#define MIN_GYRO_QUIET -20
#define MAX_GYRO_QUIET 20


St_Mpu mpu6050 = {0,0,0,0,0,0};     // ����ԭʼ����
St_Angle mpuAngle = {0, 0, 0};      // ��Ԫ��������ŷ����

int16_t MPU_Offset[6] = {0};        // ԭʼ����ƫ����

/**
 * @brief ��ƫ������ֵ�������ھ�̬ʱ
 * 
 */
void App_Mpu6050_Quaternion_SetMPUOffsets() {

    int16_t lastGyro[3] = {0};     // �ϴν��ٶ�
    int16_t errorGyro[3] = {0};  // ������ǰ-�ϴ�

    // 30�����(��ǰ-�ϴ�)������ֹ״̬������ȷ���Ƿ���ƽ����
    for(int8_t i=30; i>0; i--) {
        while(1) {
            // Int_MPU6050_GetAccl(&mpu6050.accX, &mpu6050.accY, &mpu6050.accZ);
            Int_MPU6050_GetGyro(&mpu6050.gyroX, &mpu6050.gyroY, &mpu6050.gyroZ);
            errorGyro[0] = mpu6050.accX - lastGyro[0];
            errorGyro[1] = mpu6050.accY - lastGyro[1];
            errorGyro[2] = mpu6050.accZ - lastGyro[2];
            printf("��%d\t\t%d\t\t%d\r\n", errorGyro[0], errorGyro[1], errorGyro[2]);
            lastGyro[0] = mpu6050.gyroX;
            lastGyro[1] = mpu6050.gyroY;
            lastGyro[2] = mpu6050.gyroZ;
            if( errorGyro[0] > MIN_GYRO_QUIET
                && errorGyro[0]< MAX_GYRO_QUIET
                && errorGyro[1] > MIN_GYRO_QUIET
                && errorGyro[1]< MAX_GYRO_QUIET
                && errorGyro[2] > MIN_GYRO_QUIET
                && errorGyro[2]< MAX_GYRO_QUIET
                ) break;
        }
    }

    // TODO Ϊ��׼ȷ�������Ըĸģ��� ƽ�������Ͳ������Ӽ����ˣ�ֱ�ӰѺ����ֵ��Ϊ��������
    Int_MPU6050_GetAccl(&mpu6050.accX, &mpu6050.accY, &mpu6050.accZ);
    Int_MPU6050_GetGyro(&mpu6050.gyroX, &mpu6050.gyroY, &mpu6050.gyroZ);
    MPU_Offset[0] = mpu6050.accX;
    MPU_Offset[1] = mpu6050.accY;
    MPU_Offset[2] = mpu6050.accZ - 16384;   // Z���ϵļ��ٶ���һ���������������ֵ��������
    MPU_Offset[3] = mpu6050.gyroX;
    MPU_Offset[4] = mpu6050.gyroY;
    MPU_Offset[5] = mpu6050.gyroZ;

}

/**
 * @brief ��ʼ��
 * 
 */
void App_Mpu6050_Quaternion_Init(void) {
    Int_MPU6050_Init();
    printf("У׼�У�����ȴ�...\r\n");
    App_Mpu6050_Quaternion_SetMPUOffsets();
}


/**
 * @brief �����ݻ�ȡ�������������˲�����
 * 
 */
void App_Mpu6050_Quaternion_OriginalData(void) {


    // ��ȡԭʼ����
    Int_MPU6050_GetAccl(&mpu6050.accX, &mpu6050.accY, &mpu6050.accZ);
    Int_MPU6050_GetGyro(&mpu6050.gyroX, &mpu6050.gyroY, &mpu6050.gyroZ);

    // ��ƫУ׼
    mpu6050.accX = mpu6050.accX - MPU_Offset[0];
    mpu6050.accY = mpu6050.accY - MPU_Offset[1];
    mpu6050.accZ = mpu6050.accZ - MPU_Offset[2];
    mpu6050.gyroX = mpu6050.gyroX - MPU_Offset[3];
    mpu6050.gyroY = mpu6050.gyroY - MPU_Offset[4];
    mpu6050.gyroZ = mpu6050.gyroZ - MPU_Offset[5];

    // �˲�
    // �Լ��ٶȽ���һά�������˲�
    Com_Kalman_1(&ekf[0], mpu6050.accX);
    mpu6050.accX = (uint16_t)ekf[0].out;
    Com_Kalman_1(&ekf[1], mpu6050.accY);
    mpu6050.accY = (uint16_t)ekf[1].out;
    Com_Kalman_1(&ekf[2], mpu6050.accZ);
    // �Խ��ٶȽ��м򵥵�һ�׵�ͨ�˲�������һ�ε�ֵ(��ʼΪ0��������)*0.85 + ��ε�ֵ*0.15
    static int16_t lastGyro[3] = {0};
    mpu6050.gyroX = 0.85 * lastGyro[0] + 0.15 * mpu6050.gyroX;
    lastGyro[0] = mpu6050.gyroX;
    mpu6050.gyroY = 0.85 * lastGyro[1] + 0.15 * mpu6050.gyroY;
    lastGyro[1] = mpu6050.gyroY;
    mpu6050.gyroZ = 0.85 * lastGyro[2] + 0.15 * mpu6050.gyroZ;
    lastGyro[2] = mpu6050.gyroZ;

    // TODO  TEST...
    printf("У׼���˲����ֵΪ��\r\n");
    printf("x���ٶȣ�%d\t\ty���ٵģ�%d\t\tz���ٶȣ�%d\r\n", mpu6050.accX, mpu6050.accY, mpu6050.accZ);
    printf("x���ٶȣ�%d\t\ty���ٵģ�%d\t\tz���ٶȣ�%d\r\n", mpu6050.gyroX, mpu6050.gyroY, mpu6050.gyroZ);
}

/**
 * @brief ������Ԫ�������ĽǶ�
 * 
 */
St_Angle App_Mpu6050_Quaternion_GetAngle(void) {
    App_Mpu6050_Quaternion_OriginalData();
    GetAngle(&mpu6050, &mpuAngle, 0.006f);
    return mpuAngle;
}


