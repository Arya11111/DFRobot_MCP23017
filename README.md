# DFRobot_MCP23017
这是一款16位数字IO口扩展板，通过I2C接口与主控通信，可以连接并操控各种数字传感器件（如：按钮）和数字执行器件（如：LED灯）。<br>

这里需要显示拍照图片，可以一张图片，可以多张图片（不要用SVG图）

![正反面svg效果图](https://github.com/ouki-wang/DFRobot_Sensor/raw/master/resources/images/SEN0245svg1.png)


## 产品链接（链接到英文商城）
    DFR0626：Gravity: MCP23017 IIC to 16 digital IO expansion module
   
## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary

1.设置引脚输入、输出、上拉输入模式；
2.读取引脚高低电平状态；
3.设置引脚输出高低电平；
4.中断方式：高电平中断、低电平中断、状态改变中断
5.INTA和INTB信号引脚中断，当端口DA或端口DB的某些引脚发生中断时，相应的INTA和INTB将产生高电平信号；
6.软中断，将某个引脚设置为中断模式，当中断发生时，可通过软件读取中断是否发生。

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
/**
 * @brief 初始化函数
 * @return 返回0表示初始化成功，返回其他值表示初始化失败
 */
int begin(void);
/**
 * @brief 设置引脚模式，将其配置为输入、输出或上拉输入模式
 * @param pin_ 引脚编号，可填0~15
 * @param mode 模式，可设置成输入(1)、输出(0)、上拉输入(2)模式
 * @return 返回0表示设置成功，返回其他值表示设置失败
 */
int pinMode(ePin_t pin_, uint8_t mode);
/**
 * @brief 写数字引脚，在写引脚之前，需要将引脚设置为输出模式
 * @param pin_ 引脚编号，可填0~15
 * @param level 高低电平 1(HIGH)或0(LOW)
 * @return 返回0表示设置成功，返回其他值表示写入失败
 */
int digitalWrite(ePin_t pin_, uint8_t level);
/**
 * @brief 读数字引脚，在读引脚之前，需要将引脚设置为输入模式
 * @param pin_ 引脚编号，可填0~15
 * @return 返回高低电平
 */
int digitalRead(ePin_t pin_);
/**
 * @brief 将某个引脚设置为上升沿、下降沿、状态改变中断
 * @param pin_ 引脚编号，可填0~15
 * @param mode_ 中断方式：低电平中断(LowMode)、高电平中断(HighMode)、状态改变中断(ChangeMode)
 * @return 返回0表示设置成功，返回其他值表示写入失败
 */
void setInterruptPins(ePin_t pin_, eInterruptMode_t mode_);
/**
 * @brief 清除端口DA和端口DB中断标志位
 * @return 返回0表示清除成功，返回其他值表示写入失败
 */
int clearInterrupt();
/**
 * @brief 将某个引脚设置为上升沿、下降沿、状态改变中断
 * @param pin_ 引脚编号，可填0~15 
 * @param mode_ 中断方式：低电平中断(LowMode)、高电平中断(HighMode)、状态改变中断(ChangeMode)
 * @return 返回1表示发生中断，返回0表示无中断发生,返回其他值表示读取失败
 */
int readInterruptFlag(ePin_t pin_);
```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino uno        |      √       |              |             | 
Mega2560        |      √       |              |             | 
Leonardo        |      √       |              |             | 

## History

- data 2019-7-18
- version V1.0


## Credits

Written by(xue.peng@dfrobot.com), 2019. (Welcome to our [website](https://www.dfrobot.com/))





