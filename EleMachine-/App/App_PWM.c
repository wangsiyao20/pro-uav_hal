#include "App_PWM.h"
#include "Int_PWM.h"
#include "App_Si24R1.h"


/* 4�������pwm */
int16_t motor1 = 0; // �Һ�
int16_t motor2 = 0; // ��ǰ
int16_t motor3 = 0; // ��ǰ
int16_t motor4 = 0; // ��� 


/**
 * @brief ��ʼ��
 * 
 */
void App_PWM_Init(void) {
    Int_PWM_Init();
}

/**
 * @brief ����pwm����
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
 * @brief �������
 * 
 */
void App_PWM_Moter_Control() {
    // �ȸ��ĸ����ȫ����0
    motor1 = motor2 = motor3 = motor4 = 0;
    if(status == PROCESS) {
        motor1 = motor2 = motor3 = motor4 = remote.THR-1000;    // remote.THR�ķ�Χ��[1000, 2000]
        if (remote.THR < 1050)
        {
            motor1 = motor2 = motor3 = motor4 = 0;
        }
        /* 3�� Ԥ��100��PID���� */
        LIMIT(motor1, 0, 900);
        LIMIT(motor2, 0, 900);
        LIMIT(motor3, 0, 900);
        LIMIT(motor4, 0, 900);
        /* 4������3��PID��ֵ */
        motor1 += +pidRateX.out + pidRateY.out + pidRateZ.out; // �Һ�
        motor2 += +pidRateX.out - pidRateY.out - pidRateZ.out; // ��ǰ
        motor3 += -pidRateX.out - pidRateY.out + pidRateZ.out; // ��ǰ
        motor4 += -pidRateX.out + pidRateY.out - pidRateZ.out; // ���
        /* 5������pid��ֵ����Ϊ0-1000 */
        LIMIT(motor1, 0, 1000);
        LIMIT(motor2, 0, 1000);
        LIMIT(motor3, 0, 1000);
        LIMIT(motor4, 0, 1000);
    }
    App_PWM_SetPWM(motor3, motor4, motor2, motor1);
}


