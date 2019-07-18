/*!
 * @file buttonInput.ino
 * @brief 将IO扩展板的某个引脚(例：DA0)设置为输入模式，并接上按钮，用来检测按钮的状态
 * @n 实验现象：将按钮连接到IO扩展板的某个引脚(例：DA0),检测该引脚的高低电平，并串口打印出按钮的状态：松开、按下
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-07-18
 * @https://github.com/Arya11111/DFRobot_MCP23017
 */
#include <DFRobot_MCP23017.h>

DFRobot_MCP23017 mcp23017(Wire, 0x27);
//DFRobot_MCP23017 mcp23017;//这样定义会使用默认参数， Wire  0x27(默认I2C地址)

//准备：将按钮连接到IO扩展板的某个数字引脚(例：DA0)
void setup() {
  Serial.begin(115200);
  /*在这里一致等到芯片初始化完成才能退出*/
  while(mcp23017.begin() != 0){
    Serial.println("Initialization of the chip failed, please confirm that the chip connection is correct!");
    delay(1000);
  }
  /*pinMode函数用于将引脚设置为输入（INPUT）、输出（OUTPUT）、输入上拉（INPUT_PULLUP）模式，
  可以直接指定扩展板的引脚，如下参数都是可用的：
  DA0  DA1  DA2  DA3  DA4  DA5  DA6  DA7
   0    1    2    3    4    5    6    7
  DB0  DB1  DB2  DB3  DB4  DB5  DB6  DB7
   8    9   10   11   12   13   14   15
  */
  mcp23017.pinMode(mcp23017.DA0, INPUT);
  //mcp23017.pinMode(0, INPUT);//mcp23017.DA0可用0代替
}

void loop() {
  /*digitalRead函数用于读取某个数字引脚的高低电平,在使用该函数之前，必须先将引脚设置为输入模式
  可以直接指定扩展板的引脚，如下参数都是可用的：
  DA0  DA1  DA2  DA3  DA4  DA5  DA6  DA7
   0    1    2    3    4    5    6    7
  DB0  DB1  DB2  DB3  DB4  DB5  DB6  DB7
   8    9   10   11   12   13   14   15
  */
  uint8_t value = mcp23017.digitalRead(mcp23017.DA0);
  //uint8_t value = mcp23017.digitalRead(0);//mcp23017.DA0可用0代替
  if(value){
      Serial.println("Button press!");
  }else{
      Serial.println("Button release!");
  }
  delay(1000);
}