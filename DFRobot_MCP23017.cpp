/*!
 * @file DFRobot_MCP23017.h
 * @brief 定义 DFRobot_MCP23017 类的基础结构,基础方法的实现
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-07-16
 * @https://github.com/DFRobot/DFRobot_MCP23017
 */
#include <Arduino.h>
#include <DFRobot_MCP23017.h>
#include <string.h>

DFRobot_MCP23017::sPinDescription_t DFRobot_MCP23017::_pinDescriptions[eGPIOTotal]=
{
  {eGPA0,"GPA0"},
  {eGPA1,"GPA1"},
  {eGPA2,"GPA2"},
  {eGPA3,"GPA3"},
  {eGPA4,"GPA4"},
  {eGPA5,"GPA5"},
  {eGPA6,"GPA6"},
  {eGPA7,"GPA7"},
  {eGPB0,"GPB0"},
  {eGPB1,"GPB1"},
  {eGPB2,"GPB2"},
  {eGPB3,"GPB3"},
  {eGPB4,"GPB4"},
  {eGPB5,"GPB5"},
  {eGPB6,"GPB6"},
  {eGPB7,"GPB7"}
};

DFRobot_MCP23017::DFRobot_MCP23017(TwoWire &wire, uint8_t addr)
{
  _addr = addr;
  _pWire = &wire;
  memset(_cbs, 0, sizeof(_cbs));
}

DFRobot_MCP23017::~DFRobot_MCP23017(){

}

int DFRobot_MCP23017::begin(void){
  _pWire->begin();
  if(i2cdetect(_addr) != 0){
      DBG("I2C ADDR ERROR!");
      return ERR_ADDR;
  }
  uint8_t value = 0xff;
  writeReg(REG_MCP23017_IODIRA, &value, 1);
  writeReg(REG_MCP23017_IODIRB, &value, 1);
  value = 0x00;
  for(uint8_t ad = 0x02; ad < 0x16; ad++){
     if((ad > REG_MCP23017_GPPUA) && (ad < REG_MCP23017_GPIOA)){
         continue;
     }
     writeReg(ad, &value, 1);
  }
  return 0;
}

String DFRobot_MCP23017::pinDescription(ePin_t pin)
{
  for(int i=0; i<sizeof(_pinDescriptions)/sizeof(_pinDescriptions[i]); i++){
    if(pin == (uint8_t)_pinDescriptions[i].pin){
      return _pinDescriptions[i].description;
    }
  }
  return "";
}

String DFRobot_MCP23017::pinDescription(int pin){
  for(int i=0; i<sizeof(_pinDescriptions)/sizeof(_pinDescriptions[i]); i++){
    if(pin == (int)_pinDescriptions[i].pin){
      return _pinDescriptions[i].description;
    }
  }
  return "";
}

int DFRobot_MCP23017::pinMode(ePin_t pin, uint8_t mode)
{
  uint8_t _pin = (uint8_t)pin;
  if(_pin >= /*16*/(uint8_t)eGPIOTotal){
      DBG("pin ERROR!");
      return ERR_PIN;
  }
  switch(mode){
      case INPUT: 
             DBG("INPUT");
             if(_pin < (uint8_t)eGPB0){
                 setInput(REG_MCP23017_IODIRA, _pin);
             }else{
                 setInput(REG_MCP23017_IODIRB, _pin - 8);
             }
             break;
      case OUTPUT:
             DBG("OUTPUT");
             if(_pin < (uint8_t)eGPB0){
                 setOutput(REG_MCP23017_IODIRA, _pin);
             }else{
                 setOutput(REG_MCP23017_IODIRB, _pin - 8);
             }
             break;
      default: 
            DBG("OTHERS");
             if(_pin < (uint8_t)eGPB0){
                 setInput(REG_MCP23017_IODIRA, _pin);
                 setPullUp(REG_MCP23017_GPPUA, _pin);
             }else{
                 setInput(REG_MCP23017_IODIRB, _pin - 8);
                 setPullUp(REG_MCP23017_GPPUB, _pin - 8);
             }
             break;
  }
  return ERR_OK;
}

