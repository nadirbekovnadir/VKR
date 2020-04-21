#include "AUV/rov_model.h"
#include "math.h"
#include "kx_pult/qpiconfig.h"

ROV_Model::ROV_Model(QObject *parent) : QObject(parent) {
    resetModel(0,100,0);
    m = 340;
    delta_m = 2;
    cv1[0] = 109; cv1[1] = 950; cv1[2] = 633;
    cv2[0] = 10.9; cv2[1] = 114; cv2[2] = 76;
    cw1[0] = 228.6; cw1[1] = 366; cw1[2] = 366; // kak v rabote Egorova
    cw2[0] = 2.29; cw2[1] = 36.6; cw2[2] = 36.6;
    lambda[1][1] = 50; lambda[2][2] = 101; lambda[3][3] = 101;
    lambda[4][4] = 50; lambda[5][5] = 50; lambda[6][6] = 50;
    J[0] = 9; J[1] = 320; J[2] = 320; //moment inercii apparata vdol sootvetstvuushih osei
    kd = 1; //koefficient usilenija horizontal vma
    kd_vertical = 20;

    h = 0.018; //metacentricheskaya vysota
    Td = 0.15; //postojannaya vremeni dvizhitelei
    //koordinaty uporov dvizhitelei otnositelno centra mass apparata
    l1=0.5;
    l2=0.5;
    l3 = 1.417;
    l4 = 1.662;
    k_mdv_x = cos(10/57.3);
    k_mdv_z = sin(10/57.3);
    max_depth=100;
    //течение = 0
    vxg_current=vyg_current=vzg_current=0;
    a[14] = 0; //NOTE: стартовый X аппарата
    a[15] = max_depth; //NOTE: стартовый Y аппарата (отстояние от дна)
    a[16] = 0; //NOTE: стартовый Z аппарата
    a[23] = 0; //NOTE: путь, пройденный АНПА
    modellingTimeSec =0;
    S=0;
}

ROV_Model::ROV_Model(const QString &config, const QString &name, QObject *parent)
{
    Q_UNUSED(parent)

    QPIConfig conf(config, QIODevice::ReadOnly);
    QPIConfig::Entry & e(conf.getValue(name));
    resetModel(0,100,0);
    m = e.getValue("m",340);
    delta_m = e.getValue("delta_m",2);
    cv1[0] = e.getValue("cv1_0",109);
    cv1[1] = e.getValue("cv1_1",950);
    cv1[2] = e.getValue("cv1_2",633);

    cv2[0] = e.getValue("cv2_0",10.9);
    cv2[1] = e.getValue("cv2_1",114);
    cv2[2] = e.getValue("cv2_2",76);

    cw1[0] = e.getValue("cw1_0",228.6);
    cw1[1] = e.getValue("cw1_1",366);
    cw1[2] = e.getValue("cw1_2",366);

    cw2[0] = e.getValue("cw2_0",2.29);
    cw2[1] = e.getValue("cw2_1",36.6);
    cw2[2] = e.getValue("cw2_1",36.6);
    lambda[1][1] = e.getValue("lambda11",50);
    lambda[2][2] = e.getValue("lambda22",101);
    lambda[3][3] = e.getValue("lambda33",101);
    lambda[4][4] = e.getValue("lambda44",50);
    lambda[5][5] = e.getValue("lambda55",50);
    lambda[6][6] = e.getValue("lambda66",50);

    J[0] = e.getValue("J0",9);
    J[1] = e.getValue("J1",320);
    J[2] = e.getValue("J2",320); //moment inercii apparata vdol sootvetstvuushih osei

    kd = e.getValue("kd",1); //koefficient usilenija dvizhitelei
    kd_vertical = e.getValue("kd_vertical", 20);
    h = e.getValue("h",0.018); //metacentricheskaya vysota
    Td = e.getValue("Td",0.15); //postojannaya vremeni dvizhitelei
    //koordinaty uporov dvizhitelei otnositelno centra mass apparata
    l1=e.getValue("l1",0.5);
    l2=e.getValue("l2",0.5);
    l3 =e.getValue("l3",1.417);
    l4 =e.getValue("l4",1.662);
    k_mdv_x = cos(10/57.3);
    k_mdv_z = sin(10/57.3);
    max_depth=e.getValue("max_depth",100);
    //течение = 0
    vxg_current=e.getValue("current.vx",0);
    vyg_current=0;
    vzg_current=e.getValue("current.vz",0);
    //стартовые координаты аппарата
    a[14] = e.getValue("X0",0); //NOTE: стартовый X аппарата
    a[15] = e.getValue("Y0",max_depth); //NOTE: стартовый Y аппарата (отстояние от дна)
    a[16] = e.getValue("Z",0); //NOTE: стартовый Z аппарата
    a[23] = 0; //NOTE: путь, пройденный АНПА
    modellingTimeSec =0;
    S=0;
}

