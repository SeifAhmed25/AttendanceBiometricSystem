# Attendance Biometric System 

## MCU

ATmega32 Microcontroller 

## Peripherals

R503 Finger Print Module

DS1307 RTC Module 

M24C08 EEPROM Module 

4*20 LCD Unit 

## MCAL (Micro Controller Abstraction Layer) 

DIO 

UART (Universal Asynchronous Reciever Transmitter Module) 

I2C (Two Wire Interface Module) 

## Project Description 
### New Enrollment Process 
- When a new Employee is to be enrolled in the system, he must choose to be enrolled using an option represented by LCD Module.

- To be enrolled he will insert  a unique ID which will be saved in the EEPROM Module. 

- Employee will be asked to insert his Finger Print and guided through the process with the help of LCD messages.

- The Finger Print will be connected to his ID in EEPROM.

### Login Process 
- Employee should insert his ID first, and it will be searched with through EEPROM Data base.
 
- If Employee ID is Found he will be ordered to enter his finger print and a match is carried.
 
- If a match is found, the system will register the login time using the RTC Module to get real time.
 
## Project Machine State Design
![Image Alt Text](https://ibb.co/QHJg1H1)  
