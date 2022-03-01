## 1.Introduction

the **ADC**(Analog to Digital Converter)  chip on the RAK7391 board is **ADS1115**. ADS1115 is  a high recision16-bit ADC with 4 channels.  it have a programmable gain from 2/3x to 16x so you can amplify small signals and read them with higher precision. Refer to datasheet for more information : [ADS1115 datasheet](https://cdn-shop.adafruit.com/datasheets/ads1115.pdf).

this flow show how to read 4 input channels of ads1115 with [node-red-contrib-ads1x15_i2c](https://flows.nodered.org/node/node-red-contrib-ads1x15_i2c) module.



## 2. Node Description

this flow consists of three nodes: `inject` node,  `ads1x15_i2c` node, `debug` node.

### 2.1. inject node

in this flow, inject node just as a trigger to start a reading data.

### 2.2. ads1x15_i2c node

an ads1x15_i2c node provide access to a ADS1x15 I2C analog to digital converter, to get a voltage or difference of voltage from a ADS1115 or ADS1015  just select the correct setting for your device and trigger the node.

![ads1x15_i2c](assets\ads1x15_i2c.png)



#### 2.2.1.name

Define the msg name if you wish to change the name displayed on the node.

#### 2.2.2.Property

Define the msg property name you wish. The name you select (msg.example) will also be the output property.the payload must be a number! Anything else will try to be parsed into a number and rejected if that fails.

#### 2.2.3.Chipset

The Chipset by default is set to 1115. The Chipset is the version of ads supported. If you have an ads1015 select that option.

#### 2.2.4./dev/i2c-?

the i2c device file you will access, the value by default is set to 1, which means the i2c bus index is 1.

#### 2.2.5.i2c_Address

The Address by default is set to 0x48. You can setup the ADS1X15 with one of four addresses, 0x48, 0x49, 0x4a, 0x4b. Please see ads1X15 documentation for more information

#### 2.2.2.5.Inputs

Inputs may be used for Single-ended measurements (A0-GND) or Differential measurements (A0-A1). Single-ended measurements measure voltages relative to a shared reference point which is almost always the main units ground. Differential measurements are “floating”, meaning that it has no reference to ground. The measurement is taken as the voltage difference between the two wires. Example: The voltage of a battery can be taken by connecting A0 to one terminal and A1 to the other.

#### 2.2.2.6.Samples

Select the sample per second you want your ADS to make. Higher rate equals more samples taken before being averaged and sent back from the ADS. Please see ads1X15 documentation for more information

#### 2.2.2.7.Gain

I Select the Gain you want. To increase accuracy of smaller voltage signals, the gain can be adjusted to a lower range. Do NOT input voltages higher than the range or device max voltage, pi 3.3v use a voltage devider to lower input voltages as needed.



### 2.3 debug node

debug node prints the read details about each channels to debug window.

![ads1115-read](assets\ads1115-read.png)



## 3. Access Setup

ADS1115 use the an I2C communication protocol to read analog values, you have to make sure you have access to i2c bus(`/dev/i2c-1` by default) so that this flow can works in your current system. 

In general, no additional settings are required when you run Node-Red on your host directly. if running Node-red using docker, you need to mount `/dev/i2c-1` device to the Node-Red container and add a Node-Red user with i2c group on your host.

#### 3.1 Running under Docker CLI

to run in Docker in its simplest form just run:

`docker run -it -p 1880:1880 -v node_red_data:/data --device /dev/i2c-1:/dev/i2c-1 --name node-red:998 nodered/node-red`

the `--device` can mount device to container, and `--name` can add an user with specified group.

before add node-red user to the local i2c group, you need to get the group number via running command below on your host:

cat /etc/group | grep i2c | awk -F: '{print $3}'

#### 3.2 Running under Docker Portainer

if you try to run a node-red container with Docker Portainer, you also need to do some configuration similar in the `advanced container settings`

![dev](assets\dev_mount.png)

![user_setting](assets\user_setting.png)
