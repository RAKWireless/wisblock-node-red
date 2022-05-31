# Measure Temperature and Humidity Using WisBlock Sensor RAK1901(SHTC3) from Node-RED.

[TOC]

## 1. Introduction

This guide explains how to use the [Wisblock Sensor RAK1901](https://docs.rakwireless.com/Product-Categories/WisBlock/RAK1901/Overview/#product-description) to measure temperature and humidity through the I2C interface using Node-RED. 

### 1.1 RAK1901

The RAK1901 is a WisBlock sensor board that contains a SHTC3 chip, a Sensirion temperature and humidity sensor. The SHTC3 is a digital temperature and humidity sensor designed especially for battery-driven high-volume consumer electronics applications. The device comprises a sensing element and an IC interface which communicates through I2C from the sensing element to the application. While the RAK7391 comes with a SHTC3 soldered on board, you can use the same flow to read from both RAK1901 and the RAK7391's on-board SHTC3.  

### 1.2 node-red-contrib-shtc3

The node we used in this flow is [node-red-contrib-shtc3](https://git.rak-internal.net/product-rd/gateway/wis-developer/rak7391/node-red-nodes/-/tree/master/node-red-contrib-shtc3). The node allows users to configure the I2C bus, the I2C address, and also the temperature unit.

## 2. Preparation

### 2.1 Access Setup

In this example, we are going to deploy a flow in Node-RED to measure temperature and humidity. To make the measurements, ensure you have access to I2C devices. 

If you are using Node-RED locally (in the host machine without using docker containers), you need to make sure the Node-RED user has access to the i2c bus (/dev/i2c-1 by default) on your host machine.

If your Node-RED is deployed inside a container, you need to mount `/dev/i2c-1` to the Node-RED container, and also make sure the user inside the container is assigned to the right group so that it has access to I2C devices.

For detailed "docker run" command, docker-compose file, and information about how to use a pre-configured Portainer template, please check this [instruction](https://git.rak-internal.net/product-rd/gateway/wis-developer/rak7391/wisblock-node-red/-/blob/dev/README-Docker/README.md), we provide all the information you need to know about using containerized Node-RED.

### 2.2 Install node in Node-RED

Now we need to install the required nodes for the example flow. Browse to http://{host-ip}:1880 to access Node-Red's web interface. In this example, you need to install only one node: [node-red-contrib-shtc3](https://git.rak-internal.net/product-rd/gateway/wis-developer/rak7391/node-red-nodes/-/tree/master/node-red-contrib-shtc3).

To install this node , go to the top right **Menu**, and then select **Manage palette**. In the **User Settings** page, you need to select **Install**, and search the keyword **node-red-contrib-shtc3**. Now you should be able to install this node. This node is developed by RAKWireless, the source code is hosted in this [repo](https://git.rak-internal.net/product-rd/gateway/wis-developer/rak7391/node-red-nodes/-/tree/dev/node-red-contrib-ltr-390uv), and you can also check this [documentation](https://git.rak-internal.net/product-rd/gateway/wis-developer/rak7391/wisblock-node-red/-/blob/dev/README-Docker/README.md) about how to install the node manually using command line.

![install node-red-contrib-libgpiod](assets/install-node.png)

need to change this image once out node is published.

### 2.3 Hardware

If you are using RAK7391, there is no extra hardwares required, as the SHTC3 is soldered on-board already. If you are going to use a use the RAK1901 on a Raspberry Pi or IO board, the easiest way is to use the RAK6421 WisBlock Hat that exposes all the Wisblock high-density connector pins.  The RAK1901 can be mounted to the HAT, and the HAT goes to the 40-pin headers located on Raspberry Pi 4B/IO board. 

* RAK1901 + Raspberry Pi +RAK6421 Pi-HAT

<img src="assets/rak1901+raspberrypi+pi-hat.png" alt="rak1901+raspberrypi+Pi-HAT" style="zoom:80%;" />

## 3. Flow configuration

After you deployed the Node-Red container using the [portainer app template](link to our portainer template) by Rakwireless, you can clone /copy the flow example. The example is under `sensors\rak1901\rak1901-shtc3-read` folder in the [`wisblock-node-red`](https://git.rak-internal.net/product-rd/gateway/wis-developer/rak7391/wisblock-node-red/-/tree/dev/) repository. Then you can import the  **rak1901-shtc3-read.json** file or just copy and paste the .json file contents into your new flow.

After the import is done, the new flow should look like this:

![flow overview](assets/flow-overview.png)

### 3.1 Nodes Configurations

* node-red-contrib-shtc3 

To get value of  temperature and humidity you need to select the correct bus number and i2c address for the SHTC3 chip.

<img src="assets/image-20220321162528246.png" alt="image-20220321162528246" style="zoom:80%;" />

- **Name**

  Define the msg name if you wish to change the name displayed on the node.

- **/dev/i2c-?**

  Default I2C Bus is 1.  `1` is for `'/dev/i2c-1'`.

- **i2c_Address**

  The Address fo shtc3 is 0x70 which can not be changed. 

- **Temperature Unit**

  You can select `Celsius` or `Fahrenheit` as you like.



## 4. Flow output

This is a simple flow with three node, where `inject` node supply a trigger event every 5 seconds, `shtc3_i2c`node read data of shtc3, and `debug` node print the temperature and humidity read from shtc3 sensor.

The result is as follows:

![image-20220321103758128](assets/image-20220321103758128.png)

