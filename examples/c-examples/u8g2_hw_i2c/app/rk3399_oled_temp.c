/**
 * @brief   在oled上实时显示当前cpu温度
 * @author  Mculover666
 * @date    2022-09-15
 * @note    RK3399平台，oled接在i2c4上
 */

#include <stdio.h>
#include <stdlib.h>
#include <u8g2port.h>

int read_cpu_temp(char *buf, ssize_t len)
{
    int ret;
    FILE *fp;

    if (!buf || !len) {
        return -1;
    }
 
    if ((fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r")) == NULL) {
        printf("open file fail!\n");
        return -1;
    }
 
    ret = fscanf(fp, "%[^\n]", buf);
    if (ret < 0) {
        printf("read fail, ret is %d\n", ret);
        fclose(fp);
        return -1;
    }
    //printf("temp:[%s]\n", buf);
    fclose(fp);
    
    return 0;
}

int main(void)
{
    u8g2_t u8g2;
    char temp_str[10];

    u8g2_Setup_ssd1306_i2c_128x64_noname_f( &u8g2, U8G2_R0, u8x8_byte_arm_linux_hw_i2c, u8x8_arm_linux_gpio_and_delay);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

    while (1) {
        if (read_cpu_temp(temp_str, sizeof(temp_str)) < 0) {
            printf("read_cpu_temp fail!\n");
            break;
        }

        u8g2_ClearBuffer(&u8g2);
        u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);
        u8g2_DrawStr(&u8g2, 0, 16, temp_str);

        u8g2_SendBuffer(&u8g2);

        sleep(1);
    }

    printf("exit!\n");
    return 0;
}
