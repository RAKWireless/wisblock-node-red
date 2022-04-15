# Measure lux and uvi using WisBlock UV sensor RAK12019 from Node-RED 

[TOC]

## 1 Introduction

This guide explains how to use the [WisBlock UV Sensor RAK12019](https://docs.rakwireless.com/Product-Categories/WisBlock/RAK12019/Overview/) in combination with RAK6421 Wisblock Hat or RAK7391 WisGate Developer Connect to measure lux and uvi(ultraviolet index) through the I2C interface using Node-RED.  

### 1.1 RAK12019

The RAK12019 is an Ambient Light sensor (ALS) or Ultraviolet Light Sensor (UVS), which is part of the RAKwireless WisBlock sensor series. The measured ambient light intensity and ultraviolet index are interfaced via the I2C bus making it immune to electrical noises, unlike its analog output counterpart. This module utilizes the LTR-390UV-01 sensor from Lite-On. For more information about RAK12019, refer to the [Datasheet](https://docs.rakwireless.com/Product-Categories/WisBlock/RAK12004/Datasheet/).

### 1.2 LTR-390UV-01

The LTR-390UV-01 is an integrated low voltage I2C  ambient aight sensor(ALS) and  ultraviolet light sensor(UVS) in a single miniature 2x2mm ChipLED lead-free surface mount package. This sensor converts light intensity to a digital output signal capable of direct I2C interface. For more information about LTR-390UV-01, refer to the [Datasheet](https://optoelectronics.liteon.com/upload/download/DS86-2015-0004/LTR-390UV_Final_%20DS_V1%201.pdf). 

## 2 Preparation


### 2.1 Access setup

Ensure you have access to both I2C devices when using the sensor. The ltr-uv390 chip on RAK12019 supports I2C protocol, if you are using Node-RED in the host machine directly (without using the docker container), you won't need to change anything, just make sure the Node-RED user has access to the i2c bus (/dev/i2c-1 by default) on your host machine. 

If running Node-RED using docker, you need to mount `/dev/i2c-1` device to the Node-RED container using the docker command we provided below, If you use the Portainer template provided by us, you don't need to change anything, as we have already mounted the device for you.

#### 2.1.1 Docker compose

 The example docker-compose is provided below:

```
version: '3.9'

services:

  nodered:
    image: sheng2216/nodered-docker:1.0
    container_name: NodeRed
    user: node-red
    group_add:
      - 998
    ports:
      - "1880:1880"
    restart: unless-stopped
    volumes:
      - 'node-red-data:/data'
    devices:
      - "/dev/i2c-1:/dev/i2c-1"
    networks:
      - node-red

volumes:
  node-red-data:

networks:
  node-red:
```

To bring up the service, save the above file into a file called **docker-compose.yml**, and in the same directory, run `docker-compose up`. To stop the service, just press **ctrl+c** to exit and then run `docker-compose down` to stop the services defined in the Compose file, and also remove the networks defined.

In the docker-compose file provided above, the --device can mount the device to the container, --group-add 998 adds the I2C group (group id 998 in Rakpios) to run as. Notice that **998** in the compose file needs to be changed if you are not using Rakpios, it needs to match your system group setup. Before adding the node-red user to the i2c group, you need to get the group number via running the command below on your host:

```
cat /etc/group | grep i2c | awk -F: '{print $3}'
```

#### 2.1.2 Running under Docker Portainer

If you try to run a Node-RED container with Docker Portainer using the template provided by RAKwireless, you won't need to make any changes to the configurations, just deploy the Node-RED container using the template (shown below), 

![image-20220304093748592](assets/portainer-node-red.png)

in the template, we defined a customized Node-RED docker image for you to use, so you don't need to worry about the configuration or permission anymore. After the app is deployed, you can browse to http://{host-ip}:1880 to access Node-Red's web interface.

### 2.2 Hardware preparation 

The easiest way to set up the hardware is to use the RAK6421 WisBlock Hat that exposes all the Wisblock high-density connector pins.  The RAK12019 can be mounted to the HAT, and the HAT goes to the 40-pin headers located on Raspberry Pi 4B/IO board/RAK7391. Based on your hardware selections, there are three ways to mount RAK12019:

1. Raspberry Pi model B + RAK6421 WisBlock Hat +  RAK12019

   <img src="assets/mount-on-raspberrypi-4b.jpg" alt="mouned on Raspberry 4B" style="zoom:67%;" />

2. Raspberry Pi CM4 + Compute Module 4 IO Board + RAK6421 WisBlock Hat + RAK12019

   <img src="assets/mount-on-IO-board.jpg" alt="mouned on Compute Module 4 IO Board" style="zoom:67%;" />

3. Raspberry Pi CM4  + RAK7391 WisGate Developer Connect + RAK6421 WisBlock Hat + RAK12019

   <img src="assets/mount-on-RAK7391.jpg" alt="mouned on RAK7391" style="zoom:67%;" />





## 3 Flow configuration

Whether you are using the Node-Red docker image provided by RAKwireless or the official latest image, or you host your Node-RED service on your host machine, you need to install the node `node-red-node-ltr-390uv` before you deploy the flow. 

### 3.1 Install nodes  

While the `node-red-contrib-ltr-390uv` hasn't been published, so you need to install it from our gitlib repository. Please install `node-red-contrib-ltr` node with the following commands. If you are using docker for Node-RED, you may need to replace `~/.node-red` with `/usr/src/node-red`,

```
git clone -b dev https://git.rak-internal.net/product-rd/gateway/wis-developer/rak7391/node-red-nodes.git
```

then copy `node-red-contrib-ltr-390uv directory  to  the `node_modules` directory,

```
cp -rf node-red-nodes/node-red-contrib-ltr-390uv ~/.node-red/node_modules
```

lastly, change to the `node-red-contrib-ltr-390uv` directory and install the node, 

```
cd ~/.node-red/node_modules/node-red-contrib-ltr-390uv && npm install
```

**Tips:**  After the installation of  `node-red-contrib-ltr-390uv`  is finished, please restart your node-red service/container(s).  Otherwise, the node cannot be found/added to the new flow.

### 3.2 Deploy the Example Flow 

After you deploy the NodeRED container,  you can import  [**rak12019-reading.json**](rak12019-reading.json) flow. This is a very basic flow and it uses five sets of nodes: `inject` node, `ltr-390uv` node,  and  `debug` node. After the import is done, the new flow should look like this:

<img src="assets/flow-overview.png" alt="flow-overview" style="zoom:67%;" />

### 3.3 Nodes Configurations 

To get the lux and uvi reading from the RAK12019,  you need to select the correct setting for `node-red-contrib-ltr-390uv` node.

<img src="assets/ltr-390uv-setting.png" alt="ltr-390uv-setting" style="zoom:67%;" />

**Name**

Define the msg name if you wish to change the name displayed on the node.

**/dev/i2c-?**

The i2c bus number, the default value is `1` , it means `'/dev/i2c-1'`.

**i2c_Address**

The i2c slave address for the RAK12019, by default is set to `0x53`.

**gain**

Define the als/uvs measuring gain range. the default value is `1x`

**resolution**

Define the als/uvs measuring resolution, the default value is `16 Bit`

### 3.4 Flow output

This flow outputs the measuring result with `debug` node every 2 seconds, the output of the node is a payload contains the raw als data, raw uvs data,  the calculated lux and the calculated uvi.

<img src="assets/debug-node.png" alt="debug-node" style="zoom:67%;" />

## License

This project is licensed under MIT license.
