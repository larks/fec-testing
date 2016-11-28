
# testSCA.py
# Test for talking to the GBT-SCA
import sys
import csv # pedestals
import time

# Lloyd stuff
sys.path.append('../tpc-fec-testing')
sys.path.append('../tpc-fec-testing/pyFEC')
import FEC_IO as fec_io


#FEC_IO.py:
#import rorcIO as rorcIO
#import GBTx0cnfg as Cnfg0
#import GBTx1cnfg as Cnfg1
#import sampa_decoder as SParser

# Init FEC instance
fec = fec_io.FEC()
fec.FreqMode = 160

# fec.ReadBoardID(0)
# Does this:
fec.WriteConverted("GPIO_W_DIRECTION", "GPIO", 0x1f)
fec.WriteConverted("GPIO_W_DATAOUT", "GPIO", 0x1f)
val = fec.ReadConverted("GPIO_R_DATAIN", "GPIO")
boardID = (val >>10) & 0xfff

print ("boardID: 0x%x" % boardID)

# Let's try to read config fuse values
#fec.WriteConverted("","",)
valA = fec.ReadConverted("A", "NODE") # read and convert from little endian to big endian
valB = fec.ReadConverted("B", "NODE")
valC = fec.ReadConverted("C", "NODE")
valD = fec.ReadConverted("D", "NODE")  
print("GBT-SCA control register A: 0x%X" % valA)
print("GBT-SCA control register B: 0x%X" % valB) 
print("GBT-SCA control register C: 0x%X" % valC) 
print("GBT-SCA control register D: 0x%X" % valD)

#bitch = fec.ReadConverted(366, "NODE")
#print("configFuseData: 0x%X" % bitch)

#FEC_IO.py:413:       val = self.GBTx_check(0,15)
#   def GBTx_check(self,nodenum,deviceaddr):
#       node = "I2C{}".format(nodenum)
#       val= self.GBT_SCA_I2C_Read(node,deviceaddr,365)
#       return val
valh = fec.GBT_SCA_I2C_Read("I2C0", 15, 349)
print("GBTx_check: 0x%X" %valh)

# Write to cmTestOutSelect
fec.GBT_SCA_I2C_Write("I2C0", 15, 283, 0xFA)
valh = fec.GBT_SCA_I2C_Read("I2C0", 15, 283)
print("cmTestOutSelect: 0x%X" % valh)

# Read GBLD I2C address
valh = fec.GBT_SCA_I2C_Read("I2C0", 15, 253)
print("gbld_ID: 0x%X" % valh)

# Sampa test
#val = fec.SAMPA_read_global_reg("I2C0",0,0x22)

#fec.EnableSampa(1)
fec.WriteConverted("GPIO_W_DATAOUT","GPIO",0x1)

#findact = fec.findActiveSampas(0)
#print findact.msg

import cProfile
import re

# SAMPA_write_pedestal_reg(node,deviceaddr,sampa_chan_addr,sampa_pedestal_addr,val)
clkval = fec.SAMPA_read_global_reg("I2C4",0,0x22)
print("I2C_R_CTRL I2C4: 0x%X" % fec.ReadDirect("I2C_R_CTRL","I2C4"))
startTime = time.time() # time this shit
if clkval != -1: # is clock ok?
  for Channel in range(0, 32):
    for PedestalAddr in range(0, 1024):
        #cProfile.run('fec.SAMPA_write_pedestal_reg("I2C4", 0, Channel, PedestalAddr, PedestalAddr)')
      fec.SAMPA_write_pedestal_reg("I2C4", 0, Channel, PedestalAddr, PedestalAddr)
  print ("Write took %d s" % int(time.time() - startTime))

fec.SAMPA_write_pedestal_reg("I2C4", 0, 5, 231, 0x12)
# readback?
startTime = time.time() # time this shit
print("I2C_R_CTRL I2C4: 0x%X" % fec.ReadDirect("I2C_R_CTRL","I2C4"))
for Channel in range(0,32):
    for PedestalAddr in range(0, 1024):
      PedVal = fec.SAMPA_read_pedestal_reg("I2C4", 0, Channel, PedestalAddr)
      #cProfile.run('PedVal = fec.SAMPA_read_pedestal_reg("I2C4", 0, Channel, PedestalAddr)') # check time spent
      if (PedVal != PedestalAddr):
        print("FEIL Ch: %d PedAddr: %d, Got: 0x%X" %(Channel, PedestalAddr, PedVal))
print ("Read took %d s" % int(time.time() - startTime))   


valh = fec.ReadDirect("I2C_R_STR", "I2C4")
valh = valh & 0xFF
print("I2C status: 0x%X" % valh)
