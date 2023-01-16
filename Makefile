all: rp

rp: ADXL345.cpp I2CDevice.cpp LED.cpp roll_pitch.cpp
	g++ ADXL345.cpp I2CDevice.cpp LED.cpp roll_pitch.cpp -o rp -pthread
