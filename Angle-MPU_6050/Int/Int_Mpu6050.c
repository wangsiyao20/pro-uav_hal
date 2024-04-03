#include "Int_Mpu6050.h"
#include "i2c.h"

void for_delay_ms(uint32_t ms)
{
  uint32_t Delay = ms * 72000 / 9; /* 72Mʱ��Ƶ�� */
  do
  {
    __NOP(); /* ��ָ�NOP����ռ�� CPU ʱ�� */
  } while (Delay--);
}


/**
 * @description: ��MPU6050дһ���ֽ�
 * @param {uint8_t} reg_addr  �ڲ��Ĵ����ĵ�ַ
 * @param {uint8_t} byte      Ҫ���͵�1�ֽ�����
 * @return {*}
 */
uint8_t Int_MPU6050_WriteByte(uint8_t reg_addr, uint8_t byte)
{
    HAL_I2C_Mem_Write(&hi2c1,(MPU_IIC_ADDR << 1 ),reg_addr,I2C_MEMADD_SIZE_8BIT,&byte,1,2000);
}

/**
 * @description: ��MPU6050��ȡһ���ֽ�
 * @param {uint8_t} reg_addr  �ڲ��Ĵ����ĵ�ַ
 * @param {uint8_t} byte      �����ȡ��1�ֽ�����
 * @return {*}
 */
uint8_t Int_MPU6050_ReadByte(uint8_t reg_addr, uint8_t *byte)
{
   return HAL_I2C_Mem_Read(&hi2c1,(MPU_IIC_ADDR << 1),reg_addr, I2C_MEMADD_SIZE_8BIT,byte,1,2000);
}

/**
 * @description: ��MPU6050д����ֽ�
 * @param {uint8_t} dev_addr  �豸��ַ
 * @param {uint8_t} reg_addr  �ڲ��Ĵ����ĵ�ַ
 * @param {uint8_t} data      Ҫ���͵Ķ��ֽ�����
 * @param {uint8_t} size      Ҫ���͵��ֽ���
 * @return {*}
 */
uint8_t Int_MPU6050_Write_Len(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t size)
{
    return HAL_I2C_Mem_Write(&hi2c1,(MPU_IIC_ADDR << 1),reg_addr,I2C_MEMADD_SIZE_8BIT,data,size,2000);
}


/**
 * @description: ��MPU6050������ֽ�
 * @param {uint8_t} reg_addr  �ڲ��Ĵ����ĵ�ַ
 * @param {uint8_t} data      ���ն�ȡ�Ķ��ֽ�����
 * @param {uint8_t} size      Ҫ���յ��ֽ���
 * @return {*}
 */
uint8_t Int_MPU6050_Read_Len(uint8_t reg_addr, uint8_t *data, uint8_t size)
{

    HAL_I2C_Mem_Read(&hi2c1,(MPU_IIC_ADDR << 1 ),reg_addr,I2C_MEMADD_SIZE_8BIT,data,size,2000);
}

/**
 * @description: ���������ǵ�������
 * @param {uint8_t} fsr  ����������ֵ��  3 ----- +-2000 ��/s
 * @return {*}
 */
uint8_t Int_MPU_Set_Gyro_Fsr(uint8_t fsr)
{
    /* bit4��bit3 */
    return Int_MPU6050_WriteByte(MPU_GYRO_CFG_REG,fsr<<3);
}


/**
 * @description: ���ü��ٶȼƵ�������
 * @param {uint8_t} fsr ����������ֵ��  0  --------- +- 2g
 * @return {*}
 */
uint8_t Int_MPU_Set_Accel_Fsr(uint8_t fsr)
{
    /* bit4��bit3 */
    return Int_MPU6050_WriteByte(MPU_ACCEL_CFG_REG,fsr<<3);
}

/**
 * @description: ���õ�ͨ�˲����Ĵ�������Ƶ�ʣ��������Ĳ��ܹ���
 * @param {uint16_t} band_width ���õĴ���
 * @return {*}
 */
uint8_t Int_MPU_Set_LPF(uint16_t band_width)
{
    uint8_t cfg=0;
    /* �Ӹߵ���ֵ���͵���ֵ�жϣ�������ݵ����ϸ�Ҫ�� */
    if(band_width>= 188)
    {
        cfg=1;
    }
    else if(band_width >= 98)
    {
        cfg = 2;
    }
    else if (band_width >= 42)
    {
        cfg = 3;
    }
    else if (band_width >= 20)
    {
        cfg=4;
    }
    else if (band_width >=10)
    {
        cfg = 5;
    }
    else 
    {
        cfg =6;
    }
    /* bit2 --- bit0 */
    return Int_MPU6050_WriteByte(MPU_CFG_REG,cfg);

}

/**
 * @description: ���������ǵĲ����� = ���Ƶ��/(1+��Ƶ)�����õ�ͨ�˲�����=������/2;
 * @param {uint16_t} rate �����Ĳ���ֵ��4-1000���ֲ�min=4,1k/8k���Ǽ��ٶ�1k��
 * @return {*}
 */
