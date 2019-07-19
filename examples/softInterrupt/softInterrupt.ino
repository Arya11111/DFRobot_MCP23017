/*!
 * @file softInterrupt.ino
 * @brief 将IO扩展板的某个引脚设置为中断引脚，读取软件标志位来检测某个引脚的中断是否发生.
 * @n 实验现象：将按钮连接到IO扩展板的某个引脚（例：DA0），如果检测到中断标志产生，则串口打印"中断发生！"
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

DFRobot_MCP23017 mcp23017(Wire, 0x27);//构造函数，地址可通过拨码开关更改A2A1A0的高低电平，实现硬件更改地址，范围0x20~0x27
//DFRobot_MCP23017 mcp23017;//这样定义会使用默认参数， Wire  0x27(默认I2C地址)

//将按钮连接到IO扩展板的某个引脚(例：DA0)

void setup() {
  Serial.begin(115200);
  
  /*在这里一致等到芯片初始化完成才能退出*/
  while(mcp23017.begin() != 0){
    Serial.println("Initialization of the chip failed, please confirm that the chip connection is correct!");
    delay(1000);
  }
  /*pinMode函数用于将引脚设置为输入（InputMode）、输出（OutPutMode）、输入上拉（PullUpMode）模式
  可以直接指定扩展板的引脚，如下参数都是可用的：
  DA0  DA1  DA2  DA3  DA4  DA5  DA6  DA7
   0    1    2    3    4    5    6    7
  DB0  DB1  DB2  DB3  DB4  DB5  DB6  DB7
   8    9   10   11   12   13   14   15
  */
  mcp23017.pinMode(mcp23017.DA0, mcp23017.InputMode);
  /*setInterruptPins函数用于将引脚设置中断引脚，可以填低电平中断(LowMode),高电平中断(HighMode),状态改变中断（ChangeMode）
  可以直接指定扩展板的引脚，如下参数都是可用的：
  DA0  DA1  DA2  DA3  DA4  DA5  DA6  DA7
   0    1    2    3    4    5    6    7
  DB0  DB1  DB2  DB3  DB4  DB5  DB6  DB7
   8    9   10   11   12   13   14   15
  */
  mcp23017.setInterruptPins(mcp23017.DA0, mcp23017.HighMode);//数字引脚0(DA0)，高电平中断，当引脚0的状态为高电平时产生中断，INTA输出高电平
}

void func(){
  Serial.println("Interruption occurs!");
}

void loop() {
  if(mcp23017.readInterruptFlag(mcp23017.DA0) == 1){//发生中断
      mcp23017.clearInterrupt();//清除中断
      func();
  }
  delay(1000);
}