#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include "gbt_sca.hpp"
#include "gbt_register_defs.hpp"

// Main loop
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

  tRequest = {2, GPIO, 2, (GPIO_R_DIRECTION)}; // Read GPIO read direction register
  sca.exec_command(tRequest, tReply);
  std::cout << tRequest << std::endl;
  std::cout << tReply << std::endl;
  
  tRequest = {3, GPIO, 1, (GPIO_R_DATAIN)}; // Read GPIO
  sca.exec_command(tRequest, tReply);
  std::cout << tRequest << std::endl;
  std::cout << tReply << std::endl;
  
  uint32_t reply = tReply.data;
  uint32_t convertedData;
  convertedData = ((reply & 0xff)<<24) | ((reply&0xff00)<<8) | ((reply>>8)&0xff00) | ((reply>>24)&0xff);  
  uint32_t boardID = (convertedData>>10) & 0xfff;
  std::cout << "Converted data: 0x" << std::hex << convertedData << std::endl;
  std::cout << "Board ID: 0x" << std::hex << boardID << std::endl;

  // Check GBTx
  i2cCTRL ctrl;// = {1};
  ctrl.NBYTE = 2;
  uint32_t test = 0xd;
  std::cout << " Sizeof(ctrl): " <<  sizeof(ctrl) << "test: "<< std::hex<<test << std::endl;
  //tRequest = {4, I2C0, 2, I2C_W_CTRL, ctrl};
  uint32_t effregaddr;

return 0; 
 
} // main
