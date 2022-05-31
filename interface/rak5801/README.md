# Read 4-20 mA current input using RAK5801 from NodeRed

[TOC]

## 1. Introduction

This guide explains how to use `RAK5801` with pi-hat `RAK6421` to measure 4-20 mA current input through a NodeRed flow.

### 1.1. RAK5801

The RAK5801 is a 4-20 mA current loop extension module that allows you to make an IoT solution for analog sensors with 4-20 mA interface. This module converts the 4-20 mA current signal into voltage range supported by the **ADS1115**  for further digitalization and data transmission.  For more information about RAK5801, refer to the [RAK5801 datasheet](https://docs.rakwireless.com/Product-Categories/WisBlock/RAK5801/Datasheet/).

<img src="assets/RAK5801.png" alt="RAK5801" style="zoom:50%;" />

### 1.2. RAK6421

RAK6421 is a pi-hat for [Wisblock modules](https://docs.rakwireless.com/Product-Categories/WisBlock/). It has **two IO slots** and **four sensor slots**.

<img src="assets/rak6421.png" alt="rak6421" style="zoom:40%;" />



### 1.3. ADS1115

ADS1115 is an onboard ADC chip in the RAK6421. It is a high precision 16-bit ADC with 4 channels. it have a programmable gain from 2/3x to 16x so you can amplify small signals and read them with higher precision. Refer to datasheet for more information : [ADS1115 datasheet](https://cdn-shop.adafruit.com/datasheets/ads1115.pdf).




## 2. Preparation

### 2.1 Access Setup

In this example, we are going to deploy a flow in Node-RED to measure temperature and humidity. To make the measurements, ensure you have access to I2C devices. 

If you are using Node-RED locally (in the host machine without using docker containers), you need to make sure the Node-RED user has access to the i2c bus (/dev/i2c-1 by default) on your host machine. You can enable I2C either by using **raspi-config** or just change `/boot/config.txt`.

If your Node-RED is deployed inside a container, you need to mount `/dev/i2c-1` to the Node-RED container, and also make sure the user inside the container is assigned to the right group so that it has access to I2C devices.

For detailed "docker run" command, docker-compose file, and information about how to use a pre-configured Portainer template, please check this [instruction](https://git.rak-internal.net/product-rd/gateway/wis-developer/rak7391/wisblock-node-red/-/blob/dev/README-Docker/README.md), we provide all the information you need to know about using containerized Node-RED.

### 2.1. Hardware

There are two WisBlock IO connectors (`wisblock#1` and `wisblock#2`)on the RAK6421. User can connect RAK5801 module with any one of them. There are two analog input pins(`A0` and `A1`) on the RAK5801, but only `A1` is available now.  You can use it as long as your sensors **operate at `3.3 V` or `12 V` with `4-20 mA` operating current**. 

In my example, I use an external power supply to simulate changes in the sensor's input current.

- **Raspberry Pi model 4B + RAK6421 WisBlock Hat +  RAK5801**

<img src="assets/image-20220531111019812.png" alt="image-20220531111019812" style="zoom:50%;" />



- **RAK7391 + Raspberry Pi CM4 +  RAK5801**

<img src="assets/image-20220531111124216.png" alt="image-20220531111124216" style="zoom: 67%;" />



### 2.2. Software

This flow use `node-red-contrib-ads1x15_i2c`  module, so you must install the module to your node-red first. run the following command in the root directory of your node-red install

```
npm install node-red-contrib-ads1x15_i2c
```

Another way to install required module is from editor window, open the main menu on the right, select  the `Manage Palette` option,  search node-red-contrib-ads1x15_i2c modules in the `Install` tab and install it.

<img src="assets/install.png" alt="install" style="zoom:67%;" />



## 3. Run example

Now you can import [rak5801-example](./rak5801-example-flow.json) flow.  If you use rak7391 without pi-hat rak6421, you should import [another](./rak5801-rak7391-example-flow.json) flow.

![image-20220531102017302](assets/rak5801-read-flow.png)

Use `gpio out` node to enable `rak5801`.  `ads1x15_i2c` node provides access to onboard ADS1115  to get a voltage from RAK5801 analog input.  `voltage2current` is a function node convert voltage to the corresponding current. 

After all the preparation, hit the `Deploy` button on the top right to deploy the flow, then click inject node to trigger a reading, debug node will print the current value.

![image-debug](assets/debug.png)



## 4. License

This project is licensed under MIT license.
