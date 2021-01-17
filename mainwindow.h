#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include "imx6_gpio.h"
#include "type.h"
#include "file_manager.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void test();
    QTimer  *timer_test;
    file_manager *file;
    quint8 counter;

//private slots:
private slots:
//    void on_timer_save_data();
    void on_time_out();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
