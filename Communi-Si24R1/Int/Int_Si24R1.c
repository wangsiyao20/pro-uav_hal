#include "Int_Si24R1.h"
#include "stdio.h"

/* 这个芯片和NRF24L01芯片用法一样 */

uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x0A, 0x03, 0x06, 0x0E, 0x01}; // 定义一个静态发送地址
uint8_t RX_ADDRESS[RX_ADR_WIDTH] = {0x0A, 0x03, 0x06, 0x0E, 0x01}; // 定义一个静态发送地址

uint16_t connect_flag = 1;       // 0连接成功，1连接失败

/**
 * @brief 写一个字节
 * 
 * @param reg 
 * @param data 
 */
void Int_NRF24L01_Write_Reg(uint8_t reg, uint8_t data) {

    // 片选选中
    Clr_NRF24L01_CSN;

    // 写寄存器地址
    uint8_t rxData = 0;
    HAL_SPI_TransmitReceive(&hspi1,  &reg, &rxData, 1, 100);
    // 写入数据
    HAL_SPI_TransmitReceive(&hspi1, &data, &rxData, 1, 100);

    // 片选取消
    Set_NRF24L01_CSN;

}

/**
 * @brief 读取一个字节数据
 * 
 * @param reg 
 * @return uint8_t 
 */
uint8_t Int_NRF24L01_Read_Reg(uint8_t reg) {

    // 片选选中
    Clr_NRF24L01_CSN;

    // 写寄存器地址
    uint8_t rxData = 0;
    HAL_SPI_TransmitReceive(&hspi1, &reg, &rxData, 1, 100);

    // 读取数据(写入什么无所谓)
    HAL_SPI_TransmitReceive(&hspi1, &reg, &rxData, 1, 100);

    // 片选取消
    Set_NRF24L01_CSN;

    return rxData;

}

/**
 * @brief 向指定寄存器写多个字节
 * 
 * @param reg 寄存器地址
 * @param pBuf 写入的字节的指针
 * @param len 字节数
 */
void Int_NRF24L01_Write_Buf(uint8_t reg, uint8_t* pBuf, uint8_t len) {

    // 片选选中
    Clr_NRF24L01_CSN;

    // 写寄存器地址
    uint8_t rxData = 0;
    HAL_SPI_TransmitReceive(&hspi1, &reg, &rxData, 1, 100);

    // 写入多个字节
    HAL_SPI_TransmitReceive(&hspi1, pBuf, &rxData, len, 100);

    // 片选取消
    Set_NRF24L01_CSN;

}

/**
 * @brief 读多个字节
 * 
 * @param reg 寄存器地址
 * @param pBuf 要读入到的地方
 * @param len 要接收的数据长度
 */
void Int_NRF24L01_Read_Buf(uint8_t reg, uint8_t* pBuf, uint8_t len) {

    // 片选选中
    Clr_NRF24L01_CSN;

    // 写寄存器地址
    uint8_t rxData = 0;
    HAL_SPI_TransmitReceive(&hspi1, &reg, &rxData, 1, 100);

    // 读取多个数据
    HAL_SPI_TransmitReceive(&hspi1, &rxData, pBuf, len, 100);

    // 片选取消
    Set_NRF24L01_CSN;

}


/**
 * @brief 发送模式初始化
 * 
 */
void Int_NRF24L01_TX_Mode(void) {

    Clr_NRF24L01_CE;    // CE=0，，进入待机模式

    /* 2、相关的配置：发送地址、接收管道0地址（一样）、ACK使能、使能管道0、功率、Config为发送模式 */
    Int_NRF24L01_Write_Buf(SPI_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // 写入发送地址
    Int_NRF24L01_Write_Buf(SPI_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // 为了应答接收设备，接收通道0地址和发送地址相同

    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + EN_AA, 0x01);      // 使能接收通道0自动应答
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + EN_RXADDR, 0x01);  // 使能接收通道0
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + SETUP_RETR, 0x0a); // 自动重发延时等待250us+86us，自动重发10次
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + RF_CH, 40);        // 选择射频通道0x40
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + RF_SETUP, 0x0f);   // 数据传输率2Mbps，发射功率7dBm
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + CONFIG, 0x0e);     // CRC使能，16位CRC校验，上电

    Set_NRF24L01_CE;    // 使能CE，，发送模式
}

/**
 * @brief 接收模式
 * 
 * @return uint8_t 
 */
