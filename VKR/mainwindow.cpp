#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>
#include <QVector>
#include <QPoint>
#include <Algorithms/a_star.h>
#include <Algorithms/li.h>


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

    QElapsedTimer timer;
    timer.start();

    QString currentAlgorihm;

    if (ui->btn_dataFromVehicle->isChecked())
    {
        currentAlgorihm = ui->cb_VehicleRoutingAlgorithm->currentText();
    }

    if (ui->btn_dataFromCarrier->isChecked())
    {
        currentAlgorihm = ui->cb_CarrierRoutingAlgorithm->currentText();

        if (currentAlgorihm == "Ли")
            Li::CreatePath(map, start, finish, path);

        if (currentAlgorihm == "Дийкстры")
            A_Star::CreatePath(map, start, finish, path);
    }

    qDebug() << timer.elapsed();

    ui->map->AddPath(path, Qt::white);
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