void ROV_Model::model(const float Uml,const float Ump,const float Uvk,const float Uvn) {
    int limit1, limit2;
    double G,delta_f;

    Q_UNUSED(limit1)

    //модули упоров движителей
    Pml = a[7];  // маршевый левый
    Pmp = a[8];  // маршевый правый
    Pvk = a[9];  // вертикальный кормовой
    Pvn = a[10];  //вертикальный носовой


    //проекции упоров движителей на оси апарата
    Pml_x = Pml*k_mdv_x;
    Pmp_x = Pmp*k_mdv_x;
    Pvk_x = 0;
    Pvn_x = 0;

    Pml_z = -Pml*k_mdv_z;
    Pmp_z = Pmp*k_mdv_z;
    Pvk_z = 0;
    Pvn_z = 0;

    Pml_y = 0;
    Pmp_y = 0;
    Pvk_y = Pvk;
    Pvn_y = Pvn;

    double g = 9.81;
    G = m*g; //вес аппарата
    delta_f = delta_m * g; //плавучесть (H)

    //obnulenie verticalnoi polozhitelnoi skorosti apparata pri dostizhenii poverhnosti
    limit1 = limit2 = 0;
    if (a[15] >= max_depth) {
      a[15] = max_depth;
      if (a[2] >= 0) {
          a[2] = 0;
          limit1 = 1;
      }
    };

    //obnulenie verticalnoi polozhitelnoi skorosti apparata pri dostizhenii dna
    if (a[15] <= 0)
    {
      a[15] = 0;
      if (a[2] <= 0)
      {
          a[2] = 0;
          limit2 = 1;
      }
    };

    Fdx = Pmp_x+Pml_x+Pvn_x+Pvk_x;
    Fgx = -cv1[0] * a[1] * fabs(a[1]) - cv2[0] * a[1];
    FloatageX = sin(a[6]) * delta_f;
    FloatageX = 0; //обнуление остаточной плавучести
    da[1] = (1/(m + lambda[1][1])) * (Fdx + Fgx + FloatageX); //vx'
    X[200][0] = Fdx;

    Fdy = Pmp_y+Pml_y+Pvn_y+Pvk_y;
    Fgy = -cv1[1] * a[2] * fabs(a[2]) - cv2[1] * a[2];
    //FloatageY = cos(a[6]) * cos(a[5]) * delta_f;
    FloatageY = 0; //обнуление остаточной плавучести
    da[2] = (1/(m + lambda[2][2])) * (Fgy + Fdy + FloatageY); //vy'
    X[201][0] = Fdy;

    Fdz = Pmp_z+Pml_z+Pvn_z+Pvk_z;
    Fgz = -cv1[2] * a[3] * fabs(a[3]) - cv2[2] * a[3];
    FloatageZ = -cos(a[6]) * sin(a[5]) * delta_f;
    FloatageZ = 0; //обнуление остаточной плавучести
    X[202][0] = Fdz;

    da[3] = (1/(m + lambda[3][3])) * (Fdz + Fgz + FloatageZ); //vz'

    da[4] = -(1/cos(a[6]) * ((-a[18]) * cos(a[5]) - sin(a[5]) * a[19]));  //proizvodnaya kursa

    da[5] = a[17] - tan(a[6]) * ((-a[18]) * cos(a[5]) - sin(a[5]) * a[19]);  //proizvodnaya krena

    da[6] = a[19] * cos(a[5]) + sin(a[5]) * (-a[18]); //proizvodnaya differenta

    X[26][0]=da[7] = (1/Td) * (kd * (double)Uml - Pml);  // маршевый левый

    X[27][0]=da[8] = (1/Td) * (kd * (double)Ump - Pmp); //маршевый правый

    X[28][0]=da[9] = (1/Td) * (kd_vertical * (double)Uvk - Pvk);  // вертикальный кормовой

    X[29][0]=da[10] = (1/Td) * (kd_vertical * (double)Uvn - Pvn);  // вертикальный носовой

    da[11] = 0;

    da[12] = 0;

    da[13] = 0;

    double alfa[4][4]; //матрица перевода из связанной СК в глобальную СК
    a[4] = -a[4];
    alfa[1][1] = cos(a[4])*cos(a[6]);
    alfa[2][1] = sin(a[6]);
    alfa[3][1] = -sin(a[4])*cos(a[6]);
    alfa[1][2] = sin(a[5])*sin(a[4])-cos(a[5])*cos(a[4])*sin(a[6]);
    alfa[2][2] = cos(a[5])*cos(a[6]);
    alfa[3][2] = sin(a[5])*cos(a[4])+cos(a[5])*sin(a[4])*sin(a[6]);
    alfa[1][3] = cos(a[5])*sin(a[4])+sin(a[5])*cos(a[4])*sin(a[6]);
    alfa[2][3] = -sin(a[5])*cos(a[6]);
    alfa[3][3] = cos(a[5])*cos(a[4])-sin(a[4])*sin(a[5])*sin(a[6]);
    a[4] = -a[4];

    da[14] = alfa[1][1] * a[1] + alfa[1][2] * a[2] + alfa[1][3] * a[3] + vxg_current;
    //dx_global

    da[15] = alfa[2][1] * a[1] + alfa[2][2] * a[2] + alfa[2][3] * a[3] + vyg_current;
    //dy_global

    da[16] = alfa[3][1] * a[1] + alfa[3][2] * a[2] + alfa[3][3] * a[3]+ vzg_current;
    //dz_global

    double Fa = G + delta_f;
    double Fax = sin(a[6])*Fa;
    //float Fay = cos(a[5])*cos(a[6])*Fa;
    double Faz = -sin(a[5])*cos(a[6])*Fa;

    Mdx = 0;
    Mgx = -cw1[0] * a[17] * fabs(a[17]) - cw2[0] * a[17];
    Max = Faz*h;
    //Max = 0; //obnulenie momenta ot sily Arhimeda
    da[17] = (1/(J[0] + lambda[4][4])) * (Mdx + Mgx + Max);

    Mdy = -Pml*l1+Pmp*l2;
    Mgy = -cw1[1] * a[18] * fabs(a[18]) - cw2[1] * a[18];
    da[18] = (1/(J[1] + lambda[5][5])) * (Mdy + Mgy);

    Mdz = Pvn*l4-Pvk*l3;
    Mgz = -cw1[2] * a[19] * fabs(a[19]) - cw2[2] * a[19];
    Maz = -h*Fax;
    //Maz = 0; //obnulenie momenta ot sily Arhimeda
    da[19] = (1/(J[2] + lambda[6][6])) * (Mdz + Mgz +Maz);

    da[20] = a[1];
    da[21] = a[2];
    da[22] = a[3];
    da[23] = sqrt (vx_local*vx_local+vy_local*vy_local+vz_local*vz_local);

}


