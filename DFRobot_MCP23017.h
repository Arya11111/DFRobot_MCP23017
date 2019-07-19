/*!
 * @file DFRobot_MCP23017.h
 * @brief 定义 DFRobot_MCP23017 类的基础结构
 * @n 这是一个数字I/O扩展板，IIC地址可改变,可以通过IIC口来控制它，它有下面这些功能
 * @n 读取数字IO口的高低电平
 * @n 设置数字IO口的高低电平
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya]
 * @version  V1.0
 * @date  2019-07-16
 * @https://github.com/ouki-wang/DFRobot_Sensor
 */
#ifndef __DFROBOT_MCP23017_H
#define __DFROBOT_MCP23017_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <HardwareSerial.h>
#include <Wire.h>

//定义调试宏，若想打开调试宏可将0改为1，关闭可将1改为0
#if 0
#define DBG(...) {Serial.print("["); Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

#define REG_MCP23017_IODIRA   0x00   //端口A方向寄存器
#define REG_MCP23017_IODIRB   0x01   //端口B方向寄存器
#define REG_MCP23017_IPOLA    0x02   //IO极性寄存器，默认为0，如果设置为1，当引脚检测到高电平时，会被当做低电平写入
#define REG_MCP23017_IPOLB    0x03   //IO极性寄存器，默认为0，如果设置为1，当引脚检测到高电平时，会被当做低电平写入
#define REG_MCP23017_GPINTENA 0x04   //中断使能,对应端口A的8个引脚,每位置0代表失能,置1代表使能(总开关)
#define REG_MCP23017_GPINTENB 0x05   //中断使能,对应端口B的8个引脚,每位置0代表失能,置1代表使能
#define REG_MCP23017_DEFVALA  0x06   //默认值寄存器A，当中断使能时,即INTCONx的值为1，为0，表示高电平中断，1表示低电平中断
#define REG_MCP23017_DEFVALB  0x07   //默认值寄存器B，当中断使能时,及INTCONx的值为1，为0，表示高电平中断，1表示低电平中断
#define REG_MCP23017_INTCONA  0x08   //中断方式 0：上升沿和下升沿中断 1：高低电平中断
#define REG_MCP23017_INTCONB  0x09   //中断方式 0：上升沿和下升沿中断 1：高低电平中断
#define REG_MCP23017_IOCONA   0x0A   //配置寄存器，当IOCONA和IOCONB的第
#define REG_MCP23017_IOCONB   0x0B   //配置寄存器，
#define REG_MCP23017_GPPUA    0x0C   //端口A上拉寄存器
#define REG_MCP23017_GPPUB    0x0D   //端口B上拉寄存器
#define REG_MCP23017_INTFA    0x0E   //中断标志寄存器A，1表示发生中断，0无中断
#define REG_MCP23017_INTFB    0x0F   //中断标志寄存器B，1表示发生中断，0无中断
#define REG_MCP23017_INTCAPA  0x10   //中断捕获寄存器A
#define REG_MCP23017_INTCAPB  0x11   //中断捕获寄存器B
#define REG_MCP23017_GPIOA    0x12   //端口A数据寄存器，可读取或设置该寄存器的值，若引脚被设置为中断模式，可通过读取该寄存器，清除中断
#define REG_MCP23017_GPIOB    0x13   //端口B数据寄存器，可读取或设置该寄存器的值，若引脚被设置为中断模式，可通过读取该寄存器，清除中断
#define REG_MCP23017_OLATA    0x14   //写入锁存寄存器
#define REG_MCP23017_OLATB    0x15   //写入锁存寄存器

