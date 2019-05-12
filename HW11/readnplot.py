#! /usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
import serial

port = serial.Serial("/dev/ttyACM0", baudrate = 115200)
IMU_data = []
RAW = []
MAF = []
IIR = []
FIR = []

def find_between( s, first, last ):
    try:
        start = s.index( first ) + len( first )
        end = s.index( last, start )
        return s[start:end]
    except ValueError:
        return ""

def read():
	global IMU_data,RAW,MAF,IIR,FIR,port
	recv = 0  
	port.write("r")
	port.read(1)
	while ("\x02" not in str(recv)):
		recv = port.read(49)
		# print(repr(recv))
		IMU_data.append(recv)

	del IMU_data[-1]
	print len(IMU_data)
	for i in IMU_data:
		RAW.append(int(find_between(i,"RAW:","MAF:")))
		MAF.append(int(find_between(i,"MAF:","IIR:")))
		IIR.append(float(find_between(i,"IIR:","FIR:")))
		FIR.append(float(find_between(i,"FIR:","\r\n")))
	print(IMU_data[87])
	print(RAW[87])
	print(MAF[87])
	print(IIR[87])
	# print(FIR[87])

def plot():
	global IMU_data,RAW,MAF,IIR,FIR
	count = np.linspace(0,99,100)
	plt.plot(count, RAW, color = 'green', marker = ".", label="Raw z acceleration") 
	plt.plot(count, MAF, color = 'red', marker = ".",label="MAF") 
	plt.plot(count, IIR, color = 'blue', marker = ".",label="IIR") 
	plt.plot(count, FIR, color = 'brown', marker = ".",label="FIR") 
	plt.legend()
	plt.title("Raw vs filtered IMU data") 
	plt.xlabel("Data Count") 
	plt.ylabel("RAW, MAF, IIR")
	plt.grid(color='black', linestyle='-', linewidth=0.5)
	plt.show()

def main():
	read()
	plot()

if __name__ == '__main__':
   	try:
   		main()
   	except KeyboardInterrupt:
   		print("script killed")
   		port.close()