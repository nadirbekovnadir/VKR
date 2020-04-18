#ifndef SU_ROV_H
#define SU_ROV_H

#include <QObject>
#include "kx_pult/kx_protocol.h"
#include "kx_pult/qkx_coeffs.h"
#include "AUV/rov_model.h"

extern double X[2000][2];
extern QVector<double> K;

enum SU_MODE {Ruchnoi, Automatiz};
class SU_ROV : public QObject
{
    Q_OBJECT
public:
    explicit SU_ROV(QObject *parent = nullptr);

signals:

public slots:
    void tick();
    void addKurs();//увеличение заданного курса по кнопке
    void decKurs();//уменьшение заданного курса по кнопке
    void changeMode(int btn, bool state);
private:
    Qkx_coeffs * K_Protocol;
    x_protocol * X_Protocol;
    ROV_Model *model;
    QTimer time;
    float T; //период таймера
    SU_MODE mode; //режим работы СУ НПА
    void Control_Kurs();
    void Control_Marsh();
    void Control_Kren();
    void Control_Different();
    void Control_Depth();
    void BFS_DRK(double Upsi, double Uteta, double Ux, double Uy);
    void get_data_from_model();
    int sign(float input){
        return (input>=0) ? 1 : -1;
    }
    float saturation(float input, float max);
};

#endif // SU_ROV_H
