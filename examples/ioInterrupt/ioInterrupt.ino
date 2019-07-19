/*!
 * @file ioInterrupt.ino
 * @brief IO中断，将IO扩展板的某个引脚设置为中断模式（高电平中断、低电平中断、状态改变中断(上升沿和下降沿都会发生中断)），当对应端口中断事件发生时INTA或INTB将输出一个高电平
 * INTA用来检测端口DA的引脚是否发生中断,INTB用来检测端口DB的引脚是否发生中断;将INTA和INTB引脚分别连接到UNO的外部中断0和外部中断1
 * @n 实验现象：当端口A的某个引脚发生中断时，调用函数funA，并串口打印"端口A中发生中断"
 * 当端口B的某个引脚发生中断时，调用函数funB，并串口打印"端口B中发生中断"
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

//将2个按钮分别连接到IO扩展板端口DA的某个引脚(例：DA0)和端口DB的某个引脚(例：DB0)
//将INTA连接到UNO的外部中断0，INTB连接到UNO的外部中断1

/*如果你用的是掌控板，请把此宏打开*/
//#define MPYTHON
#ifdef MPYTHON
uint8_t int0 = P0;
uint8_t int1 = P1;
#else
uint8_t int0 = 2;//使用UNO的外部中断0的引脚编号2，若你使用的是其它主控,需将引脚编号3改为相应的引脚编号
uint8_t int1 = 3;//使用UNO的外部中断1的引脚编号3，若你使用的是其它主控,需将引脚编号3改为相应的引脚编号
#endif

void setup() {
  Serial.begin(115200);
  pinMode(int0, INPUT);//使用UNO的外部中断0
  pinMode(int1, INPUT);//使用UNO的外部中断1
  
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
  //mcp23017.setInterruptPins(mcp23017.DA0, mcp23017.LowMode);//数字引脚0(DA0)，低电平中断，当引脚0的状态为低电平时产生中断，INTA输出高电平
  mcp23017.setInterruptPins(mcp23017.DA0, mcp23017.HighMode);//数字引脚0(DA0)，高电平中断，当引脚0的状态为高电平时产生中断，INTA输出高电平
  //mcp23017.setInterruptPins(mcp23017.DA0, mcp23017.ChangeMode);//数字引脚0(DA0)，状态改变电平中断，当引脚0的状态改变时，INTA输出高电平
  
  mcp23017.pinMode(mcp23017.DB0, mcp23017.InputMode);
  //mcp23017.setInterruptPins(mcp23017.DB0, mcp23017.LowMode);//数字引脚8(DB0)，低电平中断，当引脚8的状态为低电平时产生中断，INTB输出高电平
  mcp23017.setInterruptPins(mcp23017.DB0, mcp23017.HighMode);//数字引脚8(DB0)，高电平中断，当引脚8的状态为高电平时产生中断，INTB输出高电平
  //mcp23017.setInterruptPins(mcp23017.DB0, mcp23017.ChangeMode);//数字引脚8(DB0)，状态改变电平中断，当引脚8的状态改变时，INTB输出高电平
#ifndef MPYTHON
  attachInterrupt(0,funA,FALLING);//开启外部中断0,INTA连接至主控的数字引脚上：UNO(2),Mega2560(2),Leonardo(3),microbit(p0)
  attachInterrupt(1,funB,FALLING);//开启外部中断1,INTB连接至主控的数字引脚上：UNO(3),Mega2560(3),Leonardo(2),microbit(p0)
#else
  attachInterrupt(digitalPinToInterrupt(int0),funA,FALLING);//掌控用此中断，INTA连接P0
  attachInterrupt(digitalPinToInterrupt(int1),funB,FALLING);//掌控用此中断，INTB连接P2
#endif
}

void funA(){
  Serial.println("Port A Interrupt!");
}
void funB(){
  Serial.println("Port B Interrupt!");
}

void loop() {
  mcp23017.clearInterrupt();//清除中断
  delay(1000);
}