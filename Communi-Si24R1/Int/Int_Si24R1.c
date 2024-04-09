#include "Int_Si24R1.h"
#include "stdio.h"

/* ���оƬ��NRF24L01оƬ�÷�һ�� */

uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x0A, 0x03, 0x06, 0x0E, 0x01}; // ����һ����̬���͵�ַ
uint8_t RX_ADDRESS[RX_ADR_WIDTH] = {0x0A, 0x03, 0x06, 0x0E, 0x01}; // ����һ����̬���͵�ַ

uint16_t connect_flag = 1;       // 0���ӳɹ���1����ʧ��

/**
 * @brief дһ���ֽ�
 * 
 * @param reg 
 * @param data 
 */
void Int_NRF24L01_Write_Reg(uint8_t reg, uint8_t data) {

    // Ƭѡѡ��
    Clr_NRF24L01_CSN;

    // д�Ĵ�����ַ
    uint8_t rxData = 0;
    HAL_SPI_TransmitReceive(&hspi1,  &reg, &rxData, 1, 100);
    // д������
    HAL_SPI_TransmitReceive(&hspi1, &data, &rxData, 1, 100);

    // Ƭѡȡ��
    Set_NRF24L01_CSN;

}

/**
 * @brief ��ȡһ���ֽ�����
 * 
 * @param reg 
 * @return uint8_t 
 */
uint8_t Int_NRF24L01_Read_Reg(uint8_t reg) {

    // Ƭѡѡ��
    Clr_NRF24L01_CSN;

    // д�Ĵ�����ַ
    uint8_t rxData = 0;
    HAL_SPI_TransmitReceive(&hspi1, &reg, &rxData, 1, 100);

    // ��ȡ����(д��ʲô����ν)
    HAL_SPI_TransmitReceive(&hspi1, &reg, &rxData, 1, 100);

    // Ƭѡȡ��
    Set_NRF24L01_CSN;

    return rxData;

}

/**
 * @brief ��ָ���Ĵ���д����ֽ�
 * 
 * @param reg �Ĵ�����ַ
 * @param pBuf д����ֽڵ�ָ��
 * @param len �ֽ���
 */
void Int_NRF24L01_Write_Buf(uint8_t reg, uint8_t* pBuf, uint8_t len) {

    // Ƭѡѡ��
    Clr_NRF24L01_CSN;

    // д�Ĵ�����ַ
    uint8_t rxData = 0;
    HAL_SPI_TransmitReceive(&hspi1, &reg, &rxData, 1, 100);

    // д�����ֽ�
    HAL_SPI_TransmitReceive(&hspi1, pBuf, &rxData, len, 100);

    // Ƭѡȡ��
    Set_NRF24L01_CSN;

}

/**
 * @brief ������ֽ�
 * 
 * @param reg �Ĵ�����ַ
 * @param pBuf Ҫ���뵽�ĵط�
 * @param len Ҫ���յ����ݳ���
 */
void Int_NRF24L01_Read_Buf(uint8_t reg, uint8_t* pBuf, uint8_t len) {

    // Ƭѡѡ��
    Clr_NRF24L01_CSN;

    // д�Ĵ�����ַ
    uint8_t rxData = 0;
    HAL_SPI_TransmitReceive(&hspi1, &reg, &rxData, 1, 100);

    // ��ȡ�������
    HAL_SPI_TransmitReceive(&hspi1, &rxData, pBuf, len, 100);

    // Ƭѡȡ��
    Set_NRF24L01_CSN;

}


/**
 * @brief ����ģʽ��ʼ��
 * 
 */
void Int_NRF24L01_TX_Mode(void) {

    Clr_NRF24L01_CE;    // CE=0�����������ģʽ

    /* 2����ص����ã����͵�ַ�����չܵ�0��ַ��һ������ACKʹ�ܡ�ʹ�ܹܵ�0�����ʡ�ConfigΪ����ģʽ */
    Int_NRF24L01_Write_Buf(SPI_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // д�뷢�͵�ַ
    Int_NRF24L01_Write_Buf(SPI_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ

    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + EN_AA, 0x01);      // ʹ�ܽ���ͨ��0�Զ�Ӧ��
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + EN_RXADDR, 0x01);  // ʹ�ܽ���ͨ��0
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + SETUP_RETR, 0x0a); // �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + RF_CH, 40);        // ѡ����Ƶͨ��0x40
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + RF_SETUP, 0x0f);   // ���ݴ�����2Mbps�����书��7dBm
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + CONFIG, 0x0e);     // CRCʹ�ܣ�16λCRCУ�飬�ϵ�

    Set_NRF24L01_CE;    // ʹ��CE��������ģʽ
}

/**
 * @brief ����ģʽ
 * 
 * @return uint8_t 
 */
