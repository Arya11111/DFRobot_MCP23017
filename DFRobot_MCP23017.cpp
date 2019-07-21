#include <Arduino.h>
#include <DFRobot_MCP23017.h>

DFRobot_MCP23017::DFRobot_MCP23017(TwoWire &wire, uint8_t addr)
{
  _addr = addr;
  _pWire = &wire;
}

DFRobot_MCP23017::~DFRobot_MCP23017(){

}

int DFRobot_MCP23017::begin(void){
  DBG("");
  _pWire->begin();
  DBG("");
  if(i2cdetect(_addr) != 0){
      DBG("I2C ADDR ERROR!");
      return ERR_ADDR_BUS;
  }
  uint8_t value = 0x00;
  uint8_t val = 0xff;
  writeReg(REG_MCP23017_IODIRA, &val, 1);
  writeReg(REG_MCP23017_IODIRB, &val, 1);
  for(uint8_t ad = 0x02; ad < 0x16; ad++){
     if((ad > REG_MCP23017_GPPUA) && (ad < REG_MCP23017_GPIOA)){
         continue;
     }
     writeReg(ad, &value, 1);
  }
  return 0;
}

int DFRobot_MCP23017::pinMode(ePin_t p, eDirMode_t mode){
  uint8_t pin = (uint8_t)p;
  if(pin >= /*15*/(uint8_t)eGPIOCounter){
      DBG("pin ERROR!");
      return ERR_PIN;
  }
  switch(mode){
      case eInputMode: 
             if(pin < (uint8_t)eGPB0){
                 setInput(REG_MCP23017_IODIRA, pin);
             }else{
                 setInput(REG_MCP23017_IODIRB, pin - 8);
             }
             DBG("将引脚");DBG(pin);DBG("设置为输入模式");
             break;
      case eOutPutMode: 
             if(pin < (uint8_t)eGPB0){
                 setOutput(REG_MCP23017_IODIRA, pin);
             }else{
                 setOutput(REG_MCP23017_IODIRB, pin - 8);
             }
             DBG("将引脚");DBG(pin);DBG("设置为输出模式");
             break;
      default: 
             if(pin < (uint8_t)eGPB0){
                 setInput(REG_MCP23017_IODIRA, pin);
                 setPullUp(REG_MCP23017_GPPUA, pin);
             }else{
                 setInput(REG_MCP23017_IODIRB, pin - 8);
                 setPullUp(REG_MCP23017_GPPUB, pin - 8);
             }
             DBG("将引脚");DBG(pin);DBG("设置为输入上拉模式");
             break;
  }
  return ERR_OK;
}

