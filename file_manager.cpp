#include "file_manager.h"

file_manager::file_manager()
{
    gpio = new Imx6Gpio();
    recon = new RECON();
    hs = new HS();
    timer_detect = new QTimer(this);
    datetime = new QDateTime;

    gpio->gpio_mmap();

    flag_timer = true;

    this->flag_created_file = false;
    this->flag_ready_to_read = false;
    this->file_data_counter = 0;
    this->file_path="/run/media/mmcblk0p1/";
    //    this->file_path = "/home/root/work/";

    QFile check_dir(file_path);
    if(check_dir.exists()){
        this->store_location = TF_CARD;
        qDebug() << file_path << " is avaiable";
        qDebug() << "System begin to store data to" << file_path;
    } else {
        this->store_location = EMMC;
        qDebug() << file_path << " is not avaiable";
        this->file_path = "/home/root/work/";
        qDebug() << "System begin to store data to" << file_path;
        qDebug() << "This location storage size maybe not enough to use ";
        QStorageInfo storage = QStorageInfo::root();
        qDebug() << "System total size    :" << storage.bytesTotal()/1000/1000 << "MB";
        qDebug() << "System available size:" << storage.bytesAvailable()/1000/1000 << "MB";
        if(storage.bytesAvailable()/1000/1000 <= 100){
            qDebug() << "***********************************************************";
            qDebug() << "The system availabel size is not enough to store data";
            qDebug() << "The system will be exit";
            qDebug() << "***********************************************************";
            exit(0);
        }
    }
    qDebug() << "--------------------------------------------------";

    memset(recon_data_1,0,sizeof(recon_data_1));
    memset(recon_data_2,0,sizeof(recon_data_2));
    recon_data_counter_1 = 0;
    recon_data_counter_2 = 2047;
    recon_file_data_counter = 0;
    //    myDebug();
    recon_data_switch = 0;

    //    connect((QObject*)this->timer_detect, SIGNAL(timeout()), this, SLOT(on_time_out()));
    //    for (long long i = 0;i<0x1FFFFFF;i++) {
    //        gpio->ad_start();
    //        gpio->ad_stop();
    //    }
    //    //qDebug() << "************************";
    //    gpio->gpio_munmap();
    //    //qDebug() << "gpio_munmap success";
}

file_manager::~file_manager()
{
    qDebug() << "Close";
    delete gpio;
}

bool file_manager::ready_to_read()
{
    return gpio->fifo_half_flag();
    //return true if high that indicate the fifa data is available
}

bool file_manager::read_empty()
{
    return gpio->fifo_empty();
    //return false if low that indicate the fifa data is available
}

void file_manager::read_data_to_save()
{
    uint32_t data=0;
    //qDebug() << "begin write";
    //qDebug("file_ptr = %p",file_ptr);

    do{
        data = gpio->read_ad_data();
        //        qDebug("0x%x",data);
        //        //qDebug("file_ptr = 0x%x  %d",addr,file_data_counter);
        *((char *)file_mmap_addr)     = (uint8_t)( (data & 0xFF0000) >> 16 );
        *((char *)(file_mmap_addr + 1)) = (uint8_t)( (data & 0x00FF00) >> 8 );
        *((char *)(file_mmap_addr + 2)) = (uint8_t)( (data & 0x0000FF) );
#if 1
        recon_data_1[recon_data_counter_1] = (float)data;
        recon_data_2[recon_data_counter_2] = (float)data;

        recon_data_counter_1 = ++ recon_data_counter_1 % 4096;
        recon_data_counter_2 = ++ recon_data_counter_2 % 4096;

        if(recon_data_counter_1 == 2048){
            recon_data_switch = 1;
        } else if(recon_data_counter_1 == 0){
            recon_data_switch = 0;
        } else {
            recon_data_switch = 2;
        }

        if(recon_data_switch == 1 && flag_timer == true){
            int res = hs->recon(recon_data_2,5000);
            hs->clear();
            if( res == 5 ){
                //                qDebug() << recon_data_switch << " " << res;
                flag_timer = false;
                gpio->power(POWER_ON);
                //                qDebug() << "start time_out";
                //                timer_detect->start(1000);
            }
        } else if(recon_data_switch == 0 && flag_timer == true){
            int res = hs->recon(recon_data_1,5000);
            hs->clear();
            if( res == 5 ){
                //                qDebug() << recon_data_switch << " " << res ;
                flag_timer = false;
                gpio->power(POWER_ON);
            }
        }
#endif

        if(flag_timer == false) {
            recon_file_data_counter ++;
        }
        if(recon_file_data_counter >= 150000 - 1){
            //            qDebug() << "POWER OFF";
            gpio->power(POWER_OFF);
            recon_file_data_counter = 0;
            flag_timer = true;
        }
        file_mmap_addr += 3;
        file_data_counter++;
    }while ((file_data_counter % FPGA_FIFO_HALF_LEN != 0) && file_data_counter < FILE_DATA_MAX_NUMBER);
    //    } while (gpio->fifo_empty() == false && file_data_counter < FPGA_FIFO_DATA_LEN);
    //    qDebug() << "No data to read";
    //    qDebug() << file_data_counter;
    flag_ready_to_read = false; // 本次FIFO讀取完畢，等待下一次的讀取
    //qDebug() << "write success";
}

bool file_manager::created_file()
{
    if(this->store_location == EMMC){
        QStorageInfo storage = QStorageInfo::root();
        if(storage.bytesAvailable()/1000/1000 <= 100){
            qDebug() << "***********************************************************";
            qDebug() << "The system availabel size is not enough to store data";
            qDebug() << "The system will be exit";
            qDebug() << "***********************************************************";
            exit(0);
        }
    }
    currenttime = datetime->currentDateTime().toString("yyyyMMddHHmmss");
    //qDebug() << "--------------------------------------";
    //myDebug();
    //qDebug() << file_path + currenttime + ".hex";
    file.setFileName(file_path + currenttime + gpio->sample_freq + ".hex");
    if(file.open(QIODevice::ReadWrite)){
        //qDebug() << "open success";
        if(file.resize(FILE_SIZE)){
            //qDebug() << "resize success" << " " << file.size();
            file_ptr = file.map(0,file.size());
            //qDebug("file_ptr = %p",file_ptr);
            if(file_ptr != NULL){
                //qDebug() << "map success";
                //qDebug("file_ptr = %p",file_ptr);
                file_mmap_addr = (uint32_t)file_ptr;
                file_data_counter = 0;
            }
            //qDebug() << "created_file success";
            return true;
        }
    }
    file.close();
    //    file.deleteLater();
    //qDebug() << "created_file failed";
    return false;
}

bool file_manager::close_file()
{
    //qDebug() << "file_data_counter " << file_data_counter;
    //qDebug() << "begin close_file";
    file.unmap(file_ptr);
    file.close();
    file_mmap_addr = 0;
    file_data_counter = 0;
    flag_created_file = false;
    //flag_ready_to_read = false;
    //qDebug() << "begin close_file success";
    return true;
}
