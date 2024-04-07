#include "App_Mpu6050_Quaternion.h"
#include "Int_Mpu6050.h"
#include "Com_Kalman.h"
#include "Com_IMU.h"

/* MPU6050处于静止状态的偏差值 */
#define MIN_GYRO_QUIET -20
#define MAX_GYRO_QUIET 20


St_Mpu mpu6050 = {0,0,0,0,0,0};     // 六轴原始数据
St_Angle mpuAngle = {0, 0, 0};      // 四元数解算后的欧拉角

int16_t MPU_Offset[6] = {0};        // 原始数据偏移量

/**
 * @brief 给偏移量赋值，，处于静态时
 * 
 */
void App_Mpu6050_Quaternion_SetMPUOffsets() {

    int16_t lastGyro[3] = {0};     // 上次角速度
    int16_t errorGyro[3] = {0};  // 误差，，当前-上次

    // 30次误差(当前-上次)不过静止状态，，就确定是放置平衡了
    for(int8_t i=30; i>0; i--) {
        while(1) {
            // Int_MPU6050_GetAccl(&mpu6050.accX, &mpu6050.accY, &mpu6050.accZ);
            Int_MPU6050_GetGyro(&mpu6050.gyroX, &mpu6050.gyroY, &mpu6050.gyroZ);
            errorGyro[0] = mpu6050.accX - lastGyro[0];
            errorGyro[1] = mpu6050.accY - lastGyro[1];
            errorGyro[2] = mpu6050.accZ - lastGyro[2];
            printf("误差：%d\t\t%d\t\t%d\r\n", errorGyro[0], errorGyro[1], errorGyro[2]);
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

    // TODO 为了准确，，可以改改，， 平衡后这里就不作复杂计算了，直接把后面的值作为误差就行了
    Int_MPU6050_GetAccl(&mpu6050.accX, &mpu6050.accY, &mpu6050.accZ);
    Int_MPU6050_GetGyro(&mpu6050.gyroX, &mpu6050.gyroY, &mpu6050.gyroZ);
    MPU_Offset[0] = mpu6050.accX;
    MPU_Offset[1] = mpu6050.accY;
    MPU_Offset[2] = mpu6050.accZ - 16384;   // Z轴上的加速度有一个力，，把这个数值保留下来
    MPU_Offset[3] = mpu6050.gyroX;
    MPU_Offset[4] = mpu6050.gyroY;
    MPU_Offset[5] = mpu6050.gyroZ;

}

/**
 * @brief 初始化
 * 
 */
void App_Mpu6050_Quaternion_Init(void) {
    Int_MPU6050_Init();
    printf("校准中，，请等待...\r\n");
    App_Mpu6050_Quaternion_SetMPUOffsets();
}


/**
 * @brief 将数据获取到，，并进行滤波处理
 * 
 */
void App_Mpu6050_Quaternion_OriginalData(void) {


    // 获取原始数据
    Int_MPU6050_GetAccl(&mpu6050.accX, &mpu6050.accY, &mpu6050.accZ);
    Int_MPU6050_GetGyro(&mpu6050.gyroX, &mpu6050.gyroY, &mpu6050.gyroZ);

    // 零偏校准
    mpu6050.accX = mpu6050.accX - MPU_Offset[0];
    mpu6050.accY = mpu6050.accY - MPU_Offset[1];
    mpu6050.accZ = mpu6050.accZ - MPU_Offset[2];
    mpu6050.gyroX = mpu6050.gyroX - MPU_Offset[3];
    mpu6050.gyroY = mpu6050.gyroY - MPU_Offset[4];
    mpu6050.gyroZ = mpu6050.gyroZ - MPU_Offset[5];

    // 滤波
    // 对加速度进行一维卡尔曼滤波
    Com_Kalman_1(&ekf[0], mpu6050.accX);
    mpu6050.accX = (uint16_t)ekf[0].out;
    Com_Kalman_1(&ekf[1], mpu6050.accY);
    mpu6050.accY = (uint16_t)ekf[1].out;
    Com_Kalman_1(&ekf[2], mpu6050.accZ);
    // 对角速度进行简单的一阶低通滤波，，上一次的值(初始为0，往上升)*0.85 + 这次的值*0.15
    static int16_t lastGyro[3] = {0};
    mpu6050.gyroX = 0.85 * lastGyro[0] + 0.15 * mpu6050.gyroX;
    lastGyro[0] = mpu6050.gyroX;
    mpu6050.gyroY = 0.85 * lastGyro[1] + 0.15 * mpu6050.gyroY;
    lastGyro[1] = mpu6050.gyroY;
    mpu6050.gyroZ = 0.85 * lastGyro[2] + 0.15 * mpu6050.gyroZ;
    lastGyro[2] = mpu6050.gyroZ;

    // TODO  TEST...
    printf("校准并滤波后的值为：\r\n");
    printf("x加速度：%d\t\ty加速的：%d\t\tz加速度：%d\r\n", mpu6050.accX, mpu6050.accY, mpu6050.accZ);
    printf("x角速度：%d\t\ty角速的：%d\t\tz角速度：%d\r\n", mpu6050.gyroX, mpu6050.gyroY, mpu6050.gyroZ);
}

/**
 * @brief 返回四元数解算后的角度
 * 
 */
St_Angle App_Mpu6050_Quaternion_GetAngle(void) {
    App_Mpu6050_Quaternion_OriginalData();
    GetAngle(&mpu6050, &mpuAngle, 0.006f);
    return mpuAngle;
}


