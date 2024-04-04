#include "Com_Kalman.h"
#include <math.h>


struct _1_ekf_filter ekf[3] = {
        {0.02, 0, 0, 0, 0.001, 0.543},
        {0.02, 0, 0, 0, 0.001, 0.543},
        {0.02, 0, 0, 0, 0.001, 0.543}
        };

void Com_Kalman_1(struct _1_ekf_filter *ekf,float input)  //Ò»Î¬¿¨¶ûÂü
{
    ekf->Now_P = ekf->LastP + ekf->Q;
    ekf->Kg = ekf->Now_P / (ekf->Now_P + ekf->R);
    ekf->out = ekf->out + ekf->Kg * (input - ekf->out);
    ekf->LastP = (1-ekf->Kg) * ekf->Now_P ;
}