class DFRobot_MCP23017{
public:
  #define ERR_OK             0      //无错误
  #define ERR_DATA_BUS      -1      //数据总线错误
  #define ERR_PIN           -2      //引脚编号错误
  #define ERR_DATA_READ     -3      //数据总线读取失败
  #define ERR_ADDR_BUS      -4      //I2C地址错误
  typedef enum{
      DA0 = 0,    //扩展板端口A引脚DA0，数字引脚0
      DA1 = 1,    //扩展板端口A引脚DA1，数字引脚1
      DA2 = 2,    //扩展板端口A引脚DA2，数字引脚2
      DA3 = 3,    //扩展板端口A引脚DA3，数字引脚3
      DA4 = 4,    //扩展板端口A引脚DA4，数字引脚4
      DA5 = 5,    //扩展板端口A引脚DA5，数字引脚5
      DA6 = 6,    //扩展板端口A引脚DA6，数字引脚6
      DA7 = 7,    //扩展板端口A引脚DA7，数字引脚7
      DB0 = 8,    //扩展板端口B引脚DB8，数字引脚8
      DB1 = 9,    //扩展板端口B引脚DB9，数字引脚9
      DB2 = 10,   //扩展板端口B引脚DB10，数字引脚10
      DB3 = 11,   //扩展板端口B引脚DB11，数字引脚11
      DB4 = 12,   //扩展板端口B引脚DB12，数字引脚12
      DB5 = 13,   //扩展板端口B引脚DB13，数字引脚13
      DB6 = 14,   //扩展板端口B引脚DB14，数字引脚14
      DB7 = 15    //扩展板端口B引脚DB15，数字引脚15
  }ePin_t;
  
  typedef enum{
      LowMode = 0,   //低电平中断，当引脚变为低电平时,产生中断
      HighMode = 1,  //高电平中断，当引脚变为高电平时,产生中断
      ChangeMode = 2 //状态改变中断，当引脚电平由低->高或由高->低时,产生中断
  }eInterruptMode_t;
  
