#include "imx6_gpio.h"

Imx6Gpio::Imx6Gpio()
{
    this->csi_pad_ctrl_reg  = NULL;
    this->csi_pad_virt_addr = NULL;
    this->csi_pad_map_base  = NULL;

    this->gpio4_reg    = NULL;
    this->gpio4_dir_reg     = NULL;
    this->gpio4_map_base    = NULL;
    this->gpio4_virt_addr   = NULL;

    this->csi_mclk      = NULL;
    this->csi_hsync     = NULL;
    this->csi_vsync     = NULL;
    this->csi_data00    = NULL;
    this->csi_data01    = NULL;
    this->csi_data02    = NULL;
    this->csi_data03    = NULL;
    this->csi_data04    = NULL;
    this->csi_data05    = NULL;
    this->csi_data06    = NULL;
    this->csi_data07    = NULL;

    this->lcd_pad_ctrl_reg  = NULL;
    this->lcd_pad_virt_addr = NULL;
    this->lcd_pad_map_base  = NULL;

    this->gpio3_reg         = NULL;
    this->gpio3_data_reg    = NULL;
    this->gpio3_dir_reg     = NULL;
    this->gpio3_map_base    = NULL;
    this->gpio3_virt_addr   = NULL;

    this->lcd_clk       = NULL;
    this->lcd_hsync     = NULL;
    this->lcd_reset     = NULL;
    this->lcd_vsync     = NULL;
    this->lcd_data00    = NULL;
    this->lcd_data01    = NULL;
    this->lcd_data02    = NULL;
    this->lcd_data03    = NULL;
    this->lcd_data04    = NULL;
    this->lcd_data05    = NULL;
    this->lcd_data06    = NULL;
    this->lcd_data07    = NULL;
    this->lcd_data08    = NULL;
    this->lcd_data09    = NULL;
    this->lcd_data10    = NULL;
    this->lcd_data11    = NULL;
    this->lcd_data12    = NULL;
    this->lcd_data13    = NULL;
    this->lcd_data14    = NULL;
    this->lcd_data15    = NULL;
    this->lcd_data16    = NULL;
    this->lcd_data17    = NULL;
    this->lcd_data18    = NULL;
    this->lcd_data19    = NULL;
    this->lcd_data00    = NULL;
    this->lcd_data21    = NULL;
    this->lcd_data22    = NULL;
    this->lcd_data23    = NULL;

}

