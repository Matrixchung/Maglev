# -*- coding: utf-8 -*-
import serial
import time
import binascii

serial1 = serial.Serial('/dev/ttyUSB1',9600)  #/dev/ttyUSB0
if serial1.isOpen() :
    print("open success")
else :
    print("open failed")

def cl(a):
    dat1 = a[0:1]

    if dat1 == 'a':
        dat1 = 10
    elif dat1 == 'b':
        dat1 = 11
    elif dat1 == 'c':
        dat1 = 12
    elif dat1 == 'd':
        dat1 = 13
    elif dat1 == 'e':
        dat1 = 14
    elif dat1 == 'f':
        dat1 = 15
        
    return dat1

def main():
    global serial1
    time.sleep(0.1) 
    num=serial1.inWaiting()
            
    if num: 
        try:   #如果读取的不是十六进制数据--
            data= str(binascii.b2a_hex(serial1.read(num))) #十六进制显示方法2
            if(len(data)>8):
                print(data)
                dat1 = data[6:7]
                dat2 = data[7:8]
                dat3 = data[8:9]
                dat4 = data[9:10]
                #dat5 = data[13:14]
                #print("SW:",int(dat1) * 10 + int(dat2))
                #print("XW:",int(dat3) * 100 + int(dat4) * 10+ int(dat5))
                #SJ = str(int(dat1) * 10 + int(dat2)) + "." + str(int(dat3) * 100 + int(dat4) * 10+ int(dat5))
                jl = (((int(cl(dat1)) * 16) + int(cl(dat2))) * 256) + ((int(cl(dat3)) * 16) + int(cl(dat4)))
                print(jl)
        except: #--则将其作为字符串读取
            pass

while True:
    main()
