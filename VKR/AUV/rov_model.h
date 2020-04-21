#ifndef ROV_MODEL_H
#define ROV_MODEL_H

#include <QObject>

#define ANPA_MOD_CNT 25

extern QVector<double> K;
extern double X[2000][2];

struct InitData {
    double k_gamma;
    double m;
    double delta_m;
    double cv1[3];
    double cv2[3];
    double cw1[3];
    double cw2[3];
    double lambda[6][6];
    double J[3];
    double kd;
    double h;
    double Td;
    double l1,l2;
    double max_depth;
}; //struct InitData


class ROV_Model : public QObject {
    Q_OBJECT
public:
    explicit ROV_Model(QObject *parent = 0);
    explicit ROV_Model(const QString & config, const QString & name = "rov_model",QObject *parent = 0);
    virtual ~ROV_Model();
signals:
    void modelResetResult(QString res);
public slots:
    void resetModel(double X, double y, double Z);

private:
    void start();
    double modellingTimeSec;

public:
    void model(const float Uml,const float Ump,const float Uvk,const float Uvn);
    void runge(const float Uml,const float Ump,const float Uvk,const float Uvn, const float dt=0.01);
    int modellingTimeMsec(){
        return modellingTimeSec*1000; //преобразуем полученное время моделирования в миллисекунды
    }
    double a[ANPA_MOD_CNT];
    double da[ANPA_MOD_CNT];
    double delta_f;

    //константы
    double k_gamma;
    double m;
    double g;
    double G;
    double delta_m;
    double cv1[3];
    double cv2[3];
    double cw1[3];
    double cw2[3];
    double lambda[7][7];
    double J[3];
    double kd;
    double kd_vertical;
    double k_mdv_x, k_mdv_z;
    double h;
    double Td;
    double l1,l2,l3,l4;
    double max_depth;
    //переменные
    double sumX, sumZ;
    double cur_depth, Wx, Wy, Wz;
    double Psi_g, Gamma_g, Tetta_g;

    double Psi_gi, W_Psi_g, W_Gamma_g, W_Tetta_g;
    int N;


    //упоры движителей
    double Pml, Pmp, Pvk, Pvn;
    //проекции упоров движителей
    double Pml_x, Pmp_x, Pvk_x, Pvn_x;
    double Pml_z, Pmp_z, Pvk_z, Pvn_z;
    double Pml_y, Pmp_y, Pvk_y, Pvn_y;
    double Uml, Ump, Uvk, Uvn; //напряжения движителей

    double FloatageX, FloatageY, FloatageZ, Fdx, Fdy, Fdz, Fgx, Fgy, Fgz;
    double Mdx, Mdy, Mdz, Mgx, Mgy, Mgz;
    double Max,May,Maz; // моменты от силы Архимеда

    double x_global, y_global, z_global;
    double vx_local,  vy_local, vz_local;  //lineinye skorosti SPA v svyazannyh osyah
    double vx_global, vy_global, vz_global;
    double vxg_current, vyg_current, vzg_current; //скорость течения в глобальной СК
    double S; //скорость аппаратад для расчета пути

public:

    void setCurrent(double vx, double vy, double vz);
    void tick(const float Uml,const float Ump,const float Uvk,const float Uvn, const float Ttimer);
    float Fx,Fy,Fz; //total forces for XYZ-axis
    float Mx,My,Mz; //total moments for XYZ-axis
};

#endif // ROV_MODEL_H
