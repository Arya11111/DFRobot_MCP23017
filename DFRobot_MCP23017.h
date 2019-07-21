/*!
 * @file DFRobot_MCP23017.h
 * @brief 定义 DFRobot_MCP23017 类的基础结构
 * @n 这是一个数字I/O扩展板，IIC地址可改变,可以通过IIC口来控制它，它有下面这些功能
 * @n 16-bit input/output port expander with interrupt output
 * @n Cascadable for up to 8 devices on one bus
 * @n 25mA sink/source capability per I/O
 * @n Supports 100kHz, 400kHz and 1.7MHz I2C™Compatible compatible modes

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
#if 1
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
#define REG_MCP23017_INTCAPA  0x10   //中断捕获寄存器A，该寄存器反映了中断发生时，某引脚的逻辑电平，读取该寄存器可清除中断
#define REG_MCP23017_INTCAPB  0x11   //中断捕获寄存器B，该寄存器反映了中断发生时，某引脚的逻辑电平，读取该寄存器可清除中断
#define REG_MCP23017_GPIOA    0x12   //端口A数据寄存器，可读取或设置该寄存器的值，若引脚被设置为中断模式，可通过读取该寄存器，清除中断
#define REG_MCP23017_GPIOB    0x13   //端口B数据寄存器，可读取或设置该寄存器的值，若引脚被设置为中断模式，可通过读取该寄存器，清除中断
#define REG_MCP23017_OLATA    0x14   //写入锁存寄存器
#define REG_MCP23017_OLATB    0x15   //写入锁存寄存器

typedef void(*MCP23017_INT_CB)(void);
class DFRobot_MCP23017;

typedef struct{
  int intno;
  MCP23017_INT_CB cb;
  String cbDescription;
  DFRobot_MCP23017 *_this;
}sMCP23017Interrupt_t;

class DFRobot_MCP23017{
public:
  #define ERR_OK             0      //无错误
  #define ERR_DATA_BUS      -1      //数据总线错误
  #define ERR_PIN           -2      //引脚编号错误
  #define ERR_DATA_READ     -3      //数据总线读取失败
  #define ERR_ADDR_BUS      -4      //I2C地址错误
  typedef enum{
      eGPA0 = 0,  /**< 模块端口A ，数字引脚GPA0*/
      eGPA1,  /**< 模块端口A ，数字引脚GPA1*/
      eGPA2,  /**< 模块端口A ，数字引脚GPA2*/
      eGPA3,  /**< 模块端口A ，数字引脚GPA3*/
      eGPA4,  /**< 模块端口A ，数字引脚GPA4*/
      eGPA5,  /**< 模块端口A ，数字引脚GPA5*/
      eGPA6,  /**< 模块端口A ，数字引脚GPA6*/
      eGPA7,  /**< 模块端口A ，数字引脚GPA7*/
      eGPB0,  /**< 模块端口B ，数字引脚GPB0*/
      eGPB1,  /**< 模块端口B ，数字引脚GPB1*/
      eGPB2,  /**< 模块端口B ，数字引脚GPB2*/
      eGPB3,  /**< 模块端口B ，数字引脚GPB3*/
      eGPB4,  /**< 模块端口B ，数字引脚GPB4*/
      eGPB5,  /**< 模块端口B ，数字引脚GPB5*/
      eGPB6,  /**< 模块端口B ，数字引脚GPB6*/
      eGPB7,  /**< 模块端口B ，数字引脚GPB7*/
      eGPIOCounter
  }ePin_t;
  
  typedef enum{
      eLowLevel = 0,   /**< 引脚中断配置参数，低电平中断 */
      eHighLevel = 1,  /**< 引脚中断配置参数，高电平中断*/
      eChangLevel = 2 /**< 引脚中断配置参数，双边沿跳变中断*/
  }eInterruptMode_t;
  
  typedef enum{
      eInputMode = 0,  /**< 引脚配置参数，浮空输入模式 最大输入电流25mA*/
      eOutPutMode = 1, /**< 引脚配置参数，输出模式，最大输出电流25mA*/
      ePullUpMode = 2, /**< 引脚配置参数，输入上拉模式，内部上拉电阻为 100 kΩ*/
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
   * @param p 引脚编号，可填ePin_t包含的所有枚举值（eGPA0-eGPB7/ 0-15）
   * @param mode 模式，可设置成eDirMode_t包含的所有模式输入(eInputMode)、输出(eOutPutMode)、上拉输入(ePullUpMode)模式
   * @return 返回0表示设置成功，返回其他值表示设置失败
   */
  int pinMode(ePin_t p, eDirMode_t mode);
  /**
   * @brief 写数字引脚，在写引脚之前，需要将引脚设置为输出模式
   * @param p 引脚编号，可填ePin_t包含的所有枚举值（eGPA0-eGPB7/ 0-15）
   * @param level 高低电平 1(HIGH)或0(LOW)
   * @return 返回0表示设置成功，返回其他值表示写入失败
   */
  int digitalWrite(ePin_t p, uint8_t level);
  /**
   * @brief 读数字引脚，在读引脚之前，需要将引脚设置为输入模式
   * @param p 引脚编号，可填ePin_t包含的所有枚举值（eGPA0-eGPB7/ 0-15）
   * @return 返回高低电平
   */
  int digitalRead(ePin_t p);
  /**
   * @brief 将某个引脚设置为上升沿、下降沿、状态改变中断
   * @param p 引脚编号，可填ePin_t包含的所有枚举值（eGPA0-eGPB7/ 0-15）
   * @param mode 中断方式：低电平中断(eLowLevel)、高电平中断(eHighLevel)、双边沿跳变中断(eChangeLevel)
   * @return 返回0表示设置成功，返回其他值表示写入失败
   */
  void setInterruptPins(ePin_t p, eInterruptMode_t mode);
  /**
   * @brief 清除模块端口A端口B中断标志位
   */
  void clearInterrupt();
  /**
   * @brief 清除模块端口A中断标志位
   */
  void clearInterruptA();
  /**
   * @brief 清除模块端口B中断标志位
   */
  void clearInterruptB();
  /**
   * @brief 读取某个引脚是否发生中断
   * @param p 引脚编号，可填ePin_t包含的所有枚举值（eGPA0-eGPB7/ 0-15）
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
   * @brief 将某个引脚设置为双边沿跳变中断
   * @param index 引脚编号
   * @return 返回0表示设置成功，返回其他值表示设置失败
   */
  int setInterruptModeChangeLevel(uint8_t index);
  /**
   * @brief 将某个引脚设置为高电平中断
   * @param index 引脚编号
   * @return 返回0表示设置成功，返回其他值表示设置失败
   */
  int setInterruptModeHighLevel(uint8_t index);
  /**
   * @brief 将某个引脚设置为低电平中断
   * @param index 引脚编号
   * @return 返回0表示设置成功，返回其他值表示设置失败
   */
  int setInterruptModeLowLevel(uint8_t index);
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