int DFRobot_MCP23017::digitalWrite(ePin_t pin, uint8_t level){
  uint8_t _pin = (uint8_t)pin;
  if(_pin >= /*15*/(uint8_t)eGPIOTotal){
      DBG("pin ERROR!");
      return ERR_PIN;
  }
  uint8_t value = 0;
  if(_pin < (uint8_t)eGPB0){
      if(readReg(REG_MCP23017_OLATA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      DBG(value,HEX);
      value = updateBit(value, _pin, level);
      writeReg(REG_MCP23017_GPIOA, &value, 1);
      if(readReg(REG_MCP23017_OLATA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      DBG(value,HEX);
  }else{
      if(readReg(REG_MCP23017_OLATB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      DBG(value,HEX);
      value = updateBit(value, (_pin - 8), level);
      writeReg(REG_MCP23017_GPIOB, &value, 1);
      if(readReg(REG_MCP23017_OLATB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      DBG(value,HEX);

  }
  return ERR_OK;
}

int DFRobot_MCP23017::digitalRead(ePin_t pin){
  uint8_t _pin = (uint8_t)pin;
  uint8_t value = 0;
  if(_pin >= /*15*/(uint8_t)eGPIOTotal){
      DBG("pin ERROR!");
      return ERR_PIN;
  }
  if(_pin < (uint8_t)eGPB0){
      readReg(REG_MCP23017_GPIOA, &value, 1);
  }else{
      readReg(REG_MCP23017_GPIOB, &value, 1);
      _pin -= 8;
  }
  DBG("pin=");DBG(_pin);DBG("v=");DBG(value,HEX);
  return (value >> _pin)&0x01;
}

void DFRobot_MCP23017::pollInterrupts(eGPIOGrout_t group)
{
  uint8_t value;
  if(group & eGPIOA){
    if(readReg(REG_MCP23017_INTFA, &value, 1) != 1){
      DBG("I2C READ ERROR!");
      return;
    }
    DBGI("INTFA=");DBGI(value,HEX);
    if(value != 0){
      uint8_t level;
      readReg(REG_MCP23017_GPIOA, &level, 1);
      for(int i=0; i<8; i++){
        if((value & (1<<i)) && ( _cbs[i].cb!=NULL )){
          uint8_t l = (level>>i)&0x01;
          if(((_cbs[i].mode == eRising) && (!l)) || ((_cbs[i].mode == eFalling) & l)){
            continue;
          }
          _cbs[i].cb(i);
        }
      }
      if(readReg(REG_MCP23017_INTCAPA, &value, 1) != 1){
        DBG("I2C READ ERROR!");
        return;
      }
    }
  }
  if(group & eGPIOB){
    if(readReg(REG_MCP23017_INTFB, &value, 1) != 1){
      DBG("I2C READ ERROR!");
      return ;
    }
    DBGI("INTFB=");DBGI(value,HEX);
    if(value != 0){
      uint8_t level;
      readReg(REG_MCP23017_GPIOB, &level, 1);
      for(int i=0; i<8; i++){
        if((value & (1<<i)) && (_cbs[i+8].cb!=NULL)){
          uint8_t l = (level>>i)&0x01;
          if(((_cbs[i+8].mode == eRising) && (!l)) || ((_cbs[i+8].mode == eFalling) & l)){
            continue;
          }
          _cbs[i+8].cb(i+8);
        }
      }
      if(readReg(REG_MCP23017_INTCAPB, &value, 1) != 1){
        DBG("I2C READ ERROR!");
        return;
      }
    }
  }
}

void DFRobot_MCP23017::pinModeInterrupt(ePin_t pin, eInterruptMode_t mode, MCP23017_INT_CB cb){
  uint8_t _pin = (uint8_t)pin;
  if(_pin >= /*16*/(uint8_t)eGPIOTotal){
      DBGI("PIN ERROR!");
      return ;
  }
  if(cb == NULL){
      DBGI("null pointer!");
      return ;
  }
  pinMode(pin, INPUT);
  interruptConfig();
  _cbs[_pin].cb = cb;
  _cbs[_pin].mode = mode;

  switch(mode){
    case eLowLevel:
      setInterruptModeLowLevel(_pin);
      break;
    case eHighLevel:
      setInterruptModeHighLevel(_pin);
      break;
    default:
      setInterruptModeChangeLevel(_pin);
      break;
  }
}

int DFRobot_MCP23017::i2cdetect(uint8_t addr){
  _pWire->beginTransmission(addr);
  if(_pWire->endTransmission() == 0){
      DBG("Addr ok!");
      return  ERR_OK;
  }
  return ERR_ADDR;
}

int DFRobot_MCP23017::setInput(uint8_t reg, uint8_t index){
  uint8_t data = 0;
  if(readReg(reg, &data, 1) != 1){
      DBG("I2C READ ERROR!");
      return ERR_DATA_READ;
  }
  DBG(data,HEX);
  data = updateBit(data, index, 1);
  writeReg(reg, &data, 1);
  if(readReg(reg, &data, 1) != 1){
      DBG("I2C READ ERROR!");
      return ERR_DATA_READ;
  }
  DBG(data,HEX);
  return ERR_OK;
}

int DFRobot_MCP23017::setOutput(uint8_t reg, uint8_t index){
  uint8_t value = 0;
  if(readReg(reg, &value, 1) != 1){
      DBG("I2C READ ERROR!");
      return ERR_DATA_READ;
  }
  DBG(value,HEX);
  value = updateBit(value, index, 0);
  writeReg(reg, &value, 1);
  if(readReg(reg, &value, 1) != 1){
      DBG("I2C READ ERROR!");
      return ERR_DATA_READ;
  }
  DBG(value,HEX);
  return ERR_OK;
}
int DFRobot_MCP23017::setPullUp(uint8_t reg, uint8_t index){
  uint8_t value = 0;
  if(readReg(reg, &value, 1) != 1){
      DBG("I2C READ ERROR!");
      return ERR_DATA_READ;
  }
  value = updateBit(value, index, 1);
  writeReg(reg, &value, 1);
  return ERR_OK;
}

int DFRobot_MCP23017::setInterruptModeChangeLevel(uint8_t index){
  uint8_t value = 0;
  if(index < (uint8_t)eGPB0){
      if(readReg(REG_MCP23017_INTCONA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 0);
      DBG("INTCONA：");DBG(value,HEX);
      writeReg(REG_MCP23017_INTCONA, &value, 1);
      if(readReg(REG_MCP23017_DEFVALA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 0);
      writeReg(REG_MCP23017_DEFVALA, &value, 1);
      DBG("DEFVALA：");DBG(value,HEX);
      if(readReg(REG_MCP23017_GPINTENA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_GPINTENA, &value, 1);
      DBG("GPINTENA：");DBG(value,HEX);
  }else{
      index -= 8;
      if(readReg(REG_MCP23017_INTCONB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 0);
      writeReg(REG_MCP23017_INTCONB, &value, 1);
      DBG("INTCONB：");DBG(value,HEX);
      if(readReg(REG_MCP23017_DEFVALB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 0);
      writeReg(REG_MCP23017_DEFVALB, &value, 1);
      DBG("DEFVALB：");DBG(value,HEX);
      if(readReg(REG_MCP23017_GPINTENB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_GPINTENB, &value, 1);
      DBG("GPINTENB：");DBG(value,HEX);
  }
  return ERR_OK;
}
int DFRobot_MCP23017::setInterruptModeLowLevel(uint8_t index){
  uint8_t value = 0;
  if(index < (uint8_t)eGPB0){
      if(readReg(REG_MCP23017_INTCONA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_INTCONA, &value, 1);
      DBG("INTCONA：");DBG(value,HEX);
      DBG("DEFVALA：");DBG(value,HEX);
      if(readReg(REG_MCP23017_DEFVALA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_DEFVALA, &value, 1);
      DBG("DEFVALA：");DBG(value,HEX);
      if(readReg(REG_MCP23017_GPINTENA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_GPINTENA, &value, 1);
      DBG("GPINTENA：");DBG(value,HEX);
  }else{
      index -= 8;
      if(readReg(REG_MCP23017_INTCONB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_INTCONB, &value, 1);
      DBG("INTCONA：");DBG(value,HEX);
      if(readReg(REG_MCP23017_DEFVALB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_DEFVALB, &value, 1);
      DBG("DEFVALB：");DBG(value,HEX);
      if(readReg(REG_MCP23017_GPINTENB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_GPINTENB, &value, 1);
      DBG("GPINTENB：");DBG(value,HEX);
  }
  return ERR_OK;
}
int DFRobot_MCP23017::setInterruptModeHighLevel(uint8_t index){
  uint8_t value = 0;
  if(index < (uint8_t)eGPB0){
      if(readReg(REG_MCP23017_INTCONA, &value, 1) != 1){//中断控制，将引脚设置为中断
          DBGI("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      DBGI("INTCONA=");DBGI(value,HEX);
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_INTCONA, &value, 1);
      if(readReg(REG_MCP23017_INTCONA, &value, 1) != 1){//读取中断控制寄存器
          DBGI("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      DBGI("INTCONA=");DBGI(value,HEX);
      if(readReg(REG_MCP23017_DEFVALA, &value, 1) != 1){//默认比较值
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      DBGI("DEFVALA=");DBGI(value,HEX);
      value = updateBit(value, index, 0);
      writeReg(REG_MCP23017_DEFVALA, &value, 1);
      
      if(readReg(REG_MCP23017_DEFVALA, &value, 1) != 1){//默认比较值
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      DBGI("DEFVALA=");DBGI(value,HEX);
      if(readReg(REG_MCP23017_GPINTENA, &value, 1) != 1){
          DBGI("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      DBGI("GPINTENA=");DBGI(value,HEX);
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_GPINTENA, &value, 1);
      if(readReg(REG_MCP23017_GPINTENA, &value, 1) != 1){
          DBGI("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      DBGI("GPINTENA=");DBGI(value,HEX);
  }else{
      index -= 8;
      if(readReg(REG_MCP23017_INTCONB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_INTCONB, &value, 1);
      DBG("INTCONA：");DBG(value,HEX);
      if(readReg(REG_MCP23017_DEFVALB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 0);
      writeReg(REG_MCP23017_DEFVALB, &value, 1);
      DBG("DEFVALB：");DBG(value,HEX);
      if(readReg(REG_MCP23017_GPINTENB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_GPINTENB, &value, 1);
      DBG("GPINTENB：");DBG(value,HEX);
  }
  return ERR_OK;
}

uint8_t DFRobot_MCP23017::updateBit(uint8_t val, uint8_t pin, uint8_t level){
  uint8_t value = val;
  if(level){//置1
      value |= (1 << pin);
  }else{//置0  1左移pin位按位取反
      value &= (~(1 << pin));
  }
  return value;
}

void DFRobot_MCP23017::interruptConfig(){
  sIOCON_t iocon={0};
  if(readReg(REG_MCP23017_IOCONA, &iocon, 1) != 1){
      DBG("I2C READ ERROR!");
      return ;
  }
  iocon.INTPOL = 1;
  iocon.ODR = 0;
  iocon.MIRROR = 0;
  DBGI("IOCONA：");DBGI(*((uint8_t*)&iocon), HEX);
  writeReg(REG_MCP23017_IOCONA, &iocon, 1);
  if(readReg(REG_MCP23017_IOCONB, &iocon, 1) != 1){
      DBGI("I2C READ ERROR!");
      return ;
  }
  iocon.INTPOL = 1;
  iocon.ODR = 0;
  iocon.MIRROR = 0;
  writeReg(REG_MCP23017_IOCONA, &iocon, 1);
  DBGI("IOCONB：");DBGI(*((uint8_t*)&iocon), HEX);
}

void DFRobot_MCP23017::writeReg(uint8_t reg, const void* pBuf, size_t size){
  if(pBuf == NULL){
      DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  _pWire->beginTransmission(_addr);
  _pWire->write(&reg, 1);

  for(uint16_t i = 0; i < size; i++){
    _pWire->write(_pBuf[i]);
  }
  _pWire->endTransmission();
}

uint8_t DFRobot_MCP23017::readReg(uint8_t reg, void* pBuf, size_t size){
  if(pBuf == NULL){
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  _pWire->beginTransmission(_addr);
  _pWire->write(&reg, 1);
  if( _pWire->endTransmission() != 0){
      return 0;
  }
  _pWire->requestFrom(_addr, (uint8_t) size);
  for(uint16_t i = 0; i < size; i++){
    _pBuf[i] = _pWire->read();
  }
  _pWire->endTransmission();
  return size;
}
