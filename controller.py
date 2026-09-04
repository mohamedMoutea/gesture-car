import time
import serial


s = serial.Serial("COM8",9600,timeout = 2)

def led(fingerUp):
    if fingerUp==[0,1,0,0,0]:
        s.write(bytes('F','utf-8'))
        time.sleep(0.1)
    elif fingerUp == [0,0,0,0,0]:
        s.write(bytes('S','utf-8'))
        time.sleep(0.1)
    elif fingerUp == [0,1,1,0,0]:
        s.write(bytes('R','utf-8'))
        time.sleep(0.1)
    elif fingerUp == [0,1,1,1,1]:
        s.write(bytes('L','utf-8'))
        time.sleep(0.1)
    elif fingerUp == [1,1,1,1,1]:
        s.write(bytes('B','utf-8'))
        time.sleep(0.1)