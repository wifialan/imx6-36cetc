#ifndef IMX6_GPIO_H
#define IMX6_GPIO_H

#include <QMainWindow>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <QDebug>
#include "type.h"

//NET "o_AD_nCS" LOC = P119;
//NET "o_AD_Sclk" LOC = P117;
//NET "o_AD_SDI" LOC = P116;
//NET "sample_freq[2]" LOC = P123;
//NET "sample_freq[1]" LOC = P121;
//NET "sample_freq[0]" LOC = P120;
//NET "clk" LOC = P55;
//NET "ad_start" LOC = P141;
//NET "fifo_empty" LOC = P140;
//NET "fifo_rd" LOC = P139;
//NET "fifo_clear" LOC = P81
//NET "i_AD_Data" LOC = P118;
//NET "rd_clk_n" LOC = P134;
//NET "rst" LOC = P133;
//NET "rst" LOC = P45;
//
//NET "half_flag" LOC = P138;
//NET "fifo_dout[17]" LOC = P105;
//NET "fifo_dout[16]" LOC = P104;
//NET "fifo_dout[15]" LOC = P101;
//NET "fifo_dout[14]" LOC = P100;
//NET "fifo_dout[13]" LOC = P99;
//NET "fifo_dout[12]" LOC = P98;
//NET "fifo_dout[11]" LOC = P97;
//NET "fifo_dout[10]" LOC = P95;
//NET "fifo_dout[9]" LOC = P94;
//NET "fifo_dout[8]" LOC = P93;
//NET "fifo_dout[7]" LOC = P92;
//NET "fifo_dout[6]" LOC = P88;
//NET "fifo_dout[5]" LOC = P87;
//NET "fifo_dout[4]" LOC = P85;
//NET "fifo_dout[3]" LOC = P84;
//NET "fifo_dout[2]" LOC = P83;
//NET "fifo_dout[1]" LOC = P82;
//NET "fifo_dout[0]" LOC = P81;
/*
    tmp |=  BIT17;  // CSI_MCLK     - ad_start          - OUTPUT
    tmp &= ~BIT19;  // CSI_VSYNC    - fifo_empty        - INPUT
    tmp |=  BIT22;  // CSI_DATA01   - rd_clk_n          - OUTPUT
    tmp |=  BIT23;  // CSI_DATA02   - sample_freq[0]    - OUTPUT
    tmp |=  BIT24;  // CSI_DATA03   - sample_freq[1]    - OUTPUT
    tmp |=  BIT25;  // CSI_DATA04   - sample_freq[2]    - OUTPUT
    tmp |=  BIT26;  // CSI_DATA05   - fifo_rd           - OUTPUT
    tmp |=  BIT27;  // CSI_DATA06   - fifo_clear        - OUTPUT
    tmp &= ~BIT28;  // CSI_DATA07   - fifo_half_flag    - INPUT
*/

#define     POWER_ON                true
#define     POWER_OFF               false


#define     AD_START                BIT17
#define     FIFO_EMPTY              BIT19
#define     POWER                   BIT20
#define     RD_CLK                  BIT22
#define     SAMPLE_FREQ_0           BIT23
#define     SAMPLE_FREQ_1           BIT24
#define     SAMPLE_FREQ_2           BIT25
#define     FIFO_RD                 BIT26
#define     FIFO_CLEAR              BIT27
#define     FIFO_HALF_FLAG          BIT28

#define     FREQ_5K                 0
#define     FREQ_10K                1
#define     FREQ_100K               2
#define     FREQ_150K               4
#define     FREQ_180K               5
#define     FREQ_200K               6
#define     FREQ_225K               7



/*
 *  CSI_MCLK   : CSI_DATA07 --- GPIO4[17:28]
 *  LCD_CLK    : CLD_DATA23 --- GPIO3[0:28]
 */

#define     CSI_DATA00_PAD_ADDR     0x020E01E4
#define     CSI_DATA00_PAD_SIZE     0x04

#define     CSI_DATA00_DATA07_ADDR     0x020A8000      //IMX6ULRM.pdf page:1198
#define     CSI_DATA00_SIZE     0x1C

