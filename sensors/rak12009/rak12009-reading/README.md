# Read Alcohol Gas PPM Using WisBlock Sensor RAK12009 from Node-RED 

[TOC]

## 1 Introduction

This guide explains how to use the [WisBlock Sensor RAK12009](https://docs.rakwireless.com/Product-Categories/WisBlock/RAK12009/Overview/) in combination with RAK6421 Wisblock Hat or RAK7391 WisGate Developer Connect to measure alcohol gas PPM through the I2C interface using Node-RED.  

### 1.1 RAK12009

The RAK12009 is an Alcohol Gas Sensor module, part of the RAKWireless WisBlock Sensor Series. The sensor used in this module is the MQ-3B from Zhengzhou Winsen Electronics. There is also a ADC chip (ADC121C021) on RAK12009. This sensor is capable of detecting alcohol gas in the air with an alcohol concentration detection range of 25 to 500 ppm.For more information about RAK12009, refer to the [Datasheet](https://docs.rakwireless.com/Product-Categories/WisBlock/RAK12009/Datasheet/).

In RAK12009, the concentrations of gas is measured using a voltage divider network present in the sensor. The output of the sensing element is connected to a 12-bit ADC (ADC121C021) which communicates through I2C to the application. The ADC121C021 supports the I2C serial bus and data transmission protocol, and it operates as a slave device on the I2C bus. For more information about ADC121C021, refer to the [datasheet](https://www.ti.com/lit/ds/symlink/adc121c021.pdf?ts=1649226964688&ref_url=https%253A%252F%252Fwww.google.com.hk%252F). 

### 1.2 @rakwireless/adc121c021 &  node-red-contrib-libgpiod

The node we used in this flow is **[@rakwireless/adc121c021](https://www.npmjs.com/package/@rakwireless/adc121c021)**, as well as [node-red-contrib-libgpiod](https://flows.nodered.org/node/node-red-contrib-libgpiod). 

The `@rakwireless/adc121c021` provides the very basic configuration for users to configure, the user only needs to define the i2c bus of the chip. The chip is set to the automatic conversion mode by default, and the cycle time is set to 32. The I2C address of adc121c021 is configured to `0x55` on RAK12009. 

The `node-red-contrib-libgpiod` node contains a set of input and output nodes for controlling General Purpose Input and Outputs (GPIOs) though libgpiod (ioctl). It is used to pull the EN pin that must be pulled high before ASC121C021 can read analog inputs.

One of the most important things to notice when using `node-red-contrib-libgpiod` inside a container is to make sure you have access to the GPIO chip inside the container. We will cover more details about it in this [documentation](https://git.rak-internal.net/product-rd/gateway/wis-developer/rak7391/wisblock-node-red/-/blob/dev/README-Docker/README.md). If you are interested in using this node locally (outside container), please also check the [node's introduction in Node-RED's library](https://flows.nodered.org/node/node-red-contrib-libgpiod) and also their [repo](https://github.com/s5z6/node-red-contrib-libgpiod). 



## 2 Preparation

### 2.1 Access Setup

Ensure you have access to both I2C devices and GPIO when using RAK12009. In this example, we are going to deploy a flow in Node-RED to to measure alcohol gas PPM through the I2C interface. To make the measurement, we need to enable the ADC121C021 chip first. The Enable pin varies based on your hardware setup, when you connect the RAK12009 to the IO slot1 on RAK6421, the ENABLE pin is GPIO 12 (board pin 32). When using IO slot 2 on RAK6421 (check the hardware preparation section for pictures), the ENABLE pin is GPIO 22 (board pin 15). 

If you are using Node-RED locally (in the host machine without using docker containers), you need to make sure the Node-RED user has access to the i2c bus (/dev/i2c-1 by default) and gpio(/dev/gpiochip0 by default) on your host machine.

If your Node-RED is deployed inside a container, you need to mount `/dev/i2c-1` and `/dev/gpiochip0` to the Node-RED container, and also make sure the user inside the container is assigned to the right group so that it has access to I2C and GPIO devices.

For detailed "docker run" command, docker-compose file, and information about how to use a pre-configured Portainer template, please check this [instruction](../../../README-Docker/README.md), we provide all the information you need to know about using containerized Node-RED.

### 2.2 Install dependency & nodes in Node-RED

Before we install the nodes, please make sure `libgpiod-dev` has been installed, if not, please install it.

```plaintext
sudo apt update
sudo apt install libgpiod-dev
```

If your Node-RED is deployed inside a container, you need to install `libgpiod-dev` inside container, please also check the [instruction](../../../README-Docker/README.md) on install dependency inside container.

Now we need to install some nodes for the example flow. Browse to http://{host-ip}:1880 to access Node-Red's web interface. In this example, you need to install two nodes: [node-red-contrib-libgpiod](https://flows.nodered.org/node/node-red-contrib-libgpiod) and [@rakwireless/adc121c021](https://www.npmjs.com/package/@rakwireless/adc121c021).

To install this node , go to the top right **Menu**, and then select **Manage palette**. In the **User Settings** page, you need to select **Install**, and search the key word **node-red-contrib-libgpiod**. Now you should be able to install this node.

![install node-red-contrib-libgpiod](assets/install-node.png)

![install-node2](assets/install-node2.png)

### 2.3 Hardware  

The device address of RAK12009 is configured to 0x55, and connected to i2c bus 1. 

The easiest way to set up the hardware is to use the RAK6421 WisBlock Hat that exposes all the Wisbock high-density connector pins.  The RAK12009 can be mounted to the HAT, and the HAT goes to the 40-pin headers located on Raspberry Pi 4B/IO board/RAK7391. Based on your hardware selections, there are three ways to mount RAK12009, please check the following figured(use slot 2 on RAK6421, you can switch to use slot 1, make sure to change the Enable pin defined in the flow):

1. Raspberry Pi model B + RAK6421 WisBlock Hat +  RAK12009

   ![mouned on Raspberry 4B](assets/mount-on-raspberrypi-4b.jpg)

2. Raspberry Pi CM4 + Compute Module 4 IO Board + RAK6421 WisBlock Hat + RAK12009

   ![mouned on Compute Module 4 IO Board](assets/mount-on-IO board.jpg)

3. Raspberry Pi CM4  + RAK7391 WisGate Developer Connect + RAK6421 WisBlock Hat + RAK12009

   ![mouned on RAK7391](assets/mount-on-RAK7391.jpg)



## 3 Flow configuration

Import the  [rak12009-reading](./rak12009-reading.json) file or just copy and paste the .json file contents into your new flow.

After the import is done, the new flow should look like this:

![RAK12009 flow overview](assets/flow-overview.png)



As we mentioned in section 2.5,  `node-red-contrib-libgpiod` node is used to pull the Enable pin that must be pulled high before ASC121C021 can read analog input.

* Inject nodes

  The two inject nodes connected to the **GPIO out node node** are responsible for sending digital 0 and 1 (or true or false) to **GPIO out node** node, the **GPIO out node** will set the selected Board pin to high or low based on the value passed in by the inject nodes. 

  ![inject nodes](assets/inject-nodes.png)

* GPIO out node

  In section 2.1, we learned that the device name of the native 40-pin headers is **gpiochip0**, and since we want mount the RAK12009 on slot 2 of RAK6421 Pi-HAT, the Enable pi is GPIO 22. For the **Name** option, you can change it to anything based on your preference.

  ![GPIO-out node.png](assets/GPIO-out-node.png)

  

- Debug node (Pin status)

  The output of this debug node is the status of the Enable pin.

  ![Enable Pin's debug node](assets/debug-node1.png)
  
- inject node (5s trigger)

  The inject node connected to the adc121c021_i2c node will trigger the adc121c021_i2c node every five seconds.

  ![5s trigger inject node](assets/trigger-inject-node.png)
  
- adc121c021_i2c node

  ![adc121c021 node](assets/adc121c021-node.png)

  To get the voltage reading from the adc121c021, users only need to define the i2c bus of the chip. In this case (using RAK12009), the adc121c021 is on bus 1, and the I2C address of the chip ADC121c021 is configured to 0x55 on RAK12009. 

- function node 

  The function node is used to retrieve the voltage readings from the ADC chip, calibrate the sensor based on the users' setup and environment, and then calculate the PPM values. 
  
  <img src="assets/function-node.png" alt="function node"/>
  
  In order to make the sensor with better performance, calibration of R0 is needed. Users can also define the regression method in the function node. The value of `Ratiomq3CleanAir` depends on the gas users want to monitor, and constantA and constanB are calculated based on the Sensitive Characteristics provided in MQ3's [datasheet](https://www.sparkfun.com/datasheets/Sensors/MQ-3.pdf).
  
  The script is provided below:
  
  ```
  // Retrieve voltage readings from the adc node
  var sensorVoltage = msg.payload.Volts;
  const MQ2_RL = 10;
  
  const RatioMQ2CleanAir = 1;
  const VOLTAGE_REF = 5;
  const constantA = -0.888;
  const constantB = 0.738;
  
  
  function readSensor(regressionMethod) {
      // set the regression method if=1 use Exponential else Linear
      //     Exponential:  PPM =  A*(RS/R0)^B
      //     Linear:       PPM =  pow(10, (log10(RS/R0)-B)/A) 
  
      var RS_air = (VOLTAGE_REF * MQ2_RL / sensorVoltage) - MQ2_RL;
      // console.log("DEBUG RS_air VALUE", RS_air);
      var ppm;
      if (RS_air < 0) {
          //No negative values accepted.
          RS_air = 0;
      }
      // Get ratio RS_gas/RS_air
      var ratio = RS_air / R0;
      if (ratio <= 0) {
          //No negative values accepted or upper datasheet recomendation. 
          ratio = 0;
      }
  
      if (regressionMethod == 1) {
          ppm = constantA * Math.pow(ratio, constantB);
      } else if (regressionMethod == 0){
          //Get ppm value in linear scale according to the the ratio value 
          let ppm_log = (Math.log10(ratio) - constantB) / constantA;
          //Convert ppm value to log scale 
          ppm = Math.pow(10, ppm_log);
      } else{throw "Wrong regression method, only support 1 and 0 ";}
  
      if (ppm < 0) {
          ppm = 0;
      }
      return ppm;
  }
  
  function calibrateR0(ratioInCleanAir) {
      //Calculate RS in fresh air
      var RS_air = (VOLTAGE_REF * MQ2_RL / sensorVoltage) - MQ2_RL;
      //No negative values accepted
      if (RS_air < 0) {
          RS_air = 0;
      }
      //Calculate R0 
      R0 = RS_air / ratioInCleanAir;
      //No negative values accepted
      if (R0 < 0) {
          R0 = 0;
      }0
      return R0;
  }
  
  
  
  // Retrieve calibated R0 value
  var R0 = flow.get('R0');
  // calibrate R0 if it hasn't been calibrated before
  if (!R0)
  {
      var clacR0 = 0;
      for (let i = 0; i <= 100; i++) {
          clacR0 =clacR0+calibrateR0(RatioMQ2CleanAir);
      }
      R0=clacR0/100;
  
      if (R0 === Infinity) {
          throw "Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply";
      }
      if (R0 == 0){
          throw "Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply";
      }
  
      //set the value of R0 for the flow
      flow.set("R0",R0);
  }
  
  // get sensor PPM value
  var sensorPPM = readSensor(0);
  var payload = {};
  
  payload.PPM_value = sensorPPM.toFixed (3);
  payload.PPM_percentage = ((sensorPPM / 10000) * 100).toFixed(3) +'%';
  payload.R0=R0.toFixed(3);
  
  return {
      payload: payload
  };
  ```

## 4 Flow output

The output of the node is a payload that contains the PPM value, the percentage readings, and also the value of R0,

<img src="assets/flow-output.png" alt="output of the adc121c021_i2c flow "/>



## License

This project is licensed under MIT license.
