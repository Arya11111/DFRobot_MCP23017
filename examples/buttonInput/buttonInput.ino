/*!
 * @file buttonInput.ino
 * @brief 将IO扩展板的某个引脚(例：eGPA0)设置为输入模式，并接上按钮，用来检测按钮的状态
 * @n 实验现象：将按钮连接到IO扩展板的某个引脚(例：eGPA0),检测该引脚的高低电平，并串口打印出按钮的状态：松开、按下
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-07-18
 * @get from https://www.dfrobot.com
 * @url https://github.com/Arya11111/DFRobot_MCP23017
 */
#include <DFRobot_MCP23017.h>

DFRobot_MCP23017 mcp(Wire, 0x27);//构造函数，地址可通过拨码开关更改A2A1A0的高低电平，实现硬件更改地址，范围0x20~0x27
//DFRobot_MCP23017 mcp;//这样定义会使用默认参数， Wire  0x27(默认I2C地址)

//准备：将按钮连接到IO扩展板的某个数字引脚(例：eGPA0)
void setup() {
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
  mcp.pinMode(mcp.eGPA0, mcp.eInputMode);
}

void loop() {
  /*digitalRead函数用于读取某个数字引脚的高低电平,在使用该函数之前，必须先将引脚设置为输入模式
  参数pin 如下参数都是可用的：
  eGPA0  eGPA1  eGPA2  eGPA3  eGPA4  eGPA5  eGPA6  eGPA7
   0       1      2      3      4      5      6      7
  eGPB0  eGPB1  eGPB2  eGPB3  eGPB4  eGPB5  eGPB6  eGPB7
   8       9      10     11     12     13     14     15
  */
  uint8_t value = mcp.digitalRead(mcp.eGPA0);
  if(value){
      Serial.println("Button press!");
  }else{
      Serial.println("Button release!");
  }
  delay(1000);
}