/********** 配置CSI組引腳 **********/
//IMX6ULRM.pdf page:1467
#define     CSI_PAD_PHY_ADDR        0x020E01D4          // 01D4 - 0204
#define     CSI_PAD_PHY_SIZE        0x30
//IMX6ULRM.pdf page:1198
#define     GPIO4_PHY_ADDR          0x020A8000          // 8000 - 8020
#define     GPIO4_PHY_SIZE          0x20

/*
SW_MUX_CTL_PAD_LCD_CLK SW MUX Control
*/
/********** 配置LCD組引腳 **********/
//IMX6ULRM.pdf page:1415
#define     LCD_PAD_PHY_ADDR        0x020E0104          // 0104 - 0178
#define     LCD_PAD_PHY_SIZE        0x74
//IMX6ULRM.pdf page:1198
#define     GPIO3_PHY_ADDR          0x020A4000          // 4000 - 4020
#define     GPIO3_PHY_SIZE          0x20

class Imx6Gpio
{
public:
    Imx6Gpio();

public:
    void gpio_mmap();
    void gpio_munmap();
    int map_addr(int *fd, size_t length, off_t phy_addr, void **map_base, void **virt_addr);
    uint32_t read_ad_data();
    void set_freq(quint8);
    void ad_start();
    void ad_stop();
    void fifo_rd();
    bool fifo_empty();
    bool fifo_half_flag();
    void fifo_clear();
    void power(bool);


public:

    QString sample_freq;

    /********** 配置CSI組引腳 **********/
    // 引腳路由
    off_t       csi_pad_phy_addr;
    size_t      csi_pad_length;
    void        *csi_pad_virt_addr;
    void        *csi_pad_map_base;
    uint32_t    *csi_pad_ctrl_reg;
    // 引腳輸入輸出配置
    off_t       gpio4_phy_addr;
    size_t      gpio4_length;
    void        *gpio4_virt_addr;
    void        *gpio4_map_base;
    uint32_t    *gpio4_reg;
    uint32_t    *gpio4_data_reg;
    uint32_t    *gpio4_dir_reg;

    // 分配引腳

    uint32_t    *csi_mclk;
    uint32_t    *csi_pixclk;
    uint32_t    *csi_vsync;
    uint32_t    *csi_hsync;
    uint32_t    *csi_data00;
    uint32_t    *csi_data01;
    uint32_t    *csi_data02;
    uint32_t    *csi_data03;
    uint32_t    *csi_data04;
    uint32_t    *csi_data05;
    uint32_t    *csi_data06;
    uint32_t    *csi_data07;

    /********** 配置LCD組引腳 **********/
    // 引腳路由
    off_t       lcd_pad_phy_addr;
    size_t      lcd_pad_length;
    void        *lcd_pad_virt_addr;
    void        *lcd_pad_map_base;
    uint32_t    *lcd_pad_ctrl_reg;
    // 引腳輸入輸出配置
    off_t       gpio3_phy_addr;
    size_t      gpio3_length;
    void        *gpio3_virt_addr;
    void        *gpio3_map_base;
    uint32_t    *gpio3_reg;
    uint32_t    *gpio3_data_reg;
    uint32_t    *gpio3_dir_reg;

    // 分配引腳
    uint32_t    *lcd_clk;
    uint32_t    *lcd_enable;
    uint32_t    *lcd_hsync;
    uint32_t    *lcd_vsync;
    uint32_t    *lcd_reset;
    uint32_t    *lcd_data00;
    uint32_t    *lcd_data01;
    uint32_t    *lcd_data02;
    uint32_t    *lcd_data03;
    uint32_t    *lcd_data04;
    uint32_t    *lcd_data05;
    uint32_t    *lcd_data06;
    uint32_t    *lcd_data07;
    uint32_t    *lcd_data08;
    uint32_t    *lcd_data09;
    uint32_t    *lcd_data10;
    uint32_t    *lcd_data11;
    uint32_t    *lcd_data12;
    uint32_t    *lcd_data13;
    uint32_t    *lcd_data14;
    uint32_t    *lcd_data15;
    uint32_t    *lcd_data16;
    uint32_t    *lcd_data17;
    uint32_t    *lcd_data18;
    uint32_t    *lcd_data19;
    uint32_t    *lcd_data20;
    uint32_t    *lcd_data21;
    uint32_t    *lcd_data22;
    uint32_t    *lcd_data23;

};

#endif // IMX6_GPIO_H
