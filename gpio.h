#ifndef GPIO_H
#define GPIO_H

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
//NET "i_AD_Data" LOC = P118;
//NET "rd_clk_n" LOC = P134;
////NET "rst" LOC = P133;
//NET "rst" LOC = P45;
////
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
 *  CSI_MCLK   : CSI_DATA07 --- GPIO4[17:28]
 *  LCD_CLK    : CLD_DATA23 --- GPIO3[0:28]
 */

#define     CSI_DATA00_PAD_ADDR     0x020E01E4
#define     CSI_DATA00_PAD_SIZE     0x04

#define     CSI_DATA00_DATA07_ADDR     0x020A8000      //IMX6ULRM.pdf page:1198
#define     CSI_DATA00_SIZE     0x1C

//IMX6ULRM.pdf page:1467
#define     CSI_PAD_PHY_ADDR        0x020E01D4          // 01D4 - 0204
#define     CSI_PAD_PHY_SIZE        0x30
//IMX6ULRM.pdf page:1198
#define     GPIO4_PHY_ADDR          0x020A8000          // 8000 - 8020
#define     GPIO4_PHY_SIZE          0x20

class gpio
{
public:
    gpio();

public:
    void gpio_mmap();
    void gpio_unmmap();
    int map_addr(int *fd, size_t length, off_t phy_addr, void **map_base, void **virt_addr);

public:

    // 配置CSI組引腳
    off_t       csi_pad_phy_addr;
    size_t      csi_pad_length;
    void        *csi_pad_virt_addr;
    void        *csi_pad_map_base;
    uint32_t    *csi_pad_data_reg;

    off_t       gpio4_phy_addr;
    size_t      gpio4_length;
    void        *gpio4_virt_addr;
    void        *gpio4_map_base;
    uint32_t    *gpio4_data_reg;
    uint32_t    *gpio4_dir_reg;


    off_t       csi_phy_addr;
    size_t      csi_length;
    void        *csi_virt_addr;
    void        *csi_map_base;
    uint32_t    *csi_data_reg;
    uint32_t    *csi_dir_reg;

};

#endif // GPIO_H