void Int_NRF24L01_RX_Mode(void)
{
    Clr_NRF24L01_CE;
    /* 
        �뷢�͵�����
            1������Ҫ���÷��͵�ַ
            2����Ҫ���ý���ͨ��0�ĸ��س���
            3��config���õģ�bit0=1 Ϊ����ģʽ
     */
    Int_NRF24L01_Write_Buf(SPI_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // �����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + EN_AA, 0x01);                          // ʹ�ܽ���ͨ��0�Զ�Ӧ��
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + EN_RXADDR, 0x01);                      // ʹ�ܽ���ͨ��0
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + RF_CH, 40);                            // ѡ����Ƶͨ��0x40
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);             // ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + RF_SETUP, 0x0f);                       // ���ݴ�����2Mbps�����书��7dBm
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + CONFIG, 0x0f);                         // CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
    
    Set_NRF24L01_CE;                                              // ����CE���������豸
}

/**
 * @brief ����һ�����ݰ�
 * 
 * @param txBuf 
 * @return uint8_t 0�ɹ���1ʧ��
 */
uint8_t Int_NRF24L01_TxPacket(uint8_t * txBuf) {

    // ���뷢��ģʽ
    Int_NRF24L01_TX_Mode();

    // Clr_NRF24L01_CE;    // �������ģʽ

    // ʹ��дTx FIFOָ������ݷ���
    Int_NRF24L01_Write_Buf(WR_TX_PLOAD, txBuf, TX_PLOAD_WIDTH);     

    // ѭ����ȡ״̬�Ĵ�����ֱ��������ɻ�ﵽ����ط�����
    uint8_t status = 0;
    do {
        status = Int_NRF24L01_Read_Reg(SPI_READ_REG + STATUS);
    } while(!(status & (TX_OK | MAX_TX)));

    /* ״̬λ�Ĵ��� */
    // �ٴ�д1��ս��ձ�־λ������ط�������־λ
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG + STATUS, status);

    // �ﵽ����ط����������������Tx FIFO�������޷���������
    if(status & MAX_TX) {
        if(status & TX_FULL) {      // ������͵�FIFO����
            Int_NRF24L01_Write_Reg(FLUSH_TX, 0xff);
        }
    }

    return ((status & TX_OK) ? 0 : 1);      // 0���ͳɹ�

}

/**
 * @brief ����һ�����ݰ�
 * 
 * @param txBuf ��Ŷ�ȡ������
 * @return uint8_t 0��ȡ�ɹ���1��ȡʧ��
 */
uint8_t Int_NRF24L01_RxPacket(uint8_t* txBuf) {
    
    // �������ģʽ
    Int_NRF24L01_RX_Mode();

    /* ��״̬�Ĵ������ж��Ƿ��յ������� */
    uint8_t state = 0;
    state = Int_NRF24L01_Read_Reg(SPI_READ_REG + STATUS);

    /* ��� TX/MAX_TX ��־ */
    Int_NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS, state);
    
    /* ����յ������ˣ��Ϳ�ʼ�� RX FIFO */
    if (state & RX_OK)
    {
        connect_flag = 0;
        /* �� RX FIFO��ȡ���ݵ� buff �� */
        Int_NRF24L01_Read_Buf(RD_RX_PLOAD, txBuf, RX_PLOAD_WIDTH);
        /* ��� RX FIFO */
        Int_NRF24L01_Write_Reg(FLUSH_RX, 0xff);
        /* =============���ԣ���ӡ���յ�����================= */
        // for (uint8_t i = 0; i < RX_PLOAD_WIDTH; i++)
        // {
        //     printf("receive[%d]=%02x\r\n", i, txBuf[i]);
        // }
        /* ================================================ */

        return 0; // �ɹ���������
    }
    return 1; // δ���յ�����

}


/**
 * @brief �Լ�
 * 
 * @return uint8_t 0�ɹ���1ʧ��
 */
uint8_t Int_NRF24L01_Check(void) {

    uint8_t buff_w[5] = {0xA5, 0xA5, 0xA5, 0xA5, 0xA5}; // Ҫд�������
    uint8_t buff_r[5] = {0};                            // ׼�����յ�����

    // ���Ĵ���д�ֽڣ�����ͨ�ŵĵ�ַ�ļĴ�����д
    Int_NRF24L01_Write_Buf(TX_ADDR, buff_w, 5);
    // �ӼĴ�����ȡ
    Int_NRF24L01_Read_Buf(TX_ADDR, buff_r, 5);

    /* �ж�ȡ���ĺͷ��͹�ȥ���Ƿ���ͬ */
    for(uint8_t i=0; i<5; i++) {
        if(buff_w[i] != buff_w[i]) return 1;        // У��ʧ��
    }

    return 0;       // У��ɹ�

}


