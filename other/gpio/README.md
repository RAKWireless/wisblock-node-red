# Control the GPIOs in the Raspberry Pi or RAK7391 board from NodeRED.

[TOC]

## 1. Introduction

This guide explains how to create a flow and then use the node **node-red-node-pi-gpiod** to toggles an LED connected to a GPIO in the 40-pin header on a Raspberry Pi or a RAK7391 board.

### 1.1 Requirements

One of the most important things when using **node-red-node-pi-gpiod** is that you must do some setups and run the PiGPIO daemon first. If you are not using the NodeRed image provided by RAKwireless (or you want to control GPIOs on another device), you can follow the [node-red-node-pi-gpiod offical guidance]( https://flows.nodered.org/node/node-red-node-pi-gpiod), add the line  `/usr/bin/pigpiod` to the file `/etc/rc.local` , or start the PIGPIO daemon manually for temporary usage. If you are using the NodeRed image provided by us and also our portainer template, we made all the setup for you, so you can control the GPIO of the Raspberry Pi or RAK7391 board your NodeRed container is hosted on. The image we provided runs the PiGPIO daemon by default. 

### 1.2.  The 40-pin header and GPIO

There is a 40-pin header on RAK7391 board, it uses the same pinout Raspberry Pi uses. A row of GPIO ( general-purpose input/output) pins offers the ability for Raspberry Pi or RAK7391 to interface with the real world. The 40-pin header provides the following power and interface options: 

* 3V3 (power)
* 5V (power)
* Ground 
* GPIO (general-purpose input/output)
* I2C (Inter-integrated circuit)
* UART (Universal asynchronous receiver-transmitter)
* PCM (Pulse-code modulation)

Check the following figure for the pinout, make sure don't get confused by BCM numbering (aka “Broadcom” or ”GPIO“ ）and Board pin numbering (aka "physical pin)". They are two different numbering systems. BCM numbering refers to the pins defined by the "Broadcom SOC channel", while the Board pin refers to the pin's physical location on the header. For example, Board pin 7 is actually GPIO 4.  

<img src="assets\GPIO-Pinout-Diagram.png" alt="GPIO-Pinout-Diagram" style="zoom:67%;" />

<span style="color:blue">here is the [reference](https://www.raspberrypi.com/documentation/computers/images/GPIO-Pinout-Diagram-2.png), need the documentation team to cite it or create a new one?</span>

By configuring the status of each pin in software, users can define each pin as an input or output pin, and use them for different real-world applications.

<img src="assets\GPIO_pins.png" alt="GPIO_pins" style="zoom:67%;" />

<span style="color:blue">here is the [reference](https://www.raspberrypi.com/documentation/computers/images/GPIO.png), need the documentation team to cite it or create a new one?</span>

## 2. Preparation

### 2.1. Hardware

In this example, we will first connect a LED to Board pin 7 (GPIO 4), and then create a flow in node-red to toggle the led.  Please check the figure below for how to connect the LED:

![led_diagram](assets\led_diagram.png)

<span style="color:blue">need the documentation team to create a new one?</span>

Notice that you can connect the Ground to any of the ground pin( Board pin 6、9、14、20、25、30、34、39).

### 2.2. Software

After you deployed the Node-Red container using the [portainer app template](link to our portainer template) by Rakwireless, you can clone /copy the flow example. The example is under `other/gpio/gpio-toggle-led` folder in the [`wisblock-node-red`](https://git.rak-internal.net/product-rd/gateway/wis-developer/rak7391/wisblock-node-red/-/tree/dev/) repository. Then you can import the  **gpio-toggle-led.json** file or just copy and paste the .json file contents into your new flow.

After the import is done, the new flow should look like this:

![gpio-toggle-led-overview](assets\gpio-toggle-led-overview.png)

Hit the **Deploy** button on the top right to deploy the flow.

 This is a simple flow with two inject nodes for ON and OFF that toggles an LED connected to a Board pin 7 （GPIO 4). Now you should be able to toggle the LED by hitting the On and OFF button. 

### 2.3. Flow configuration

The two inject nodes are responsible for sending digital 0 and 1 (or true or false) to **pi-gpiod out** node, the **pi-gpiod out** node will set the selected Board pin high or low depending on the value passed in by the inject nodes.  One more thing to notice is that the **Host** defined in the  **pi-gpiod out** node is set to localhost, because the PiGPIO daemon is running inside the container. To control the GPIOs on another device/outside the container, please change this Host virable.