  typedef enum{
      InputMode = 0,
      OutPutMode = 1,
      PullUpMode = 2,
  }eDirMode_t;
public:
  /**
   * @brief 构造函数
   * @param pWire I2C总线指针对象，构造设备，可传参数也可不传参数，默认Wire
   * @param addr 8位I2C地址，范围0x20~0x27,可通过拨码开关更改A2A1A0来更改地址，构造设备时，可以指定它的I2C地址，默认0x27
   * 0  0  1  0  | 0  A2 A1 A0
     0  0  1  0  | 0  1  1  1    0x27
     0  0  1  0  | 0  1  1  0    0x26
     0  0  1  0  | 0  1  0  1    0x25
     0  0  1  0  | 0  1  0  0    0x24
     0  0  1  0  | 0  0  1  1    0x23
     0  0  1  0  | 0  0  1  0    0x22
     0  0  1  0  | 0  0  0  1    0x21
     0  0  1  0  | 0  0  0  0    0x20
   */
  DFRobot_MCP23017(TwoWire &wire = Wire, uint8_t addr = 0x27);
  ~DFRobot_MCP23017();
  /**
   * @brief 初始化函数
   * @return 返回0表示初始化成功，返回其他值表示初始化失败
   */
  int begin(void);
  /**
   * @brief 设置引脚模式，将其配置为输入、输出或上拉输入模式
   * @param p 引脚编号，可填0~15
   * @param mode 模式，可设置成输入(InputMode)、输出(OutPutMode)、上拉输入(PullUpMode)模式
   * @return 返回0表示设置成功，返回其他值表示设置失败
   */
  int pinMode(ePin_t p, eDirMode_t mode);
  /**
   * @brief 写数字引脚，在写引脚之前，需要将引脚设置为输出模式
   * @param p 引脚编号，可填0~15
   * @param level 高低电平 1(HIGH)或0(LOW)
   * @return 返回0表示设置成功，返回其他值表示写入失败
   */
  int digitalWrite(ePin_t p, uint8_t level);
  /**
   * @brief 读数字引脚，在读引脚之前，需要将引脚设置为输入模式
   * @param p 引脚编号，可填0~15
   * @return 返回高低电平
   */
  int digitalRead(ePin_t p);
  /**
   * @brief 将某个引脚设置为上升沿、下降沿、状态改变中断
   * @param p 引脚编号，可填0~15
   * @param mode 中断方式：低电平中断(LowMode)、高电平中断(HighMode)、状态改变中断(ChangeMode)
   * @return 返回0表示设置成功，返回其他值表示写入失败
   */
  void setInterruptPins(ePin_t p, eInterruptMode_t mode);
  /**
   * @brief 清除端口DA和端口DB中断标志位
   * @return 返回0表示清除成功，返回其他值表示写入失败
   */
  int clearInterrupt();
  /**
   * @brief 读取某个引脚是否发生中断
   * @param p 引脚编号，可填0~15
   * @return 返回1表示发生中断，返回0表示无中断发生,返回其他值表示读取失败
   */
  int readInterruptFlag(ePin_t p);

protected:
  /**
   * @brief 将某个引脚设置为输入模式
   * @param reg 方向寄存器 REG_MCP23017_IODIRA 或MCP23017_IODIRB
   * @param index 值左移位数
   * @return 返回0表示设置成功，返回其他值表示设置失败
   */
  int setInput(uint8_t reg, uint8_t index);
  /**
   * @brief 将某个引脚设置为输出模式
   * @param reg 方向寄存器 REG_MCP23017_IODIRA 或MCP23017_IODIRB
   * @param index 值左移位数
   * @return 返回0表示设置成功，返回其他值表示设置失败
   */
  int setOutput(uint8_t reg, uint8_t index);
  /**
   * @brief 将某个引脚设置为上拉模式
   * @param reg 上拉寄存器 MCP23017_GPPUA或MCP23017_GPPUB
   * @param index 值左移位数
   * @return 返回0表示设置成功，返回其他值表示设置失败
   */
  int setPullUp(uint8_t reg, uint8_t index);
  /**
   * @brief 将某个引脚设置为状态改变中断
   * @param index 引脚编号
   * @return 返回0表示设置成功，返回其他值表示设置失败
   */
  int setChangeMode(uint8_t index);
  /**
   * @brief 将某个引脚设置为高电平中断
   * @param index 引脚编号
   * @return 返回0表示设置成功，返回其他值表示设置失败
   */
  int setHighMode(uint8_t index);
  /**
   * @brief 将某个引脚设置为低电平中断
   * @param index 引脚编号
   * @return 返回0表示设置成功，返回其他值表示设置失败
   */
  int setLowMode(uint8_t index);
  /**
   * @brief 将一个8比特位数据的指定位置0或置1
   * @param val 8比特数据
   * @param pin 指定位
   * @param level_ 数据位0或1
   * @return 返回修改后的8比特位的数据
   */
  uint8_t updateBit(uint8_t val, uint8_t pin, uint8_t level);
  /**
   * @brief INTA与INTB中断信号引脚配置，当端口DA的某个引脚发生中断时，INTA输出高电平
   * 当端口DB的某个引脚发生中断时，INTB输出高电平
   */
  void interruptConfig();
  /**
   * @brief I2C地址检测
   * @param addr I2C地址
   * @return 返回0表示I2C地址正确，返回其他值表示I2C地址错误
   */
  int i2cdetect(uint8_t addr);
  /**
   * @brief 写寄存器函数
   * @param reg  寄存器地址 8bits
   * @param pBuf 要写入数据的存放缓存
   * @param size 要写入数据的长度
  */
  void writeReg(uint8_t reg, const void* pBuf, size_t size);
  /**
   * @brief 读寄存器函数
   * @param reg  寄存器地址 8bits
   * @param pBuf 要读取数据的存放缓存
   * @param size 要读取数据的长度
   * @return 返回实际读取的长度，返回0表示读取失败
   */
  uint8_t readReg(uint8_t reg, void* pBuf, size_t size);

private:
  TwoWire *_pWire;
  uint8_t _addr;
};
#endif