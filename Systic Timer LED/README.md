<br/>
<p align="center">
 <h1 align="center" id="title">Blinking LEDs</h1>

  <p align="center">
    ✈A program to control a set of LEDs using buttons 
    <br/>
    <br/>
  </p>
</p>


## 💡Concepts Used in This Project

* C programming language
* SysTick Timer
* Interrupts
* I/O ports
* Keil µVision 
  

## 🎯 Purpose

The aim of this project is to control on board RGB LED of FRDM KL-25Z board and 4 external LEDs using 3 buttons. As default RGB LED emits red light and toggles in every 0.8s. 4 external LEDs are used as a 4-bit binary counter. At the beginning of the program, they all turned of indicating counter value to be equal to 0. Functionality of buttons are given below:  

**Button 1:** Turn RGB LED color to green. Blink 5 times.  

**Button 2:** Halt blinking of RGB LED for 2 seconds.  

**Button 3:** Increase LED counter



## 🔓 Algorithm & Methodology

SysTick timer interrupt is used to toggle leds in required frequency. Necessary timer load value is calculated with respect to clock frequency of ARM Cortex-M0+. I/O port interrupts are enabled. When a button is pressed, program goes to related interrupt vector adress and does the required jobs. 


## 🎨 Design Specifications

MKL25Z4 header file is initialized at the beginning of the program so that port assignments could be done in a more convenient way. All related ports are configurated in *SystemStart* function. Pull-up resistor configuration is preferred for input ports so that input port is active low. PortA, PortD, and SysTick timer interrupts are enabled and prioritized in sequence: **SysTick > PortD > PortA**. SysTick timer load value is calculated to be *4193999<sub>10</sub>*. PortD interrupt Handler is assigned to Button3, therefore it increments the counter LEDs. *glitch_catch* is used to check whether there is a glitch, preventing glitches occuring when the button is pressed so counter LED does not increment by more than one in each press. PortA interrupt Handler is assigned to both Button1 and Button2. Because of the design of ARM Cortex-M0+, microcontroller is not able to differantiate from which pin of PortA there is an interrupt. Therefore, an extra if statement is needed to check which pin rised the interrupt flag. Pin18 of portA is assigned to Button1 and pin19 of portA is assigned to Button2. SysTick timer interrupt handler controls blinking frequency of RGB LED by toggling in each 10 interrupts. Counter variable *bekle* is used for this purpose. 
## 🏆 Simulation Results

*not completed yet*


## 🔎 Further Details

Under CodeFile

* You may check the LED_control.c file  to look at my main Code. [here](CodeFile/LED_control.c)

  



