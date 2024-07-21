Wheeled Rover with CANBUS Communication
Project Main Subject: CANBUS
Project Name: Wheeled Rover with Cam
Abstract
The proposed project aims to design and develop a wheeled Rover equipped with two ESP32s and an ESP32-Cam microcontroller, alongside MCP2515 CAN bus modules to enhance control and communication capabilities.

Keywords
ESP32s, ESP32-CAM, Rover, Sensor, LCD Display

Objectives
Design and construct a wheeled Rover chassis capable of traversing various terrains and environments.
Integrate three ESP32 microcontrollers into the Rover system to enable efficient data processing.
Incorporate CAN bus modules for reliable communication between ESP32 units and peripheral devices.
Implement sensors and actuators for detecting objects, obstacles, and navigation.
What is a Wheeled Rover?
A wheeled rover is a type of robotic vehicle designed for mobility on land surfaces. It typically consists of a chassis equipped with wheels or tracks, motors for propulsion, and various sensors, actuators, and control systems. They can be controlled remotely or autonomously, depending on their design.

What are ESP-32s and ESP32-CAM?
ESP-32s
The ESP-32s is a powerful Wi-Fi and Bluetooth-enabled microcontroller chip.
Widely used in IoT and control projects.
It can be programmed using Arduino IDE, ESP-IDF.
ESP32-CAM
It is a specific variant of the ESP32 microcontroller with a built-in camera module.
It can capture images and primitively recognize faces.
Popular for projects involving video streaming.
It can be programmed with the same process as ESP-32s.
Modules
TJA1050 High Speed CAN Transceiver Module
The TJA1050 is the interface between the Controller Area Network (CAN) protocol controller and the physical bus.
Provides differential transmit capability to the bus and differential receive capability to the CAN controller.
Lower electromagnetic emission and improved behavior in case of an unpowered node.
MPU6050 Gyroscope and Accelerometer
The MPU6050 is a Micro-Electro-Mechanical System (MEMS) that consists of a 3-axis Accelerometer and 3-axis Gyroscope.
Measures acceleration, velocity, orientation, displacement, and many other motion-related parameters.
Communicates using the I2C protocol.
Standard 16x2 LCD Display
A 16x2 LCD can display 16 characters per line, with 2 lines available.
Displays characters in a 5x7 pixel matrix and is capable of showing 224 different characters and symbols.
HC-SR04 Ultrasonic Sensor
The HC-SR04 ultrasonic sensor measures distance by sending an ultrasonic pulse and measuring the time it takes for the echo to return.

Block Diagram
[block-diagram](İmages/Block_Diagram.png)

Operating Mechanism
The project consists of three main parts: Central Processing Unit, Sensor Unit, and Camera Display Unit.

Central Processing Unit (CPU): Uses ESP32s to receive and process data from the sensors.
Sensor Unit: Collects data from MPU6050 and HC-SR04 sensors and sends it via the CANBUS module.
Camera Display Unit: Uses ESP32-CAM to capture images based on proximity data and displays them on an LCD.
