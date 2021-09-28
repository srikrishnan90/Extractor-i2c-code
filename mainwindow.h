#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <wiringPiI2C.h>
#include <QDebug>
#include "pi2c.h"
#include <QThread>
#include <QTimer>
#define DEVICE_ID 0x18

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


    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();


    void testing();
    void processing();
     //void testing1();




    void on_pushButton_8_clicked();

private:
    Ui::MainWindow *ui;
     QTimer *timer, *timer1;
};

#endif // MAINWINDOW_H
