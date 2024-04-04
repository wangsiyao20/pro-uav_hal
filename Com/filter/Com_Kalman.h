#ifndef _COM_KALMAN_H
#define _COM_KALMAN_H




struct _1_ekf_filter
{
    float LastP;    //��һʱ�̵�״̬�����Э���
    float Now_P;    //��ǰʱ�̵�״̬�����Э���
    float out;      //�˲��������ֵ�������Ƶ�״̬
    float Kg;       //���������棬���ڵ���Ԥ��ֵ�Ͳ���ֵ֮���Ȩ��
    float Q;        //���������ķ����ӳϵͳģ�͵Ĳ�ȷ����
    float R;        //���������ķ����ӳ�������̵Ĳ�ȷ����
};

extern struct _1_ekf_filter ekf[3];

extern void Com_Kalman_1(struct _1_ekf_filter *ekf,float input);  //һά������

#endif


