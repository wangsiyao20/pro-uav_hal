#include "App_Si24R1.h"
#include "Int_Si24R1.h"

struct _Rc remote;


uint16_t connect_count = 0; // ���ݽ���ʧ�ܵĴ���
uint8_t unlock_flag = 0;    // ������־λ��0������1����

/**
 * @brief ����У����յ���ң������
 * 
 * @param buf ���յ���ң������
 * @param len ��Ч���ݳ��� 
 */
void App_Si24R1_Remote_Check(uint8_t *buf, uint8_t len) {
    connect_flag ++ ;
    if(connect_flag == 1) {   // ͨѶ�ɹ�������ʼ��������

        /* ֡ͷ������У�� */
        if( !( (buf[0] == 0xAA) && (buf[1] == 0xAF) ) ) {   // ������ֱ�ӷ���
            return;
        }

        /* У���У�� */
        // ȡ��У���
        uint32_t rc_sum = (*(buf+len-4) << 24) | 
        (*(buf+len-3) << 16) |
        (*(buf+len-2) << 8) |
        *(buf+len-1);
        // ͨ���������ݣ��Լ�����У���
        uint32_t flight_sum = 0;
        for (uint8_t i = 4; i < len-4; i++)
        {
            /* code */
            flight_sum += buf[i];
        }
        // �������Ƚ�
        if(flight_sum != rc_sum) {  // У��Ͳ�һ��
            return ;
        }

        /* �жϹ����֣�������Ӧ���� */
        if(buf[2]==0x03) {
            // ȡ��ң������
            remote.THR = (buf[4] << 8) | buf[5];
            remote.YAW = (buf[6] << 8) | buf[7];
            remote.ROL = (buf[8] << 8) | buf[9];
            remote.PIT = (buf[10] << 8) | buf[11];
            // ����ͨ����ȡ�ˣ��ṹ��Ҳû�������λ�ã����͵�Ҳû��ֵ
        }
    } 
    // else {
    //     connect_count++;
    // }

    if(connect_flag > 1000) {    // ��ʱ��ʧ��������Խ�磬������
        connect_flag = 310;      // ע�����ʧ�����������ж�ʱ��
    }

    // TODO test...
    // printf("thr=%d\r\n", remote.THR);
    // printf("yaw=%d\r\n", remote.YAW);
    // printf("rol=%d\r\n", remote.ROL);
    // printf("pit=%d\r\n", remote.PIT);

}

/**
 * @brief ����
 * 
 */
uint8_t rx_buff[28]={0};
void App_Si24R1_Receive(void) {
    Int_NRF24L01_RxPacket(rx_buff);
    App_Si24R1_Remote_Check(rx_buff, 28);
}


/**
 * @brief ����ָ��жϽ����ɻ�
 * 
 */
void App_Si24R1_RC_Unlock() {

    /* ����ָ�������� ������������� ������������� ���������� */
    static uint8_t status = WAITTING_1; // Ĭ�Ͻ׶�1
    static uint32_t timeout = 0;         // ��ʱ

    if(status == ENMERGENCY) {          // �������
        status = EXIT;
    }

    switch(status) {
        case WAITTING_1:                // �׶�1
            printf("level1 test...\r\n");
            if(remote.THR < 1030) {     // �������
                status = WAITTING_2;    // ׼������׶�2
            }
            break;
        case WAITTING_2:                // �׶�2
            printf("level2 test...\r\n");
            if(remote.THR > 1900) {     // �������
                timeout++;
                if(timeout > 50) {      // ����ú���10ms����һ�Σ�50*10=500ms
                    status = WAITTING_3;    // ׼������׶�3
                    timeout = 0;
                }
            }
            break;
        case WAITTING_3:                // �׶�3
            printf("level3 test...\r\n");
            if(remote.THR < 1030) {     // �������
                status = WAITTING_4;    // ׼������׶�4
            }
            break;
        case WAITTING_4:                // �׶�4������
            printf("unlock...\r\n");
            unlock_flag = 1;
            status = PROCESS;
            break;
        case PROCESS:                   // ��ʽ�������
            printf("process...\r\n");
            if(remote.THR < 1030) {     // ����û��
                /* ������ʱ�䲻�����ٴ����� */
                if(timeout++ > 1000) {
                    printf("test_abc...\r\n");
                    /* ˵����ʱ��û����,��ʼ��ʱ */
                    unlock_flag = 0;
                    status = EXIT;
                    timeout = 0;
                }
            }
            break;
        case EXIT:
            printf("exit...\r\n");
            unlock_flag = 0;
            timeout = 0;
            status = WAITTING_1;
            break;
        default:
            status = EXIT;
            break;
    }

}

/**
 * @brief ң��ָ��������������ָ������ʧ����Ĵ����߼�
 * 
 */
void App_Si24R1_RC_Analysis() {
    static uint16_t thr_count = 0;
    if(connect_flag == 1) {         // ��������
        // printf("con_flag=%d\r\n", connect_flag);      
        App_Si24R1_RC_Unlock();     // ִ�н���ָ��
    } else {    // ����3s�ж�ʧ��
        // printf("con_flag1=%d\r\n", connect_flag);
        if(connect_flag > 300) {

            // ���������������
            // �������ţ�ȫ������
            remote.PIT = 1500;
            remote.ROL = 1500;
            remote.YAW = 1500;

            // ���ŵͣ�ֱ�����
            if(remote.THR < 1200) {
                remote.THR = 1000;
                /* �������� */
                printf("restart 2.4G...\r\n");
                while (Int_NRF24L01_Check());
                // Int_NRF24L01_RX_Mode();
                printf("restart 2.4G success...\r\n");
            }
            else {                              // ���Ų��ͣ�������������ֵ
                if( thr_count ++ > 100 ) {      // ����10msִ��һ�Σ�100��=1s
                    remote.THR -= 50;
                    thr_count = 0;
                }
            }
            LIMIT(remote.THR, 1000, 2000);
        }
    }
}