void Int_NRF24L01_RX_Mode(void)
{
    Clr_NRF24L01_CE;
    /* 
        与发送的区别：
            1、不需要设置发送地址
            2、需要设置接收通道0的负载长度
            3、config配置的，bit0=1 为接收模式
     */
    Int_NRF24L01_Write_Buf(SPI_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // 接收设备接收通道0使用和发送设备相同的发送地址
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + EN_AA, 0x01);                          // 使能接收通道0自动应答
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + EN_RXADDR, 0x01);                      // 使能接收通道0
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + RF_CH, 40);                            // 选择射频通道0x40
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);             // 接收通道0选择和发送通道相同有效数据宽度
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + RF_SETUP, 0x0f);                       // 数据传输率2Mbps，发射功率7dBm
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + CONFIG, 0x0f);                         // CRC使能，16位CRC校验，上电，接收模式
    
    Set_NRF24L01_CE;                                              // 拉高CE启动接收设备
}

/**
 * @brief 发送一个数据包
 * 
 * @param txBuf 
 * @return uint8_t 0成功，1失败
 */
uint8_t Int_NRF24L01_TxPacket(uint8_t * txBuf) {

    // 进入发送模式
    Int_NRF24L01_TX_Mode();

    // Clr_NRF24L01_CE;    // 进入待机模式

    // 使用写Tx FIFO指令，将数据发送
    Int_NRF24L01_Write_Buf(WR_TX_PLOAD, txBuf, TX_PLOAD_WIDTH);     

    // 循环读取状态寄存器，直到发送完成或达到最大重发次数
    uint8_t status = 0;
    do {
        status = Int_NRF24L01_Read_Reg(SPI_READ_REG + STATUS);
    } while(!(status & (TX_OK | MAX_TX)));

    /* 状态位的处理 */
    // 再次写1清空接收标志位或最大重发次数标志位
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + STATUS, status);

    // 达到最大重发次数，，主动清除Tx FIFO，否则无法继续发送
    if(status & MAX_TX) {
        if(status & TX_FULL) {      // 如果发送的FIFO已满
            Int_NRF24L01_Write_Reg(FLUSH_TX, 0xff);
        }
    }

    return ((status & TX_OK) ? 0 : 1);      // 0发送成功

}

/**
 * @brief 接收一个数据包
 * 
 * @param txBuf 存放读取的数据
 * @return uint8_t 0读取成功；1读取失败
 */
uint8_t Int_NRF24L01_RxPacket(uint8_t* txBuf) {
    
    // 进入接收模式
    Int_NRF24L01_RX_Mode();

    /* 读状态寄存器，判断是否收到数据了 */
    uint8_t state = 0;
    state = Int_NRF24L01_Read_Reg(SPI_READ_REG + STATUS);

    /* 清除 TX/MAX_TX 标志 */
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS, state);
    
    /* 如果收到数据了，就开始读 RX FIFO */
    if (state & RX_OK)
    {
        connect_flag = 0;
        /* 从 RX FIFO读取数据到 buff 里 */
        Int_NRF24L01_Read_Buf(RD_RX_PLOAD, txBuf, RX_PLOAD_WIDTH);
        /* 清空 RX FIFO */
        Int_NRF24L01_Write_Reg(FLUSH_RX, 0xff);
        /* =============测试：打印接收的数据================= */
        // for (uint8_t i = 0; i < RX_PLOAD_WIDTH; i++)
        // {
        //     printf("receive[%d]=%02x\r\n", i, txBuf[i]);
        // }
        /* ================================================ */

        return 0; // 成功接收数据
    }
    return 1; // 未接收到数据

}


/**
 * @brief 自检
 * 
 * @return uint8_t 0成功，1失败
 */
uint8_t Int_NRF24L01_Check(void) {

    uint8_t buff_w[5] = {0xA5, 0xA5, 0xA5, 0xA5, 0xA5}; // 要写入的数据
    uint8_t buff_r[5] = {0};                            // 准备接收的数组

    // 往寄存器写字节，，往通信的地址的寄存器里写
    Int_NRF24L01_Write_Buf(TX_ADDR, buff_w, 5);
    // 从寄存器中取
    Int_NRF24L01_Read_Buf(TX_ADDR, buff_r, 5);

    /* 判断取出的和发送过去的是否相同 */
    for(uint8_t i=0; i<5; i++) {
        if(buff_w[i] != buff_w[i]) return 1;        // 校验失败
    }

    return 0;       // 校验成功

}


