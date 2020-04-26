#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>
#include <QVector>
#include <QPoint>
#include <Algorithms/a_star.h>
#include <Algorithms/li.h>
#include <Algorithms/d_lite.h>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_addObstacle_clicked()
{
    Obstacle *obst;

    double scale =  ui->dsb_obstacleScale->value();
    QString obstFormStr = ui->cb_obstacleForm->currentText();
    int rotation = ui->le_obstacleRotation->text().toInt();

    if (obstFormStr == "Стена")
        obst = new Obstacle(Obstacle::wall);
    else if (obstFormStr == "Квадрат")
        obst = new Obstacle(Obstacle::square);
    else
        obst = new Obstacle(Obstacle::circle);

    int centerX = int(ui->map->scene()->sceneRect().width() / 2);
    int centerY = int(ui->map->scene()->sceneRect().height() / 2);

    int randomX = QRandomGenerator::global()->bounded(-100, 100);
    int randomY = QRandomGenerator::global()->bounded(-100, 100);

    ui->map->AddObstacle(obst, centerX + randomX, centerY + randomY, rotation, scale);
}

void MainWindow::on_btn_deleteObstacle_clicked()
{
    ui->map->DeleteSelectedObstacles();
}

void MainWindow::on_btn_deleteAllObstacles_clicked()
{
    ui->map->DeleteAllObstacles();
}

void MainWindow::on_btn_routeBuildingStart_clicked()
{
    QPoint start;
    QPoint finish;

    QVector<QVector<int>> map = ui->map->GetMapDots(start, finish);

    QVector<QPoint> path;

    QString currentAlgorihm;
    QColor color = Qt::white;

    qint64 wastedTime = -1;

    if (ui->btn_dataFromVehicle->isChecked())
    {
        currentAlgorihm = ui->cb_VehicleRoutingAlgorithm->currentText();
    }

    if (ui->btn_dataFromCarrier->isChecked())
    {
        currentAlgorihm = ui->cb_CarrierRoutingAlgorithm->currentText();

        if (currentAlgorihm == "Ли")
        {
            Li::CreatePath(map, start, finish, path);
            color = Qt::white;
            wastedTime = Li::wastedTime;
        }

        if (currentAlgorihm == "Дийкстры")
        {
            A_Star::CreatePath(map, start, finish, path);
            color = Qt::lightGray;
            wastedTime = A_Star::wastedTime;
        }

        if (currentAlgorihm == "D*")
        {
            D_Lite::SetMap(map);
            D_Lite::SetStartPoint(start);
            D_Lite::SetFinishPoint(finish);
            D_Lite::ComputeShortestPath();
            D_Lite::CreatePath(path);
            D_Lite::Reset();
            wastedTime = D_Lite::wastedTime;
        }
    }

    ui->l_wastedTime->setText(QString::number(wastedTime));
    ui->map->AddPath(path, color);
}

void MainWindow::on_btn_deletePath_clicked()
{
    ui->map->DeleteSelectedPath();
}

void MainWindow::on_btn_deleteAllPaths_clicked()
{
    ui->map->DeleteAllPaths();
}

void MainWindow::on_sb_GridStep_valueChanged(int arg1)
{
    ui->map->SetResolution(arg1);
}

void MainWindow::on_btn_routeBuildingStep_clicked()
{
//    ui->map->auv->UpdateSonarData();
//    double *d = ui->map->auv->sonar_data;

//    for (int i = 0; i < Vehicle::sonar_numberOfBeams; i++)
//    {
//        qDebug() << *(d + i);
//    }

    //ui->map->auv->SetCourseAngle(90);
    //ui->map->auv->MoveForward(5);

    ui->map->auv->MoveToPoint(10, 10);
}
