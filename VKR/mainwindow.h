#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_addObstacle_clicked();

    void on_btn_deleteObstacle_clicked();

    void on_btn_deleteAllObstacles_clicked();

    void on_btn_routeBuildingStart_clicked();

    void on_btn_deletePath_clicked();

    void on_btn_deleteAllPaths_clicked();

    void on_sb_GridStep_valueChanged(int arg1);

    void on_btn_routeBuildingStep_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
