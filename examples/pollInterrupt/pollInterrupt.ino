/*!
 * @file pollInterrupt.ino
 * @brief 将IO扩展板的某个引脚设置为中断引脚，读取软件标志位来检测某个引脚的中断是否发生.
 * @n 实验现象：将按钮连接到IO扩展板的某个引脚（例：eGPA0），如果检测到中断标志产生，则串口打印"中断发生！"
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

//将按钮连接到IO扩展板的某个引脚(例：eGPA0)

void setup() {
  Serial.begin(115200);
  
  /*在这里一致等到芯片初始化完成才能退出*/
  while(mcp.begin() != 0){
    Serial.println("Initialization of the chip failed, please confirm that the chip connection is correct!");
    delay(1000);
  }

  /*setInterruptPins函数用于将引脚设置中断引脚，该函数会自动将引脚设置为输入模式
  参数pin 如下参数都是可用的：
  eGPA0  eGPA1  eGPA2  eGPA3  eGPA4  eGPA5  eGPA6  eGPA7
   0    1    2    3    4    5    6    7
  eGPB0  eGPB1  eGPB2  eGPB3  eGPB4  eGPB5  eGPB6  eGPB7
   8    9   10   11   12   13   14   15
  参数mode 如下参数是可用的：将引脚设置为低电平中断（eLowLevel）、高电平中断（eHighLevel）、双边沿跳变中断(eChangeLevel)模式
  */
  mcp.setInterruptPins(/*p = */mcp.eGPA0, /*mode = */mcp.eHighLevel);//数字引脚0(eGPA0)，高电平中断，当引脚0的状态为高电平时产生中断，INTA输出高电平
}

void func(){
  Serial.println("Interruption occurs!");
  mcp.clearInterruptA();//清除端口A的中断
  //mcp.clearInterrupt();//清除端口A和B的所有中断
}

void loop() {
  if(mcp.readInterruptFlag(mcp.eGPA0) == 1){//发生中断
      func();
  }
  delay(1000);
}