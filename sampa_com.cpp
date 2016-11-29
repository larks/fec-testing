#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include "gbt_sca.hpp"

// Move this crap to a header file

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

enum gpioCommands {
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


// I2C Register masks
enum i2cCtrlRegisterMask {
  FREQ    = 0x3,  // 0b00->100kHz, 0b01->200kHz, 0b10->400kHz, 0b11->1MHz
  NBYTE   = 0x7C, // I2C transmission length (num of bytes)
  SCLMODE = 0x80  // 0: open-drain, 1: CMOS output
};
// Use I2C_R_STR to read
enum i2cStatusRegisterMask {
  SUCC   = 0x4,  // Set if last transaction was a success
  LEVERR = 0x8,  // Set if master finds the SDA line pulled low before transaction.
  INVCOM = 0x20, // Set if invalid command was sent to the I2C channel, cleared by channel reset
  NOACK  = 0x40, // Set if last operation has not been acked by the slave ack. Set/reset at end of each transaction.
};
// I2C commands
enum i2cStartOfTransmissionCommands {
  I2C_S_7B_W  = 0x82, // single byte write, 7-bit addressing, LEN=2
  I2C_S_7B_R  = 0x86, // single byte read, 7-bit addressing, LEN=2
  I2C_S_10B_W = 0x8A, // single byte write, 10-bit addressing, LEN=3
  I2C_S_10B_R = 0x8E, // single byte read, 10-bit addressing, LEN=2
  I2C_M_7B_W  = 0xDA, // multi byte write, 7-bit addressing, LEN=1
  I2C_M_7B_R  = 0xDE, // multi byte read, 7-bit addressing, LEN=1
  I2C_M_10B_W = 0xE2, // multi byte write, 10-bit addressing, LEN=2
  I2C_M_10B_R = 0xE6 // multi byte read, 10-bit addressing, LEN=2
//  I2C_RMW_AND = hmmm...
};

enum i2cCommands {
  I2C_W_CTRL = 0x30, // Write CONTROL register
  I2C_R_CTRL = 0x31, // Read CONTROL register 
  I2C_R_STR  = ...
};
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
