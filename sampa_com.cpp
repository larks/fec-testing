#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include "gbt_sca.hpp"

// Channel definitions
enum Channel { 
  NODE = 0x0,
  SPI,
  GPIO,
  I2C0, I2C2, I2C3, I2C4, I2C5, I2C6, I2C7, I2C8, I2C9, I2CA, I2CB, I2CC, I2CD, I2CE, I2CF,
  JTAG,
  ADC,
  DAC
};

// Register definitions
enum ControlRegisterB {
  ENSPI  = 0x2,
  ENGPIO = 0x4,
  ENI2C0 = 0x8,
  ENI2C1 = 0x10,
  ENI2C2 = 0x20,
  ENI2C3 = 0x40,
  ENI2C4 = 0x80
};
enum ControlRegisterC {
  ENI2C5  = 0x1,
  ENI2C6  = 0x2,
  ENI2C7  = 0x4,
  ENI2C8  = 0x8,
  ENI2C9  = 0x10,
  ENI2CA  = 0x20,
  ENI2CB  = 0x40,
  ENI2CC  = 0x80
};
enum ControlRegisterD {
  ENI2CD = 0x1,
  ENI2CE = 0x2,
  ENI2CF = 0x4,
  ENJTAG = 0x8,
  ENADC  = 0x10,
  ENDAC  = 0x20
};

enum commandsGPIO {
  GPIO_W_DATAOUT   = 0x10,
  GPIO_R_DATAOUT   = 0x11,
  GPIO_R_DATAIN    = 0x01,
  GPIO_W_DIRECTION = 0x20,
  GPIO_R_DIRECTION = 0x21,
  GPIO_W_INTENABLE = 0x60,
  GPIO_R_INTENABLE = 0x61,
  GPIO_W_INTSEL    = 0x30,
  GPIO_R_INTSEL    = 0x31,
  GPIO_W_INTTRIG   = 0x40,
  GPIO_R_INTTRIG   = 0x41,
  GPIO_W_INTS      = 0x70,
  GPIO_R_INTS      = 0x71,
  GPIO_W_CLKSEL    = 0x80,
  GPIO_R_CLKSEL    = 0x81,
  GPIO_W_EDGESEL   = 0x90,
  GPIO_R_EDGESEL   = 0x91
};



enum 

int main(int argc, char** argv) 
{ 

  const int minor = 0;
  const int channel = 2;

  GbtSca::reqrep_t tRequest;
  GbtSca::reqrep_t tReply;

  Rorc rorc(minor, channel);
  GbtScaRorc sca(rorc);

  sca.enable_core();

  // Enable SPI, GPIO, I2C_0 channel
  // treq.trid = 1;
  // treq.channel = 0x4;
  // treq.length = 2;
  // treq.command = 0x31;
  // treq.data = 0xFF;

  tRequest = {1, NODE, 2, (ENGPIO | ENSPI | ENI2C0)}; // Enable GPIO, SPI, I2C0
  sca.exec_command(tRequest, tReply);
  std::cout << tRequest << std::endl;
  std::cout << tReply << std::endl;
  
  // GBT_SCA_I2C_Read("I2C0", 15, 365)
  tRequest = {}

#if 0
  tRequest = {1, GPIO, 2, GPIO_W_DIRECTION, 0x1f}; // Enable GPIO
  sca.exec_command(tRequest, tReply);
  std::cout << tRequest << std::endl;
  std::cout << tReply << std::endl;
  tRequest = {1, GPIO, 2, GPIO_W_DATAOUT, 0x1f}; // Enable GPIO
  sca.exec_command(tRequest, tReply);
  std::cout << tRequest << std::endl;
  std::cout << tReply << std::endl;
 
  tRequest = {1, GPIO, 4, GPIO_R_DATAIN, 0xFF}; // Enable GPIO
  sca.exec_command(tRequest, tReply);
  uint32_t reply = tReply.data;
  uint32_t convertedData;
  convertedData = ((reply & 0xff)<<24) | ((reply&0xff00)<<8) | ((reply>>8)&0xff00) | ((reply>>24)&0xff);  
  uint32_t boardID = (convertedData>>10) & 0xfff;
  std::cout << tRequest << std::endl;
  std::cout << tReply << std::endl;
  std::cout << "Converted data: " << convertedData << std::endl;
  std::cout << "Board ID: " << boardID << std::endl;
#endif
return 0; 
 
} // main