void ROV_Model::resetModel(double X, double y, double Z){
    for (int i=0;i<ANPA_MOD_CNT;i++) {a[i] = 0.0f; da[i]=0.0f;}
    a[14] = X;
    a[15] = y;
    a[16] = Z;
    modellingTimeSec = 0;
    emit modelResetResult("done");
}

void ROV_Model::setCurrent(double vx, double vy, double vz)
{
    vxg_current=vx;
    vyg_current=vy;
    vzg_current=vz;
}

void ROV_Model::tick(const float Uml,const float Ump,const float Uvk,const float Uvn, const float Ttimer){
    runge(Uml,Ump,Uvk,Uvn,Ttimer);
}

ROV_Model::~ROV_Model(){

}

void ROV_Model::runge(const float Uml, const float Ump, const float Uvk, const float Uvn, const float dt_) {
    const double Kc = 180/M_PI;
    double a1[ANPA_MOD_CNT], y[ANPA_MOD_CNT];
    int i;
    const double H1 = dt_;
    const int n = ANPA_MOD_CNT;
    modellingTimeSec+=dt_; //увеличение времени моделирования (секунды)
    model(Uml,Ump,Uvk,Uvn);
    for (i = 1; i < n; i++) {
      a1[i] = a[i];
      y[i] = da[i];
      a[i] = a1[i] + 0.5 * H1 * da[i];
    }

    model(Uml,Ump,Uvk,Uvn);
    for (i = 1; i < n; i++)
    {
      y[i] = y[i]+ 2 * da[i];
      a[i] = a1[i] + 0.5 * H1 * da[i];
    }
    model(Uml,Ump,Uvk,Uvn);
    for (i = 1; i < n; i++) {
      y[i] = y[i] + 2 * da[i];
      a[i] = a1[i] + H1 * da[i];
    }
    model(Uml,Ump,Uvk,Uvn);
    for (i = 1; i < n; i++) {
      a[i] = a1[i] + (H1 / 6) * (y[i] + da[i]);
    }


    //данные в СУ ( с преобразованием координат)

    x_global = a[14]; //koordinata apparata v globalnoi SK
    y_global = a[15];  //otstojanie ot dna otnositelno repernoi tochki, kotoraja na dne
    cur_depth = max_depth - y_global;  //tekush"aya glubina SPA
    z_global = a[16]; //koordinaty apparata v globalnoi SK (преобразование координат)
    Wx = a[17] * Kc; //uglovye skorosti SPA v svyazannyh osyah v gradus/sekunda
    Wy = a[18] * Kc;
    Wz = a[19] * Kc;

    vx_local = a[1]; vy_local = a[2]; vz_local = a[3];  //lineinye skorosti SPA v svyazannyh osyah
    vx_global = da[14]; vy_global = da[15]; vz_global = da[16];  // lineinye skorosti SPA v globalnyh osyah

    Psi_g = a[4] * Kc; // ugol kursa (преобразование координат)
    Gamma_g = a[5] * Kc; // ugol krena
    Tetta_g = a[6] * Kc; // ugol differenta
    W_Psi_g = da[4] * Kc; // proizvodnaya ugla kursa
    W_Gamma_g = da[5] * Kc; // proizvodnaya ugla krena
    W_Tetta_g = da[6] * Kc; // proizvodnaya ugla differenta
    S = a[23]; //общий пройденный путь
    N = fabs(Psi_g / 360);
    if (Psi_g >= 360) Psi_gi = Psi_g - N * 360; // ugol kursa na indikaciu
    if (Psi_g <= -360) Psi_gi = Psi_g + N * 360;




}



