#include "App_Si24R1.h"
#include "Int_Si24R1.h"

struct _Rc remote;


uint16_t connect_count = 0; // 数据接收失败的次数
uint8_t unlock_flag = 0;    // 解锁标志位，0锁定，1解锁

/**
 * @brief 解析校验接收到的遥控数据
 * 
 * @param buf 接收到的遥控数据
 * @param len 有效数据长度 
 */
void App_Si24R1_Remote_Check(uint8_t *buf, uint8_t len) {
    connect_flag ++ ;
    if(connect_flag == 1) {   // 通讯成功，，开始解析数据

        /* 帧头、功能校验 */
        if( !( (buf[0] == 0xAA) && (buf[1] == 0xAF) ) ) {   // 不满足直接返回
            return;
        }

        /* 校验和校验 */
        // 取出校验和
        uint32_t rc_sum = (*(buf+len-4) << 24) | 
        (*(buf+len-3) << 16) |
        (*(buf+len-2) << 8) |
        *(buf+len-1);
        // 通过接收数据，自己计算校验和
        uint32_t flight_sum = 0;
        for (uint8_t i = 4; i < len-4; i++)
        {
            /* code */
            flight_sum += buf[i];
        }
        // 两个作比较
        if(flight_sum != rc_sum) {  // 校验和不一致
            return ;
        }

        /* 判断功能字，进行相应处理 */
        if(buf[2]==0x03) {
            // 取出遥控数据
            remote.THR = (buf[4] << 8) | buf[5];
            remote.YAW = (buf[6] << 8) | buf[7];
            remote.ROL = (buf[8] << 8) | buf[9];
            remote.PIT = (buf[10] << 8) | buf[11];
            // 辅助通道不取了，结构体也没定义这个位置，发送的也没放值
        }
    } 
    // else {
    //     connect_count++;
    // }

    if(connect_flag > 1000) {    // 长时间失联，避免越界，不加了
        connect_flag = 310;      // 注意后面失联处理函数的判断时间
    }

    // TODO test...
    // printf("thr=%d\r\n", remote.THR);
    // printf("yaw=%d\r\n", remote.YAW);
    // printf("rol=%d\r\n", remote.ROL);
    // printf("pit=%d\r\n", remote.PIT);

}

/**
 * @brief 接收
 * 
 */
uint8_t rx_buff[28]={0};
void App_Si24R1_Receive(void) {
    Int_NRF24L01_RxPacket(rx_buff);
    App_Si24R1_Remote_Check(rx_buff, 28);
}


/**
 * @brief 根据指令，判断解锁飞机
 * 
 */
void App_Si24R1_RC_Unlock() {

    /* 解锁指令：油门最低 ——》油门最高 ——》油门最低 ——》解锁 */
    static uint8_t status = WAITTING_1; // 默认阶段1
    static uint32_t timeout = 0;         // 延时

    if(status == ENMERGENCY) {          // 紧急情况
        status = EXIT;
    }

    switch(status) {
        case WAITTING_1:                // 阶段1
            printf("level1 test...\r\n");
            if(remote.THR < 1030) {     // 油门最低
                status = WAITTING_2;    // 准备进入阶段2
            }
            break;
        case WAITTING_2:                // 阶段2
            printf("level2 test...\r\n");
            if(remote.THR > 1900) {     // 油门最高
                timeout++;
                if(timeout > 50) {      // 假设该函数10ms调用一次，50*10=500ms
                    status = WAITTING_3;    // 准备进入阶段3
                    timeout = 0;
                }
            }
            break;
        case WAITTING_3:                // 阶段3
            printf("level3 test...\r\n");
            if(remote.THR < 1030) {     // 油门最低
                status = WAITTING_4;    // 准备进入阶段4
            }
            break;
        case WAITTING_4:                // 阶段4，解锁
            printf("unlock...\r\n");
            unlock_flag = 1;
            status = PROCESS;
            break;
        case PROCESS:                   // 正式进入控制
            printf("process...\r\n");
            if(remote.THR < 1030) {     // 油门没动
                /* 解锁后长时间不操作再次锁定 */
                if(timeout++ > 1000) {
                    printf("test_abc...\r\n");
                    /* 说明长时间没操作,开始计时 */
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
 * @brief 遥控指令解析（比如解锁指令），添加失联后的处理逻辑
 * 
 */
void App_Si24R1_RC_Analysis() {
    static uint16_t thr_count = 0;
    if(connect_flag == 1) {         // 连接上了
        // printf("con_flag=%d\r\n", connect_flag);      
        App_Si24R1_RC_Unlock();     // 执行解锁指令
    } else {    // 超过3s判定失联
        // printf("con_flag1=%d\r\n", connect_flag);
        if(connect_flag > 300) {

            // 慢慢降低油门落地
            // 除了油门，全部归中
            remote.PIT = 1500;
            remote.ROL = 1500;
            remote.YAW = 1500;

            // 油门低，直接落地
            if(remote.THR < 1200) {
                remote.THR = 1000;
                /* 尝试重连 */
                printf("restart 2.4G...\r\n");
                while (Int_NRF24L01_Check());
                // Int_NRF24L01_RX_Mode();
                printf("restart 2.4G success...\r\n");
            }
            else {                              // 油门不低，缓慢减少油门值
                if( thr_count ++ > 100 ) {      // 假设10ms执行一次，100次=1s
                    remote.THR -= 50;
                    thr_count = 0;
                }
            }
            LIMIT(remote.THR, 1000, 2000);
        }
    }
}


