#include "gpio.h"

gpio::gpio()
{

}

void gpio::gpio_mmap(){
    int fd = open("/dev/mem",O_RDWR | O_SYNC);
    if(fd < 0){
        qDebug() << "Open /dev/mem failed!";
        return;
    }

    csi_pad_length  =   CSI_DATA00_PAD_SIZE;
    csi_pad_phy_addr=   CSI_DATA00_PAD_ADDR;
    if (map_addr(&fd, csi_pad_length, csi_pad_phy_addr, &csi_pad_map_base, &csi_pad_virt_addr) == -1)
    {
        ::close(fd); // qt 的close和系统的close冲突,系统函数使用::来调用
        qDebug() << "mmap failed!";
        exit(0);
    }
    csi_pad_data_reg = (uint32_t *)csi_pad_virt_addr;
    qDebug("csi_pad_data_reg  = %p",csi_pad_data_reg);
    qDebug("*csi_pad_data_reg = %p",*csi_pad_data_reg);
    *csi_pad_data_reg = 0x5;
    qDebug("*csi_pad_data_reg = %p",*csi_pad_data_reg);

    qDebug() << "--------------------------------";

    csi_length      =   CSI_DATA00_SIZE;
    csi_phy_addr    =   CSI_DATA00_ADDR;

    if (map_addr(&fd, csi_length, csi_phy_addr, &csi_map_base, &csi_virt_addr) == -1)
    {
        ::close(fd); // qt 的close和系统的close冲突,系统函数使用::来调用
        qDebug() << "mmap failed!";
        exit(0);
    }

    csi_data_reg = (uint32_t *)csi_virt_addr;
    csi_dir_reg  = ((uint32_t *)csi_virt_addr + 0x1 );
    qDebug("csi_map_base = %p",csi_map_base);
    qDebug("csi_data_reg = %p",csi_data_reg);
    qDebug("csi_dir_reg  = %p",csi_dir_reg);
    qDebug("*csi_dir_reg = 0x%08x",*csi_dir_reg);
    *csi_dir_reg = (1<<21);
    qDebug("*csi_dir_reg = 0x%08x",*csi_dir_reg);
    qDebug("*csi_data_reg = 0x%08x",*csi_data_reg);
    *csi_data_reg = (1<<21);
    qDebug("*csi_data_reg = 0x%08x",*csi_data_reg);
    *csi_data_reg &= ~(1<<21);
    qDebug("*csi_data_reg = 0x%08x",*csi_data_reg);

}

void gpio::gpio_unmmap(){
    munmap(csi_map_base, csi_length);
}

int gpio::map_addr(int *fd, size_t length, off_t phy_addr, void **map_base, void **virt_addr) {
    fflush(stdout);
    off_t pa_offset = phy_addr & ~(sysconf(_SC_PAGE_SIZE) - 1);
    *map_base = mmap(NULL, length + phy_addr - pa_offset, PROT_READ | PROT_WRITE, MAP_SHARED, *fd, pa_offset);
    if (*map_base == (void *) -1) {
        //        close(*fd);
        return -1;
    }
    printf("Memory mapped at address %p.\n", *map_base);
    *virt_addr = *map_base + phy_addr - pa_offset;
    return 0;
}
