# Acquisition and Processing of Motion Sensor Data Implemented on Embedded Linux Platform

## Introduction
The topic of this project was to develop hardware support for the implementation and testing of a complete motion sensor data acquisition system. The test environment on the Linux operating system includes acquisition, visualization, storage, and processing of source data. The key tool in the work was the Qt framework and C ++ . Based on the observation of the graph describing the axis of the gyroscope and accelerometer, an application was made to verify the physiotherapeutic exercise and the 3D motion sensor model. 



## Technology

MPU6050 – integrated circuit that consists of accelerometer and gyroscope
Raspberry Pi2 - single board computer, with I2C protocol connected to MPU6050
Qt, C++ - cross-platform application framework in which application is created
Python – script that runs on Raspberry Pi2 ( reads data from sensor and sends it on local area network

![image](https://user-images.githubusercontent.com/24511950/29192485-e156760a-7e21-11e7-986d-016f0760806f.png)


## Achievement

Developed device sends data in real-time and displays it in the application.
The device consists of Rpi2, MPU6050 and potable charger.

![mpuhand](https://user-images.githubusercontent.com/24511950/29194515-3fae2ee8-7e2a-11e7-8f4f-ae9853dd937a.png)

The application consists of 4 parts:

 1. A graph in which the data from the sensor is drawn
 
![graph](https://user-images.githubusercontent.com/24511950/29194512-3ef073bc-7e2a-11e7-8639-e39dfd4c26dd.jpg)

 2. Window to display information on proper exercise execution
 
![exerciseapp](https://user-images.githubusercontent.com/24511950/29194511-3ead84a8-7e2a-11e7-97f2-f9cee6916bdc.png)
 
 3. Recording and display of recorded data

![rec](https://user-images.githubusercontent.com/24511950/29194513-3efca1a0-7e2a-11e7-9a1d-3fdfa257dfb0.png)
 
 4. 3D model of MPU6050 that rotates in real-time, and table where data from sensor is written

![mpucroped](https://user-images.githubusercontent.com/24511950/29194514-3efcf07e-7e2a-11e7-9080-0175379406ea.jpg)