int DFRobot_MCP23017::digitalWrite(ePin_t p, uint8_t level){
  uint8_t pin = (uint8_t)p;
  if(pin >= /*15*/(uint8_t)eGPIOCounter){
      DBG("pin ERROR!");
      return ERR_PIN;
  }
  uint8_t value = 0;
  if(pin < (uint8_t)eGPB0){
      if(readReg(REG_MCP23017_OLATA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, pin, level);
      writeReg(REG_MCP23017_GPIOA, &value, 1);
      DBG("向端口A的");DBG(pin);DBG("引脚写入高(1)或低(0)电平: ");DBG(level);
  }else{
      if(readReg(REG_MCP23017_OLATB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, (pin - 8), level);
      writeReg(REG_MCP23017_GPIOB, &value, 1);
      DBG("向端口B的");DBG(pin);DBG("引脚写入高(1)或低(0)电平: ");DBG(level);
  }
  return ERR_OK;
}

int DFRobot_MCP23017::digitalRead(ePin_t p){
  uint8_t pin = (uint8_t)p;
  uint8_t value = 0;
  if(pin >= /*15*/(uint8_t)eGPIOCounter){
      DBG("pin ERROR!");
      return ERR_PIN;
  }
  if(pin < (uint8_t)eGPB0){
      readReg(REG_MCP23017_GPIOA, &value, 1);
      DBG("读取端口A引脚");
  }else{
      readReg(REG_MCP23017_GPIOB, &value, 1);
      pin -= 8;
      DBG("读取端口B引脚");
  }
  DBG(pin);DBG("的高低电平");
  return (value >> pin)&0x01;
}

void DFRobot_MCP23017::setInterruptPins(ePin_t p, eInterruptMode_t mode){
  uint8_t pin = (uint8_t)p;
  if(pin >= /*15*/(uint8_t)eGPIOCounter){
      DBG("PIN ERROR!");
      return ;
  }
  pinMode(p, eInputMode);
  interruptConfig();
  switch(mode){
      case eLowLevel:
                  setInterruptModeLowLevel(pin);
                  DBG("将引脚");DBG(pin);DBG("设置为低电平中断");
                  break;
      case eHighLevel:
                  setInterruptModeHighLevel(pin);
                  DBG("将引脚");DBG(pin);DBG("设置为高电平中断");
                  break;
      default:
              setInterruptModeChangeLevel(pin);
              DBG("将引脚");DBG(pin);DBG("设置为双边沿跳变中断");
              break;
  }
}

void DFRobot_MCP23017::clearInterrupt(){
  DBG("清除端口A和B的所有中断！");
  clearInterruptA();
  clearInterruptB();
}

void DFRobot_MCP23017::clearInterruptA(){
  uint8_t value = 0;
  DBG("清除端口A的中断！");
  if(readReg(REG_MCP23017_INTCAPA, &value, 1) != 1){
      DBG("I2C READ ERROR!");
      return;
  }
  return ERR_OK;
}

void DFRobot_MCP23017::clearInterruptB(){
  uint8_t value = 0;
  DBG("清除端口B的中断！");
  if(readReg(REG_MCP23017_INTCAPB, &value, 1) != 1){
      DBG("I2C READ ERROR!");
      return;
  }
  return ERR_OK;
}

int DFRobot_MCP23017::readInterruptFlag(ePin_t p){
  uint8_t pin = (uint8_t)p;
  uint8_t regAddr = 0;
  uint8_t value = 0;
  DBG("读取引脚")DBG(pin);DBG("是否发生中断，发生中断返回1，否则返回0");
  if(pin >= /*15*/(uint8_t)eGPIOCounter){
      DBG("I2C READ ERROR!");
      return ERR_PIN;
  }
  if(pin < (uint8_t)eGPB0){
      regAddr = REG_MCP23017_INTFA;
  }else{
      pin -= 8;
      regAddr = REG_MCP23017_INTFB;
  }
  if(readReg(regAddr, &value, 1) != 1){
      DBG("I2C READ ERROR!");
      return ERR_DATA_READ;
  }
  if(((value >> pin)&0x01) == 1){
      return 1;
  }
  return 0;
}

int DFRobot_MCP23017::i2cdetect(uint8_t addr){
  _pWire->beginTransmission(addr);
  if(_pWire->endTransmission() == 0){
      DBG("Addr ok!");
      return  ERR_OK;
  }
  return ERR_ADDR_BUS;
}

int DFRobot_MCP23017::setInput(uint8_t reg, uint8_t index){
  uint8_t value = 0;
  if(readReg(reg, &value, 1) != 1){
      DBG("I2C READ ERROR!");
      return ERR_DATA_READ;
  }
  value = updateBit(value, index, 1);
  writeReg(reg, &value, 1);
  DBG("将寄存器")DBG(reg);DBG("的第");DBG(index);DBG("位设置为1输入");
  return ERR_OK;
}

int DFRobot_MCP23017::setOutput(uint8_t reg, uint8_t index){
  uint8_t value = 0;
  if(readReg(reg, &value, 1) != 1){
      DBG("I2C READ ERROR!");
      return ERR_DATA_READ;
  }
  value = updateBit(value, index, 0);
  writeReg(reg, &value, 1);
  DBG("将寄存器")DBG(reg);DBG("的第");DBG(index);DBG("位设置为0输出");
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
  DBG("将寄存器")DBG(reg);DBG("的第");DBG(index);DBG("位设置为1上拉");
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
      DBG("将中断控制寄存器INTCONA设置为：");DBG(value,HEX);
      writeReg(REG_MCP23017_INTCONA, &value, 1);
      if(readReg(REG_MCP23017_DEFVALA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 0);
      writeReg(REG_MCP23017_DEFVALA, &value, 1);
      DBG("将中断默认值寄存器DEFVALA设置为：");DBG(value,HEX);
      if(readReg(REG_MCP23017_GPINTENA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_GPINTENA, &value, 1);
      DBG("将中断使能寄存器GPINTENA设置为：");DBG(value,HEX);
  }else{
      index -= 8;
      if(readReg(REG_MCP23017_INTCONB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 0);
      writeReg(REG_MCP23017_INTCONB, &value, 1);
      DBG("将中断控制寄存器INTCONB设置为：");DBG(value,HEX);
      if(readReg(REG_MCP23017_DEFVALB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 0);
      writeReg(REG_MCP23017_DEFVALB, &value, 1);
      DBG("将中断默认值寄存器DEFVALB设置为：");DBG(value,HEX);
      if(readReg(REG_MCP23017_GPINTENB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_GPINTENB, &value, 1);
      DBG("将中断使能寄存器GPINTENB设置为：");DBG(value,HEX);
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
      DBG("将中断控制寄存器INTCONA设置为：");DBG(value,HEX);
      DBG("将中断默认值寄存器DEFVALA设置为：");DBG(value,HEX);
      if(readReg(REG_MCP23017_DEFVALA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_DEFVALA, &value, 1);
      DBG("将中断默认值寄存器DEFVALA设置为：");DBG(value,HEX);
      if(readReg(REG_MCP23017_GPINTENA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_GPINTENA, &value, 1);
      DBG("将中断使能寄存器GPINTENA设置为：");DBG(value,HEX);
  }else{
      index -= 8;
      if(readReg(REG_MCP23017_INTCONB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_INTCONB, &value, 1);
      DBG("将中断控制寄存器INTCONA设置为：");DBG(value,HEX);
      if(readReg(REG_MCP23017_DEFVALB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_DEFVALB, &value, 1);
      DBG("将中断默认值寄存器DEFVALB设置为：");DBG(value,HEX);
      if(readReg(REG_MCP23017_GPINTENB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_GPINTENB, &value, 1);
      DBG("将中断使能寄存器GPINTENB设置为：");DBG(value,HEX);
  }
  return ERR_OK;
}
int DFRobot_MCP23017::setInterruptModeHighLevel(uint8_t index){
  uint8_t value = 0;
  if(index < (uint8_t)eGPB0){
      if(readReg(REG_MCP23017_INTCONA, &value, 1) != 1){//中断方式 1
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_INTCONA, &value, 1);
      DBG("将中断控制寄存器INTCONA设置为：");DBG(value,HEX);
      if(readReg(REG_MCP23017_DEFVALA, &value, 1) != 1){//默认比较值
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 0);
      writeReg(REG_MCP23017_DEFVALA, &value, 1);
      DBG("将中断默认值寄存器DEFVALA设置为：");DBG(value,HEX);
      if(readReg(REG_MCP23017_GPINTENA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_GPINTENA, &value, 1);
      DBG("将中断使能寄存器GPINTENA设置为：");DBG(value,HEX);
  }else{
      index -= 8;
      if(readReg(REG_MCP23017_INTCONB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_INTCONB, &value, 1);
      DBG("将中断控制寄存器INTCONA设置为：");DBG(value,HEX);
      if(readReg(REG_MCP23017_DEFVALB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 0);
      writeReg(REG_MCP23017_DEFVALB, &value, 1);
      DBG("将中断默认值寄存器DEFVALB设置为：");DBG(value,HEX);
      if(readReg(REG_MCP23017_GPINTENB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_GPINTENB, &value, 1);
      DBG("将中断使能寄存器GPINTENB设置为：");DBG(value,HEX);
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
  uint8_t value = 0;
  if(readReg(REG_MCP23017_IOCONA, &value, 1) != 1){
      DBG("I2C READ ERROR!");
      return ;
  }
  value = updateBit(value, 6, 0);
  value = updateBit(value, 2, 0);
  value = updateBit(value, 1, 1);//INTA输出高电平
  DBG("配置寄存器IOCONA的值为：");DBG(value, HEX);
  writeReg(REG_MCP23017_IOCONA, &value, 1);
  if(readReg(REG_MCP23017_IOCONB, &value, 1) != 1){
      DBG("I2C READ ERROR!");
      return ;
  }
  value = updateBit(value, 6, 0);
  value = updateBit(value, 2, 0);
  value = updateBit(value, 1, 1);//INTB输出高电平
  writeReg(REG_MCP23017_IOCONA, &value, 1);
  DBG("配置IOCONB寄存器的值为：");DBG(value, HEX);
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
