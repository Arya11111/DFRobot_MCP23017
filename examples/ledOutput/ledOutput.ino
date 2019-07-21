/*!
 * @file ledOutput.ino
 * @brief 将IO扩展板的某个引脚(例：eGPA0)设置为输出模式，并输出高低电平
 * @n 实验现象：连在扩展板某个引脚(例：eGPA0)的LED灯先亮1s再灭1s，以此循环
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @eGPAte  2019-07-18
 * @get from https://www.dfrobot.com
 * @url https://github.com/Arya11111/DFRobot_MCP23017
 */

#include <DFRobot_MCP23017.h>

DFRobot_MCP23017 mcp(Wire, 0x27);//构造函数，地址可通过拨码开关更改A2A1A0的高低电平，实现硬件更改地址，范围0x20~0x27
//DFRobot_MCP23017 mcp;//这样定义会使用默认参数， Wire  0x27(默认I2C地址)

//准备：将LED灯连接到IO扩展板的某个数字引脚(例：eGPA0)
void setup(void)
{
  Serial.begin(115200);
  /*在这里一致等到芯片初始化完成才能退出*/
  while(mcp.begin() != 0){
    Serial.println("Initialization of the chip failed, please confirm that the chip connection is correct!");
    delay(1000);
  }
  /*pinMode函数用于这是模块引脚模式
  参数pin 如下参数都是可用的：
  eGPA0  eGPA1  eGPA2  eGPA3  eGPA4  eGPA5  eGPA6  eGPA7
   0       1      2      3      4      5      6      7
  eGPB0  eGPB1  eGPB2  eGPB3  eGPB4  eGPB5  eGPB6  eGPB7
   8       9      10     11     12     13     14     15
  参数mode 如下参数是可用的：将引脚设置为输入（eInputMode）、输出（eOutPutMode）、输入上拉（ePullUpMode）模式
  */
  mcp.pinMode(mcp.eGPA0, mcp.eOutPutMode);
}

void loop(void)
{
  Serial.println("Pin output high level!");
  /*digitalWrite函数用于将引脚输出高电平(HIGH)或低电平(LOW),在使用该函数之前，必须先将引脚设置为输出模式
  可以直接指定扩展板的引脚，如下参数都是可用的：
  eGPA0  eGPA1  eGPA2  eGPA3  eGPA4  eGPA5  eGPA6  eGPA7
   0      1    2    3    4    5    6    7
  eGPB0  eGPB1  eGPB2  eGPB3  eGPB4  eGPB5  eGPB6  eGPB7
   8    9   10   11   12   13   14   15
  */
  mcp.digitalWrite(mcp.eGPA0, HIGH);
  delay(1000);
  Serial.println("Pin output low level!");
  mcp.digitalWrite(mcp.eGPA0, LOW);
  delay(1000);
}