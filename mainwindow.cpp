#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDateTime datetime;
    QString str = datetime.currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    qDebug() << "Hello IMX " ;
    qDebug() << str;


    /*
    QProcess st;

    QStringList cmdlist;
    cmdlist<<"-s" << m_ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");

    st.start("date",cmdlist);
    system("hwclock -w"); //写入RTC

    system("hwclock -s"); //实现同步
*/

    file = new file_manager;
    file->gpio->power(POWER_OFF);
//    file->gpio->set_freq(FREQ_150K);
//    file->gpio->ad_start();
//    test();
    timer_test = new QTimer(this);
    connect(timer_test,SIGNAL(timeout()),this,SLOT(on_time_out()));
    timer_test->setInterval(5000);
    this->counter = 0;
    timer_test->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_time_out()
{

    this->counter = ++ this->counter % 2;
    if(this->counter == 0){
         qDebug() << "POWER_OFF";
        file->gpio->power(POWER_OFF);
    } else {
        qDebug() << "POWER_ON";
        file->gpio->power(POWER_ON);
    }
}

void MainWindow::test()
{
    for (;;) {

        if(file->file_data_counter == 0 && file->flag_created_file == false){
            file->flag_created_file = file->created_file();
            continue;
            //qDebug() << "close_file success";
        }

        while(file->flag_ready_to_read == false)
        {
            //            qDebug() << "flag_ready_to_read";
            file->flag_ready_to_read = file->ready_to_read();
            //ready_to_read() return true if high that indicate the fifa data is available
        }
        file->read_data_to_save();
        if(file->file_data_counter >= FILE_DATA_MAX_NUMBER){
            file->close_file();
            //            qDebug() << "close_file success";
        }
    }
}
