# Acquisition and Processing of Motion Sensor Data Implemented on Embedded Linux Platform

## Introduction
The topic of this project was to develop hardware support for the implementation and testing of a complete motion sensor data acquisition system. The test environment on the Linux operating system includes acquisition, visualization, storage, and processing of source data. The key tool in the work was the Qt framework and C ++ . Based on the observation of the graph describing the axis of the gyroscope and accelerometer, an application was made to verify the physiotherapeutic exercise and the 3D motion sensor model. 



## Techonology

MPU6050 – integrated circuit that conisists of accelerometer and gyroscope
Raspberry Pi2 - single board computer, with I2C protocol connected to MPU6050
Qt, C++ - cross-platform application framework in which application is created
Python – script that runs on Raspberry Pi2 ( reads data from sensor and sends it on local area network

![image](https://user-images.githubusercontent.com/24511950/29192485-e156760a-7e21-11e7-986d-016f0760806f.png)


## Achivement

Developed device sends data in real-time and displays it in the application.
The device consists of Rpi2, MPU6050 and potable charger.

![image](https://user-images.githubusercontent.com/24511950/29192784-08d4c5b4-7e23-11e7-9b00-7a9436632c8e.png)

The application consists of 4 parts:

 1. A graph in which the data from the sensor is drawn
![image](https://user-images.githubusercontent.com/24511950/29192764-ef3d9108-7e22-11e7-9489-23c6a0d53ef7.png)

 2.	Window to display information on proper exercise execution 
	  (it is made for exercise on picture above)
![image](https://user-images.githubusercontent.com/24511950/29192770-f41c8422-7e22-11e7-99da-3a84e521ab28.png)
![image](https://user-images.githubusercontent.com/24511950/29192774-f99cfe86-7e22-11e7-9d75-870bb5fbd185.png)

 3. Recording and display of recorded data
![image](https://user-images.githubusercontent.com/24511950/29192968-c212a4ec-7e23-11e7-8c8d-6290063fc112.png)
 
 4. 3D model of MPU6050 that rotates in real-time, and table where data from sensor is written
![image](https://user-images.githubusercontent.com/24511950/29192781-03da9688-7e23-11e7-9226-e8cafbcffc66.png)





