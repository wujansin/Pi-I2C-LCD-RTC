cmake_minimum_required(VERSION 2.6)
project(i2c_lcd_rtc)
add_executable(i2c_lcd_rtc i2c_lcd_rtc.c )
target_link_libraries(i2c_lcd_rtc wiringPi)
