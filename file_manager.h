#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <QFile>
#include <QDateTime>
#include <QThread>
#include <QTimer>
#include <QStorageInfo>
#include "imx6_gpio.h"
#include "RECON.h"
#include "hs.h"

#define     TF_CARD                 true
#define     EMMC                    false

#define     FILE_SIZE               (3*1024*1024LL)
#define     FILE_DATA_MAX_NUMBER    (FILE_SIZE/3)
#define     FPGA_FIFO_HALF_LEN      8192

#define myDebug()    qDebug() << __FILE__ << tr("line:") <<__LINE__ << tr("fun:") << __FUNCTION__

class file_manager: public QThread
{
    Q_OBJECT
public:
    explicit file_manager();
    ~file_manager();

public:
    Imx6Gpio *gpio;
    RECON *recon;
    HS  *hs;
    QTimer  *timer_detect;
    QFile   file;
    uchar   *file_ptr;
    quint32 file_data_counter;
    uint32_t file_mmap_addr;
    bool flag_created_file;
    bool flag_timer;
    volatile bool flag_ready_to_read;
    quint16 recon_data_switch;
    bool    store_location;

private:
    QString currenttime;
    QDateTime *datetime;
    QString file_path;
    float recon_data_1[4096];
    float recon_data_2[4096];
    volatile quint32 recon_data_counter_1;
    volatile quint32 recon_data_counter_2;
    volatile quint32 recon_file_data_counter;



public:
    void read_data_to_save();
    bool ready_to_read();
    bool  created_file();
    bool  close_file();
    bool  read_empty();


};

#endif // FILE_MANAGER_H
