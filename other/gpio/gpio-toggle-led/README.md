# Control the native GPIOs on Raspberry Pi or RAK7391 board from NodeRED.

[TOC]

## 1. Introduction

This guide explains how to create a flow and then use the node **node-red-contrib-libgpiod** to toggles an LED connected to a GPIO pin in the 40-pin header on a Raspberry Pi or a RAK7391 board.

### 1.1  The 40-pin header and GPIO

There is a 40-pin header on RAK7391 board, it follows the same pinout Raspberry Pi uses. The 40-pin header is a row of GPIO ( general-purpose input/output) pins offers the ability for Raspberry Pi or RAK7391 to interface with the real world. The 40-pin header provides the following power and interface options: 

* 3V3 (power)
* 5V (power)
* Ground 
* GPIO (general-purpose input/output)
* I2C (Inter-integrated circuit)
* UART (Universal asynchronous receiver-transmitter)
* PCM (Pulse-code modulation)

Check the following figure for the pinout, make sure don't get confused by BCM numbering (aka “Broadcom” or ”GPIO“ ）and Board pin numbering (aka "physical pin)". They are two different numbering systems. BCM numbering refers to the pins defined by the "Broadcom SOC channel", while the Board pin refers to the pin's physical location on the header. For example, Board pin 7 and GPIO 4 refer to the same pin.  

![GPIO-Pinout-Diagram](assets/GPIO-Pinout-Diagram.png)

<span style="color:blue">here is the [reference](https://pinout.xyz/), need the documentation team to cite it or create a new one?</span>

By configuring the status of each pin in software, users can define each pin as an input or output pin, and use them for different applications.

### 1.2 node-red-contrib-libgpiod

The node we used in this flow is **[node-red-contrib-libgpiod](https://flows.nodered.org/node/node-red-contrib-libgpiod)**. It contains a set of input and output nodes for controlling General Purpose Input and Outputs (GPIOs) though libgpiod (ioctl). One of the most important things to notice when using **node-red-contrib-libgpiod** in a container is to make sure you have access to the GPIO chip inside the container. We will cover more details about it in section 2.1. If you are interested in using this node locally (outside container), please also check the [node's introduction in Node-RED's library](https://flows.nodered.org/node/node-red-contrib-libgpiod) and also their [repo](https://github.com/s5z6/node-red-contrib-libgpiod). 

## 2. Preparation

### 2.1 Access Setup

Ensure you have access to GPIO device. In this example, since we are going to deploy a flow in Node-RED to toggle a LED connected to GPIO4 (board pin 7), you need to make sure you can detect the GPIO chip first whether you use Node-RED locally or inside container. 

If you are only trying to control the native 40-pin header on Raspberry or RAK7391, you won't need to modify the `config.txt` file; however, if you need to control other gpiochips connected to the Raspberry Pi/RAK7391, for example, a built-in GPIO expander on RAK7391, please check [this example](https://git.rak-internal.net/product-rd/gateway/wis-developer/rak7391/wisblock-node-red/-/tree/dev/other/libgpiod/libgpiod-blink) for instructions on how to modify the `config.txt` file and enable device tree overlay(s). 

If you are using Node-RED locally (in the host machine without using docker containers), you need to make sure the Node-RED user has access to the gpio(/dev/gpiochip0 by default) on your host machine.

If your Node-RED is deployed inside a container, you need to mount `/dev/gpiochip0` to the Node-RED container, and also make sure the user inside the container is assigned to the right group so that it has access to GPIO devices.

For detailed "docker run" command, docker-compose file, and information about how to use a pre-configured Portainer template, please check this [instruction](../../../README-Docker/README.md), we provide all the information you need to know about using containerized Node-RED.

### 2.2 Install dependency & nodes in Node-RED

Before we install the nodes, please make sure `libgpiod-dev` has been installed, if not, please install it.

```plaintext
sudo apt update
sudo apt install libgpiod-dev
```

If your Node-RED is deployed inside a container, you need to install `libgpiod-dev` inside container, please also check the [instruction](../../../README-Docker/README.md) on install dependency inside container.

Now we need to install some nodes for the example flow. Browse to http://{host-ip}:1880 to access Node-Red's web interface. In this example, you need to install one node: [node-red-contrib-libgpiod](https://flows.nodered.org/node/node-red-contrib-libgpiod).

Take `node-red-contrib-libgpiod` as an example. To install this node , go to the top right **Menu**, and then select **Manage palette**. In the **User Settings** page, you need to select **Install**, and search the key word **node-red-contrib-libgpiod**. Now you should be able to install this node.

![install node-red-contrib-libgpiod](assets/install-node.png)

### 2.3 Hardware  

For the hardware side, you need a Raspberry Pi (or RAK7391), LED, and some jumper wires. In this example, we will first connect a LED to Board pin 7 (GPIO 4), and then create a flow in node-red to toggle the led.  Please check the figure below for how to connect the LED:

![led_diagram](assets/led_diagram.png)

<span style="color:blue">need the documentation team to create a new one?</span>

Notice that you can connect the Ground to any of the ground pin( Board pin 6、9、14、20、25、30、34、39).

## 3. Flow configuration

You can import the  [gpio-toggle-led.json](./gpio-toggle-led.json) file or just copy and paste the .json file contents into your new flow.

After the import is done, the new flow should look like this:

![GPIO toggle led flow](assets/flow-example.jpg)

Hit the **Deploy** button on the top right to deploy the flow.

This is a simple flow with one inject node to trigger the function node once a second, the GPIO out node set/unset the pin defined, and the debug node display the result. 

* inject node

  ![inject node configuration](assets/inject-node.png)

* function node

  ![function node configuration](assets/function-node.png)

* GPIO out node

  In section 2.1, we learned that the device name of the native 40-pin headers is **gpiochip0**, and since we want to control GPIO 4, we need to enter 4 for the **Pin**. For the **Name** option, you can change it to anything based on your preference.

  ![GPIO-out node configuration](assets/GPIO-out-node.png)

* Debug node

  ![debug node configuration](assets/debug-node.png)



## 4. Flow output

Once the flow is correctly configured and deployed, you should see the LED connected to GPIO 4 is blinking at a frequency of 1 Hz, and also the output payloads in the debug window.

![debug window](assets/debug-window.png)



## License

This project is licensed under MIT license.