uint8_t Int_MPU_Set_Rate(uint16_t rate)
{
    uint8_t div=0;
    /* ����Ƶ��=���Ƶ�ʣ�1k��/(1+��Ƶ) ==�������Ƶ ===�� ���õ��Ĵ�����*/
    /* Ϊ���Ժ��ͨ���ԣ�������һ����ֵ���� */
    if(rate > 1000)
    {
        rate = 1000;
    }
    else if(rate < 4)
    {
        rate = 4;
    }

    /* �����Ƶ = ���Ƶ�ʣ�1k��/ ����Ƶ��rate -1 */
    div = 1000/rate - 1 ;
    /* ������õ��ķ�Ƶ��д�� �����ʷ�Ƶ������ */
    Int_MPU6050_WriteByte(MPU_SAMPLE_RATE_REG,div);
    /* ˳���������µ�ͨ�˲��Ĵ��� = ������/2 */
    return Int_MPU_Set_LPF(rate/2);
}


/**
 * @brief ��ʼ��
 * 
 */
void Int_MPU6050_Init()
{
    uint8_t res=0;
    /* 1�� ��ʼ��I2C2 */
    

    /* 2�� ��λ */
    Int_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0X80);
    /* 3���ӳ�һ�� */
    for_delay_ms(100);
    /* 4�� ���� */
    Int_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0X00);
    /* 5�����������̣������ǣ����ٶȣ���+- 2000 ��/s ,��Ϊ11*/
    Int_MPU_Set_Gyro_Fsr(3);
    /* 6������������: ���ٶȣ� +- 2g����Ϊ0 */
    Int_MPU_Set_Accel_Fsr(0);
    /* 7�������жϡ�FIFO��I2C AUXģʽ */
    Int_MPU6050_WriteByte(MPU_INT_EN_REG, 0X00);
    Int_MPU6050_WriteByte(MPU_FIFO_EN_REG, 0X00);
    Int_MPU6050_WriteByte(MPU_USER_CTRL_REG, 0X00);
    /* �ж��Ƿ���ã�������Զ�������ID��˵��ǰ��ĳ�ʼ��û�д��� */
    Int_MPU6050_ReadByte(MPU_DEVICE_ID_REG,&res);
    if (res == MPU_IIC_ADDR)
    {
        /* 8��ʱ�ӣ�Ĭ��8M����===�� X���ϵ�ʱ�� */
        Int_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0X01);
        /* 8�������ǵĲ�����\���õ�ͨ�˲�����(������һ��) �����Ƶ��100��10ms����һ��*/
        Int_MPU_Set_Rate(100);     
        /* 9�����ٶ��������Ƕ����� */
        Int_MPU6050_WriteByte(MPU_PWR_MGMT2_REG, 0X00); 
    }
}


/**
 * @description: ��ȡ���ٶȵ�ԭʼ����
 * @param {short} *ax
 * @param {short} *ay
 * @param {short} *az
 * @return {*}
 */
uint16_t Int_MPU6050_GetAccl(short *ax, short *ay, short *az)
{
    uint8_t acc_buff[6];
    /* ÿ�����Ӧ2���Ĵ�������8λ����8λ�� ��һ����0x3B��X��ĸ�λ�� */
    Int_MPU6050_Read_Len(MPU_ACCEL_XOUTH_REG,acc_buff,6);
    /* 
        acc_buff[0]:X����ٶȵĸ�8λ��
        acc_buff[1]:X����ٶȵĵ�8λ��
        acc_buff[2]:Y����ٶȵĸ�8λ��
        acc_buff[3]:Y����ٶȵĵ�8λ��
        acc_buff[4]:Z����ٶȵĸ�8λ��
        acc_buff[5]:Z����ٶȵĵ�8λ��
     */
    *ax= ((short)acc_buff[0] << 8) | acc_buff[1];
    *ay= ((short)acc_buff[2] << 8) | acc_buff[3];
    *az= ((short)acc_buff[4] << 8) | acc_buff[5];
    
}

/**
 * @description: ��ȡ�����ǵĽ��ٶ�ԭʼ����
 * @param {short} *gx
 * @param {short} *gy
 * @param {short} *gz
 * @return {*}
 */
uint16_t Int_MPU6050_GetGyro(short *gx, short *gy, short *gz)
{
    uint8_t gyro_buff[6];
    /* ÿ�����Ӧ2���Ĵ�������8λ����8λ�� ��һ����0x43��X��ĸ�λ�� */
    Int_MPU6050_Read_Len(MPU_GYRO_XOUTH_REG,gyro_buff,6);
    /* 
        gyro_buff[0]:X����ٶȵĸ�8λ��
        gyro_buff[1]:X����ٶȵĵ�8λ��
        gyro_buff[2]:Y����ٶȵĸ�8λ��
        gyro_buff[3]:Y����ٶȵĵ�8λ��
        gyro_buff[4]:Z����ٶȵĸ�8λ��
        gyro_buff[5]:Z����ٶȵĵ�8λ��
     */
    *gx= ((short)gyro_buff[0] << 8) | gyro_buff[1];
    *gy= ((short)gyro_buff[2] << 8) | gyro_buff[3];
    *gz= ((short)gyro_buff[4] << 8) | gyro_buff[5];
}

/**
 * @return uint8_t ��ȡ�����豸ID����������
 */
uint8_t Int_MPU6050_GetID(void) {
    uint8_t res = 0;
    Int_MPU6050_ReadByte(MPU_DEVICE_ID_REG, &res);
    return res;
}





