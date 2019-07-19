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
  return 0;
}

int DFRobot_MCP23017::pinMode(ePin_t p, eDirMode_t mode){
  uint8_t pin = (uint8_t)p;
  if(pin > 15){
      DBG("pin ERROR!");
      return ERR_PIN;
  }
  DBG(pin);
  DBG(mode);
  switch(mode){
      case InputMode: 
             if(pin < 8){
                 setInput(REG_MCP23017_IODIRA, pin);
             }else{
                 setInput(REG_MCP23017_IODIRB, pin - 8);
             }
             DBG(0);
             break;
      case OutPutMode: 
             if(pin < 8){
                 setOutput(REG_MCP23017_IODIRA, pin);
             }else{
                 setOutput(REG_MCP23017_IODIRB, pin - 8);
             }
             DBG(1);
             break;
      default: 
             if(pin < 8){
                 setInput(REG_MCP23017_IODIRA, pin);
                 setPullUp(REG_MCP23017_GPPUA, pin);
             }else{
                 setInput(REG_MCP23017_IODIRB, pin - 8);
                 setPullUp(REG_MCP23017_GPPUB, pin - 8);
             }
             DBG(2);
             break;
  }
  return ERR_OK;
}

int DFRobot_MCP23017::digitalWrite(ePin_t p, uint8_t level){
  uint8_t pin = (uint8_t)p;
  uint8_t value = 0;
  DBG(pin);
  if(pin < 8){
      if(readReg(REG_MCP23017_OLATA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      DBG(value, HEX);
      value = updateBit(value, pin, level);
      DBG(value, HEX);
      writeReg(REG_MCP23017_GPIOA, &value, 1);
  }else{
      if(readReg(REG_MCP23017_OLATB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      DBG(value, HEX);
      value = updateBit(value, (pin - 8), level);
      DBG(value, HEX);
      writeReg(REG_MCP23017_GPIOB, &value, 1);
  }
  return ERR_OK;
}

int DFRobot_MCP23017::digitalRead(ePin_t p){
  uint8_t pin = (uint8_t)p;
  uint8_t value = 0;
  if(pin < 8){
      readReg(REG_MCP23017_GPIOA, &value, 1);
  }else{
      readReg(REG_MCP23017_GPIOB, &value, 1);
      pin -= 8;
  }
  return (value >> pin)&0x01;
}

void DFRobot_MCP23017::setInterruptPins(ePin_t p, eInterruptMode_t mode){
  uint8_t pin = (uint8_t)p;
  if(pin > 15){
      DBG("PIN ERROR!");
      return ;
  }
  DBG(pin);
  DBG(mode);
  interruptConfig();
  switch(mode){
      case LowMode:
                  setLowMode(pin);
                  DBG(0);
                  break;
      case HighMode:
                  setHighMode(pin);
                  DBG(1);
                  break;
      default:
              setChangeMode(pin);
              DBG(2);
              break;
  }
}

int DFRobot_MCP23017::clearInterrupt(){
  uint8_t value = 0;
  if(readReg(REG_MCP23017_GPIOA, &value, 1) != 1){
      DBG("I2C READ ERROR!");
      return ERR_DATA_READ;
  }
  if(readReg(REG_MCP23017_GPIOB, &value, 1) != 1){
      DBG("I2C READ ERROR!");
      return ERR_DATA_READ;
  }
  return ERR_OK;
}

int DFRobot_MCP23017::readInterruptFlag(ePin_t p){
  uint8_t pin = (uint8_t)p;
  uint8_t regAddr = 0;
  uint8_t value = 0;
  DBG(pin);
  if(pin > 15){
      DBG("I2C READ ERROR!");
      return ERR_PIN;
  }
  if(pin < 8){
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
  return ERR_OK;
}

int DFRobot_MCP23017::setOutput(uint8_t reg, uint8_t index){
  uint8_t value = 0;
  if(readReg(reg, &value, 1) != 1){
      DBG("I2C READ ERROR!");
      return ERR_DATA_READ;
  }
  DBG(value, HEX);
  value = updateBit(value, index, 0);
  DBG(value, HEX);
  writeReg(reg, &value, 1);
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

int DFRobot_MCP23017::setChangeMode(uint8_t index){
  uint8_t value = 0;
  if(index < 8){
      if(readReg(REG_MCP23017_INTCONA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 0);
      writeReg(REG_MCP23017_INTCONA, &value, 1);
      if(readReg(REG_MCP23017_DEFVALA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 0);
      writeReg(REG_MCP23017_DEFVALA, &value, 1);
      if(readReg(REG_MCP23017_GPINTENA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_GPINTENA, &value, 1);
  }else{
      index -= 8;
      if(readReg(REG_MCP23017_INTCONB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 0);
      writeReg(REG_MCP23017_INTCONB, &value, 1);
      if(readReg(REG_MCP23017_DEFVALB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 0);
      writeReg(REG_MCP23017_DEFVALB, &value, 1);
      if(readReg(REG_MCP23017_GPINTENB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_GPINTENB, &value, 1);
  }
  return ERR_OK;
}
int DFRobot_MCP23017::setLowMode(uint8_t index){
  uint8_t value = 0;
  if(index < 8){
      if(readReg(REG_MCP23017_INTCONA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_INTCONA, &value, 1);
      if(readReg(REG_MCP23017_DEFVALA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_DEFVALA, &value, 1);
      if(readReg(REG_MCP23017_GPINTENA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_GPINTENA, &value, 1);
  }else{
      index -= 8;
      if(readReg(REG_MCP23017_INTCONB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_INTCONB, &value, 1);
      if(readReg(REG_MCP23017_DEFVALB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_DEFVALB, &value, 1);
      if(readReg(REG_MCP23017_GPINTENB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_GPINTENB, &value, 1);
  }
  return ERR_OK;
}
int DFRobot_MCP23017::setHighMode(uint8_t index){
  uint8_t value = 0;
  if(index < 8){
      if(readReg(REG_MCP23017_INTCONA, &value, 1) != 1){//中断方式 1
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      DBG(value,HEX);
      value = updateBit(value, index, 1);
      DBG(value,HEX);
      writeReg(REG_MCP23017_INTCONA, &value, 1);
      
      if(readReg(REG_MCP23017_DEFVALA, &value, 1) != 1){//默认比较值
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      DBG(value,HEX);
      value = updateBit(value, index, 0);
      DBG(value,HEX);
      writeReg(REG_MCP23017_DEFVALA, &value, 1);
      
      if(readReg(REG_MCP23017_GPINTENA, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      value = updateBit(value, index, 1);
      DBG(value,HEX);
      writeReg(REG_MCP23017_GPINTENA, &value, 1);
  }else{
      index -= 8;
      DBG(index,HEX);
      if(readReg(REG_MCP23017_INTCONB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      DBG(value,HEX);
      value = updateBit(value, index, 1);
      DBG(value,HEX);
      writeReg(REG_MCP23017_INTCONB, &value, 1);
      if(readReg(REG_MCP23017_DEFVALB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      DBG(value,HEX);
      value = updateBit(value, index, 0);
      DBG(value,HEX);
      writeReg(REG_MCP23017_DEFVALB, &value, 1);
      if(readReg(REG_MCP23017_GPINTENB, &value, 1) != 1){
          DBG("I2C READ ERROR!");
          return ERR_DATA_READ;
      }
      DBG(value,HEX);
      value = updateBit(value, index, 1);
      writeReg(REG_MCP23017_GPINTENB, &value, 1);
      DBG(value,HEX);
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
  DBG(value, HEX);
  value = updateBit(value, 6, 0);
  value = updateBit(value, 2, 0);
  value = updateBit(value, 1, 1);//INTA输出高电平
  DBG(value, HEX);
  writeReg(REG_MCP23017_IOCONA, &value, 1);
  readReg(REG_MCP23017_IOCONA, &value, 1);
  DBG(value, HEX);
  if(readReg(REG_MCP23017_IOCONB, &value, 1) != 1){
      DBG("I2C READ ERROR!");
      return ;
  }
  value = updateBit(value, 6, 0);
  value = updateBit(value, 2, 0);
  value = updateBit(value, 1, 1);//INTB输出高电平
  writeReg(REG_MCP23017_IOCONA, &value, 1);
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
