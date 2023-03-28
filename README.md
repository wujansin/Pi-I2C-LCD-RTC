# I2C LCD and RTC Demo program
* Compile:

    *gcc -o i2c_lcd_rtc  i2c_lcd_rtc.c -lwiringPi*

---
## I2C LCD
* LCD1602 Pin Connect
    * 5V  (RPI :5V   pin 2)
    * GND (RPI: GND pin 6)
    * SDA (RPI: SDA pin 3)
    * SCL (RPI: SCL pin 5)
* LCD Function
    * void lcd_clear(int lcd_fd) 
    * void lcd_set_cursor(int lcd_fd, int col, int row)
    * void lcd_init(int lcd_fd)
    * void lcd_print_str(int lcd_fd, const char *str) 

---
## I2C RTC Module DS1307
* RTC Pin Connect
    * 5V  (RPI :5V   pin 2)
    * GND (RPI: GND pin 6)
    * SDA (RPI: SDA pin 3)
    * SCL (RPI: SCL pin 5) 
* RTC Function
    * int rtc_read(int rtc_fd, int reg)
    * void rtc_write(int rtc_fd, int reg, int value) 
---
## I2C Base functions
* wiringPi library
    * wiringPiSetup
    * wiringPiI2CSetup
    * wiringPiI2CWrite

---
Edit by *Allan Wu*