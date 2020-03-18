#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>
#include <QVector>
#include <QPoint>
#include <pathfinderalgorithms.h>


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
//    QVector<QPoint> points;
//    points.append(QPoint(100, 100));
//    points.append(QPoint(1000, 100));

//    ui->map->AddPath(points, Qt::darkBlue);

//    QPoint start;
//    QPoint end;
//    ui->map->UpdateDiscreteMap(ui->map->GetMapDots(&start, &end));

    PathfinderAlgorithms p;

    QPoint start;
    QPoint finish;

    QVector<QVector<int>> map = ui->map->GetMapDots(start, finish);
    QVector<QPoint> path;

    QElapsedTimer E_timer;
    E_timer.start();

    p.A_star(map, start, finish, path);
    long long time = E_timer.elapsed();

    ui->map->AddPath(path, Qt::darkBlue);
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