void Imx6Gpio::gpio_mmap(){

    int fd = open("/dev/mem",O_RDWR | O_SYNC);
    if(fd < 0){
        qDebug() << "Open /dev/mem failed!";
        return;
    }

    qDebug() << "--------------------------------------------------";
    /********** 配置CSI組引腳 **********/
    qDebug() << "********** Config CSI Group GPIO **********";
    csi_pad_phy_addr=   CSI_PAD_PHY_ADDR;
    csi_pad_length  =   CSI_PAD_PHY_SIZE;
    if (map_addr(&fd, csi_pad_length, csi_pad_phy_addr, &csi_pad_map_base, &csi_pad_virt_addr) == -1){
        ::close(fd); // qt 的close和系统的close冲突,系统函数使用::来调用
        qDebug() << "CSI PAD mmap failed!";
        exit(0);
    }
    csi_pad_ctrl_reg = (uint32_t *)csi_pad_virt_addr;
//    qDebug("csi_pad_ctrl_reg  = %p",csi_pad_ctrl_reg);

    for (uint32_t i = 0; i < (csi_pad_length / 4) ; i ++) {
        *(csi_pad_ctrl_reg + i) = ALT5; // 將該PAD路由至通用GPIO功能引腳
    }

    gpio4_phy_addr    =   GPIO4_PHY_ADDR;
    gpio4_length    =   GPIO4_PHY_SIZE;
    if (map_addr(&fd, gpio4_length, gpio4_phy_addr, &gpio4_map_base, &gpio4_virt_addr) == -1){
        ::close(fd); // qt 的close和系统的close冲突,系统函数使用::来调用
        qDebug() << "GPIO4 mmap failed!";
        exit(0);
    }

    gpio4_reg = (uint32_t *)gpio4_virt_addr;
    gpio4_data_reg = (uint32_t *)((uint32_t)gpio4_virt_addr + 0);
    gpio4_dir_reg = (uint32_t *)((uint32_t)gpio4_virt_addr + 4);

//    qDebug("gpio4_data_reg  = %p",gpio4_data_reg);
//    qDebug("gpio4_dir_reg   = %p",gpio4_dir_reg);

    // 開始配置IO口輸入輸出
    uint32_t tmp = *gpio4_dir_reg;
//    qDebug("gpio4_dir_reg   = 0x%x",*gpio4_dir_reg);
    tmp |=  BIT17;  // CSI_MCLK     - ad_start          - OUTPUT
    tmp &= ~BIT19;  // CSI_VSYNC    - fifo_empty        - INPUT
    tmp |=  BIT20;  // CSI_HSYNC    - POWER             - OUTPUT
    tmp |=  BIT22;  // CSI_DATA01   - rd_clk_n          - OUTPUT
    tmp |=  BIT23;  // CSI_DATA02   - sample_freq[0]    - OUTPUT
    tmp |=  BIT24;  // CSI_DATA03   - sample_freq[1]    - OUTPUT
    tmp |=  BIT25;  // CSI_DATA04   - sample_freq[2]    - OUTPUT
    tmp |=  BIT26;  // CSI_DATA05   - fifo_rd           - OUTPUT
    tmp |=  BIT27;  // CSI_DATA06   - fifo_clear        - OUTPUT
    tmp &= ~BIT28;  // CSI_DATA07   - fifo_half_flag    - INPUT
    *gpio4_dir_reg = tmp;
    //    tmp
//    qDebug("gpio4_dir_reg   = 0x%x",*gpio4_dir_reg);
    qDebug() << "--------------------------------------------------";

    /********** 配置LCD組引腳 **********/
    qDebug() << "********** Config LCD Group GPIO **********";
    lcd_pad_phy_addr=   LCD_PAD_PHY_ADDR;
    lcd_pad_length  =   LCD_PAD_PHY_SIZE;
    if (map_addr(&fd, lcd_pad_length, lcd_pad_phy_addr, &lcd_pad_map_base, &lcd_pad_virt_addr) == -1){
        ::close(fd); // qt 的close和系统的close冲突,系统函数使用::来调用
        qDebug() << "LCD_PAD mmap failed!";
        exit(0);
    }
    lcd_pad_ctrl_reg = (uint32_t *)lcd_pad_virt_addr;
    //    qDebug("lcd_pad_ctrl_reg  = %p",lcd_pad_ctrl_reg);
    //    qDebug("*lcd_pad_ctrl_reg = %p",*lcd_pad_ctrl_reg);

    for (uint32_t i = 0; i < (lcd_pad_length / 4) ; i ++) {
        *(lcd_pad_ctrl_reg + i) = ALT5; // 將該PAD路由至通用GPIO功能引腳
    }
    //    qDebug("*lcd_pad_ctrl_reg = %p",*lcd_pad_ctrl_reg);

    gpio3_phy_addr  =   GPIO3_PHY_ADDR;
    gpio3_length    =   GPIO3_PHY_SIZE;
    if (map_addr(&fd, gpio3_length, gpio3_phy_addr, &gpio3_map_base, &gpio3_virt_addr) == -1){
        ::close(fd); // qt 的close和系统的close冲突,系统函数使用::来调用
        qDebug() << "GPIO3 mmap failed!";
        exit(0);
    }
    qDebug() << "--------------------------------------------------";

    gpio3_reg = (uint32_t *)gpio3_virt_addr;
    gpio3_data_reg = (uint32_t *)((uint32_t)gpio3_virt_addr + 0);
    gpio3_dir_reg = (uint32_t *)((uint32_t)gpio3_virt_addr + 4);
    // 配置爲輸入管教
    // 整個GPIO3組全部爲輸入功能
    *gpio3_dir_reg = 0;

}

void Imx6Gpio::power(bool state)
{
    if(state)
        *gpio4_data_reg |=   POWER;
    else
        *gpio4_data_reg &=  ~POWER;
}

void Imx6Gpio::fifo_clear()
{
    *gpio4_data_reg |=  FIFO_RD;
    *gpio4_data_reg &= ~FIFO_RD;
}

void Imx6Gpio::fifo_rd()
{
    *gpio4_data_reg &= ~FIFO_RD;
}

bool Imx6Gpio::fifo_half_flag()
{
    if( !!(*gpio4_data_reg & FIFO_HALF_FLAG) ){
        return true;
    } else {
        return false;
    }
}

bool Imx6Gpio::fifo_empty()
{
    if( !!(*gpio4_data_reg & FIFO_EMPTY) ){
        // 該位高：說明爲空
        return true;
    } else {
        // 該位低：說明存數據
        return false;
    }
}

void Imx6Gpio::ad_start()
{
    // 清空一次fifo
    this->fifo_clear();
    // 將讀使能線拉低
    this->fifo_rd();
    // 開始啓動AD轉換
    *gpio4_data_reg |= AD_START;
    // 將讀時鍾線拉高
    *gpio4_data_reg |= RD_CLK;
}

void Imx6Gpio::ad_stop()
{
    *gpio4_data_reg &= ~AD_START;
}

