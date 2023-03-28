#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define DS1307_ADDR 0x68
#define LCD1602_ADDR 0x27

#define LCD_RS 0x01
#define LCD_E  0x04
#define LCD_D4 0x10
#define LCD_D5 0x20
#define LCD_D6 0x40
#define LCD_D7 0x80
#define LCD_BACKLIGHT 0x08

int rtc_read(int rtc_fd, int reg) {
    return wiringPiI2CReadReg8(rtc_fd, reg);
}

void rtc_write(int rtc_fd, int reg, int value) {
    wiringPiI2CWriteReg8(rtc_fd, reg, value);
}

int bcd_to_decimal(int bcd) {
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}

int decimal_to_bcd(int decimal) {
    return ((decimal / 10) << 4) + (decimal % 10);
}

void lcd_write_byte(int lcd_fd, int data, int mode) {
    int high_data = mode | (data & 0xF0) | LCD_BACKLIGHT;
    int low_data = mode | ((data << 4) & 0xF0) | LCD_BACKLIGHT;

    wiringPiI2CWrite(lcd_fd, high_data);
    wiringPiI2CWrite(lcd_fd, high_data | LCD_E);
    wiringPiI2CWrite(lcd_fd, high_data);

    wiringPiI2CWrite(lcd_fd, low_data);
    wiringPiI2CWrite(lcd_fd, low_data | LCD_E);
    wiringPiI2CWrite(lcd_fd, low_data);

    delay(2);
}

void lcd_write_command(int lcd_fd, int cmd) {
    lcd_write_byte(lcd_fd, cmd, 0);
}

void lcd_write_data(int lcd_fd, int data) {
    lcd_write_byte(lcd_fd, data, LCD_RS);
}

void lcd_clear(int lcd_fd) {
   
    lcd_write_command(lcd_fd, 0x01);
    delay(2);
}

void lcd_set_cursor(int lcd_fd, int col, int row) {
    int row_offsets[] = {0x00, 0x40};
    lcd_write_command(lcd_fd, 0x80 | (col + row_offsets[row]));
}

void lcd_init(int lcd_fd) {
    delay(50);
    lcd_write_byte(lcd_fd, 0x03, 0);
    lcd_write_byte(lcd_fd, 0x03, 0);
    lcd_write_byte(lcd_fd, 0x03, 0);
    lcd_write_byte(lcd_fd, 0x02, 0);

    lcd_write_command(lcd_fd, 0x28); // Function set: 4-bit, 2 lines, 5x8 dots
    lcd_write_command(lcd_fd, 0x0C); // Display control: Display on, Cursor off, Blink off
    lcd_clear(lcd_fd);
    lcd_write_command(lcd_fd, 0x06); // Entry mode set: Increment cursor, No shift
}

void lcd_print_str(int lcd_fd, const char *str) 
{
    while (*str) {
        lcd_write_data(lcd_fd, *str++);
    }
}

int main() 
{
    if (wiringPiSetup() == -1) 
    {
        printf("WiringPi setup failed!\n");
        return 1;
    }
    printf("WiringPi setup OK!\n");

    int rtc_fd = wiringPiI2CSetup(DS1307_ADDR);
    int lcd_fd = wiringPiI2CSetup(LCD1602_ADDR);

    if (rtc_fd < 0 || lcd_fd < 0) 
    {
        printf("I2C setup failed!\n");
        return 1;
    }
    printf("WiringPi I2C setup OK!\n");

    lcd_init(lcd_fd);

    while (1) 
    {
        int seconds = bcd_to_decimal(rtc_read(rtc_fd, 0x00));
        int minutes = bcd_to_decimal(rtc_read(rtc_fd, 0x01));
        int hours =   bcd_to_decimal(rtc_read(rtc_fd, 0x02));
        int years =   bcd_to_decimal(rtc_read(rtc_fd, 0x03));
        int days =    bcd_to_decimal(rtc_read(rtc_fd, 0x04));
        int mons =    bcd_to_decimal(rtc_read(rtc_fd, 0x05));

        hours = hours+8;
        years = years+20;
        if(hours>24)
        {
            hours = hours-24;
            days = days+1;
        }    
        printf("Time: %02d:%02d:%02d\n", hours, minutes, seconds);
        printf("Day: %02d:%02d:%02d\n", years, mons, days);
        char time_str[9];
        snprintf(time_str, sizeof(time_str), "%02d:%02d:%02d", hours, minutes, seconds);
        char day_str[9];
        snprintf(day_str, sizeof(day_str), "%02d/%02d/%02d", years, mons, days);

        lcd_set_cursor(lcd_fd, 0, 0);
        lcd_print_str(lcd_fd, time_str);
        lcd_set_cursor(lcd_fd, 0, 1);
        lcd_print_str(lcd_fd, day_str);
        delay(1000);
    }

    return 0;
}

