#include "AUV/su_rov.h"

double X[2000][2];

SU_ROV::SU_ROV(QObject *parent) : QObject(parent)
{
    K_Protocol = new Qkx_coeffs("protocols.conf", "ki");
    X_Protocol = new x_protocol ("protocols.conf","xi",X);
    model = new ROV_Model ();

    X[41][0] = 0; // Начальное значение курса равно нулю
    //if (K[1]>0) T=K[1];
    /*else*/ T=0.01;
    time.start(T*1000);//запуск проводим в мсек
    connect (&time, SIGNAL(timeout()), SLOT(tick()));

}



void SU_ROV::tick()
{
    get_data_from_model();
    Control_Kurs();
    BFS_DRK(X[49][0], X[9][0],0,0);
    model->tick(X[22][0], X[23][0], X[24][0], X[25][0], T);;
}





void SU_ROV::addKurs()
{
    qDebug() << "Course before changes: " << X[41][0];
    X[41][0]++;
    qDebug() << "Course after changes: " << X[41][0];
}
void SU_ROV::decKurs(){
    X[41][0]--;
}

void SU_ROV::changeMode(int btn, bool state)
{
    if (btn==Automatiz && state) mode=Automatiz;
    else mode=Ruchnoi;
}



void SU_ROV::BFS_DRK(double Upsi, double Uteta, double Ux, double Uy){

    //ограничим входные задающие сигналы в бфс ДРК
    X[11][0] = saturation(Ux, K[11]);
    X[12][0] = saturation(Upsi,K[12]);
    X[13][0] = saturation(Uteta,K[13]);
    X[74][0] = saturation(Uy, K[66]);

    //далее по структурной схеме БФС, вычисляем значения после первого сумматора
    X[14][0] = -X[12][0] + X[11][0]; //промежуточное значение для ВМА МЛ (управление курсом и маршем)
    X[15][0] = X[12][0] + X[11][0]; //промежуточное значение для ВМА МПр (управление курсом и маршем)
    X[16][0] = -X[74][0] - X[13][0]; //промежуточное значение для ВМА ВК (управление дифферентом и глубиной)
    X[17][0] = -X[74][0] + X[13][0]; //промежуточное значение для ВМА ВН (управление дифферентом и глубиной)


    //ограничим и промасштабируем управляющие значения напряжений для ВМА
    X[18][0] = saturation(X[14][0],K[14]); //управляющее напряжение на ВМА МВЛ
    X[19][0] = saturation(X[15][0],K[15]); //управляющее напряжение на ВМА МНЛ
    X[20][0] = saturation(X[16][0],K[16]); //управляющее напряжение на ВМА МВП
    X[21][0] = saturation(X[17][0],K[17]); //управляющее напряжение на ВМА МНП

    X[22][0] = X[18][0]*K[22];
    X[23][0] = X[19][0]*K[23];
    X[24][0] = X[20][0]*K[24];
    X[25][0] = X[21][0]*K[25];
}

void SU_ROV::Control_Kurs()
{

    //X[41][0]=K[20]; //INFO: заданный курс для отладки через kx-pult
    X[43][0]=X[41][0]-X[42][0];
    X[44][0]=X[43][0]*K[44];

    X[45][0]=X[50][0]*K[45];
    X[46][0]=X[44][0]-X[45][0]+X[41][0]*K[43];
    X[48][0]=X[46][0];
    X[49][0]=saturation(X[48][0],K[49]);


}

void SU_ROV::Control_Marsh()
{
    //X[9][0]=K[];
}

void SU_ROV::Control_Different(){
    //mode = Automatiz; //NOTE: отладка


    X[90][0]=K[18]; //заданное значение дифферента
    X[91][0]=X[90][0];
    X[93][0]=X[91][0]-X[39][0];
    X[94][0]=X[93][0]*K[90];

    X[100][0]=X[91][0]*K[91];
    X[108][0] = sin(X[39][0]/57.3)*K[108];
    X[101][0]=X[94][0]+X[100][0]+X[108][0];

    X[102][0]=X[51][0]*K[93];
    X[103][0]=X[101][0]-X[102][0];
    X[104][0]=saturation(X[103][0],K[95]);
}

void SU_ROV::Control_Depth(){
       X[60][0]=K[19]; //NOTE: заданная глубина через коэффициент для тестирования
        X[61][0]=X[60][0]-X[62][0];
        X[63][0]=K[60]*X[61][0];

        X[68][0]=X[73][0]*K[64];
        X[69][0]=X[63][0]+K[65]*X[60][0];
        X[77][0]=X[69][0]-X[68][0];
        X[70][0]=saturation(X[77][0],K[63]);
}

float SU_ROV::saturation(float input, float max) {
    if (fabs(input)>= max)
        return (sign(input)*max);
    else return input;
}


void SU_ROV::get_data_from_model()
{
    X[32][0]=model->Fx;
    X[33][0]=model->vx_global;
    X[34][0]=model->vx_local;
    X[35][0]=model->x_global;
    X[36][0]=model->y_global;
    X[37][0]=model->z_global;
    X[39][0]=model->Tetta_g;
    X[40][0]=model->Gamma_g;
    X[51][0]=model->W_Tetta_g;
    X[42][0]=model->Psi_g;
    X[50][0]=model->W_Psi_g;
    X[73][0]=model->vy_global;
    X[62][0]=model->cur_depth;
}