void Imx6Gpio::set_freq(quint8 freq)
{
    //    tmp |=  BIT23;  // CSI_DATA02   - sample_freq[0]    - OUTPUT
    //    tmp |=  BIT24;  // CSI_DATA03   - sample_freq[1]    - OUTPUT
    //    tmp |=  BIT25;  // CSI_DATA04   - sample_freq[2]    - OUTPUT
    sample_freq.clear();
    uint32_t tmp = *gpio4_data_reg;
    switch (freq) {
    case FREQ_5K:
        tmp &= ~SAMPLE_FREQ_2;
        tmp &= ~SAMPLE_FREQ_1;
        tmp &= ~SAMPLE_FREQ_0;
        sample_freq.append("_5K");
        qDebug() << "Sample Frequency: 5K";
        break;
    case FREQ_10K:
        tmp &= ~SAMPLE_FREQ_2;
        tmp &= ~SAMPLE_FREQ_1;
        tmp |=  SAMPLE_FREQ_0;
        sample_freq.append("_10K");
        qDebug() << "Sample Frequency: 10K";
        break;
    case FREQ_100K:
        tmp &= ~SAMPLE_FREQ_2;
        tmp |=  SAMPLE_FREQ_1;
        tmp &= ~SAMPLE_FREQ_0;
        sample_freq.append("_100K");
        qDebug() << "Sample Frequency: 100K";
        break;
    case FREQ_150K:
        tmp |=  SAMPLE_FREQ_2;
        tmp &= ~SAMPLE_FREQ_1;
        tmp &= ~SAMPLE_FREQ_0;
        sample_freq.append("_150K");
        qDebug() << "Sample Frequency: 150K";
        break;
    case FREQ_180K:
        tmp |=  SAMPLE_FREQ_2;
        tmp &= ~SAMPLE_FREQ_1;
        tmp |=  SAMPLE_FREQ_0;
        sample_freq.append("_180K");
        qDebug() << "Sample Frequency: 180K";
        break;
    case FREQ_200K:
        tmp |=  SAMPLE_FREQ_2;
        tmp |=  SAMPLE_FREQ_1;
        tmp &= ~SAMPLE_FREQ_0;
        sample_freq.append("_200K");
        qDebug() << "Sample Frequency: 200K";
        break;
    case FREQ_225K:
        tmp |=  SAMPLE_FREQ_2;
        tmp |=  SAMPLE_FREQ_1;
        tmp |=  SAMPLE_FREQ_0;
        sample_freq.append("_225K");
        qDebug() << "Sample Frequency: 225K";
        break;
    default:
        qDebug() << "Sample Frequency Failed, Please send the correct argument";
        break;
    }
    *gpio4_data_reg = tmp;
}

uint32_t Imx6Gpio::read_ad_data(){

    uint32_t tmp_data = 0;
    *gpio4_data_reg |=  FIFO_RD;
    *gpio4_data_reg &= ~RD_CLK;
//    qDebug("%p",*gpio3_data_reg);
    tmp_data =   (*gpio3_data_reg & (uint32_t)0b00000000000000000000000000000011);
    tmp_data |= ((*gpio3_data_reg & (uint32_t)0b00011111111111111110000000000000) >> 11);
    *gpio4_data_reg &= ~FIFO_RD;
    *gpio4_data_reg |=  RD_CLK;
    return tmp_data;
}

void Imx6Gpio::gpio_munmap(){
    if(csi_pad_map_base != NULL)
        munmap(csi_pad_map_base, csi_pad_length);
    if(gpio4_map_base != NULL)
        munmap(gpio4_map_base, gpio4_length);
    if(lcd_pad_map_base != NULL)
        munmap(lcd_pad_map_base, lcd_pad_length);
    if(gpio3_map_base != NULL)
        munmap(gpio3_map_base, gpio3_length);

    csi_pad_map_base    =NULL;
    gpio4_map_base      =NULL;
    lcd_pad_map_base    =NULL;
    gpio3_map_base      =NULL;
}

int Imx6Gpio::map_addr(int *fd, size_t length, off_t phy_addr, void **map_base, void **virt_addr) {

    off_t pa_offset = phy_addr & ~(sysconf(_SC_PAGE_SIZE) - 1);
    *map_base = mmap(NULL, length + phy_addr - pa_offset, PROT_READ | PROT_WRITE, MAP_SHARED, *fd, pa_offset);
    if (*map_base == (void *) -1) {
        //        close(*fd);
        return -1;
    }
    printf("Memory mapped at address %p\n", *map_base);
    *virt_addr = (uint32_t *)((uint32_t)*map_base + phy_addr - pa_offset);
    return 0;
}
