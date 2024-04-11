#include "App_PWM.h"
#include "Int_PWM.h"
#include "App_Si24R1.h"


/* 4个电机的pwm */
int16_t motor1 = 0; // 右后
int16_t motor2 = 0; // 右前
int16_t motor3 = 0; // 左前
int16_t motor4 = 0; // 左后 


/**
 * @brief 初始化
 * 
 */
void App_PWM_Init(void) {
    Int_PWM_Init();
}

/**
 * @brief 设置pwm方波
 * 
 * @param pwa 
 * @param pwb 
 * @param pwc 
 * @param pwd 
 */
void App_PWM_SetPWM(int pwa, int pwb, int pwc, int pwd) {
    Int_PWM_SetPWM_Left0(pwa);      
    Int_PWM_SetPWM_Left1(pwb);      
    Int_PWM_SetPWM_Right0(pwc);    
    Int_PWM_SetPWM_Right1(pwd);     
}

/**
 * @brief 电机控制
 * 
 */
void App_PWM_Moter_Control() {
    // 先给四个电机全部赋0
    motor1 = motor2 = motor3 = motor4 = 0;
    if(status == PROCESS) {
        motor1 = motor2 = motor3 = motor4 = remote.THR-1000;    // remote.THR的范围是[1000, 2000]
        if (remote.THR < 1050)
        {
            motor1 = motor2 = motor3 = motor4 = 0;
        }
        /* 3、 预留100给PID控制 */
        LIMIT(motor1, 0, 900);
        LIMIT(motor2, 0, 900);
        LIMIT(motor3, 0, 900);
        LIMIT(motor4, 0, 900);
        /* 4、加上3个PID的值 */
        motor1 += +pidRateX.out + pidRateY.out + pidRateZ.out; // 右后
        motor2 += +pidRateX.out - pidRateY.out - pidRateZ.out; // 右前
        motor3 += -pidRateX.out - pidRateY.out + pidRateZ.out; // 左前
        motor4 += -pidRateX.out + pidRateY.out - pidRateZ.out; // 左后
        /* 5、加完pid的值限制为0-1000 */
        LIMIT(motor1, 0, 1000);
        LIMIT(motor2, 0, 1000);
        LIMIT(motor3, 0, 1000);
        LIMIT(motor4, 0, 1000);
    }
    App_PWM_SetPWM(motor3, motor4, motor2, motor1);
}


