#!/usr/bin/env python

import os
import time
import Adafruit_BBIO.GPIO as GPIO

class Tranceiver:
  repeat = 8 #later in config file
  pulselength = 0.000130 # = 130 ms (verified with logic analyser) later in config file
  transmitFIFO = '/tmp/transmitterfifo'
  pin_out ="P8_17" #later in config file

  def __init__(self):
    if os.path.exists(self.transmitFIFO) :
      os.unlink(self.transmitFIFO)
    os.mkfifo(self.transmitFIFO)
    os.chmod(self.transmitFIFO, 0666)
    GPIO.cleanup()
    GPIO.setup(self.pin_out, GPIO.OUT)
    GPIO.output(self.pin_out,GPIO.LOW)

  def transmitter(self):
    while 1:
      with open(self.transmitFIFO,'r') as fifo:
        lines = fifo.read()
        lines = lines.splitlines()
        for line in lines:
          if len(line):
            for bit in line:
              if bit == "1":
                GPIO.output(self.pin_out, GPIO.HIGH)
              else :
                GPIO.output(self.pin_out, GPIO.LOW)
              time.sleep(self.pulselength)
      GPIO.output(self.pin_out, GPIO.LOW)
      time.sleep(0.3)

transmit = Tranceiver()
transmit.transmitter()
