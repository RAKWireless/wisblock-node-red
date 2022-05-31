

# Getting Started with Running Node-RED under Docker  

[TOC]

## 1. Introduction

This guide explains how to run Node-RED under docker, as well as some other permission-related issues (GPIO, I2C, et cetera) inside containers. 



## 2. GPIO

### 2.1. Get gpiochip number and pin number 

Whether you are going to use Node-RED locally or inside container, we highly recommend you to install a package called **gpiod** on your host machine to help you identify the gpiochip number and also interact with Linux GPIO character device if necessary. Run the following command on your host machine to install **gpiod**:

```
sudo apt install gpiod
```

Then reboot your Raspberry Pi or the RAK7391. After reboot, we can execute `gpiodetect` to detect existing or new gpio chips.

```plaintext
sudo gpiodetect
```

You should be able to detect some gpio chips:

```
rak@rakpios:~ $ gpiodetect
gpiochip0 [pinctrl-bcm2835] (54 lines)
gpiochip1 [raspberrypi-exp-gpio] (8 lines)
```

Notice that `gpiochip0` is for the native 40-pin header. 

Sometimes we need to enable/reset a pin of the chip connected to Raspberry Pi. For example, when we use RAK12009 (or RAK12004) to read the PPM of smoke/alcool gas, we need to enable the built-in ADC121C021 chip first. The Enable pin on ADC121C021 chip need to be pulled to high to work. Based on your hardware setup, when you connect the RAK12009 to the IO slot1 on RAK6421, the ENABLE pin is GPIO 12 (board pin 32). When using IO slot 2 on RAK6421, the ENABLE pin is GPIO 22 (board pin 15). 

You can run `gpioinfo` to check the status of GPIO 12 and GPIO 22 to see whether they are used or not.

```
rak@rakpios:~ $ gpioinfo
gpiochip0 - 54 lines:
	line   0:     "ID_SDA"       unused   input  active-high 
	line   1:     "ID_SCL"       unused   input  active-high 
	line   2:       "SDA1"       unused   input  active-high 
	line   3:       "SCL1"       unused   input  active-high 
	...
	line  12:     "GPIO12"       unused   input  active-high
	...
	line  22:     "GPIO22"       unused   input  active-high 
	...
```

Now you know the gpiochip number and also the pin number, you will need this information for mounting the correct devices to Node-RED container and also configuring [node-red-contrib-libgpiod](https://flows.nodered.org/node/node-red-contrib-libgpiod).

### 2.2. Running Node-RED container

#### 2.2.1. Using Docker Command Line

To run Node-RED in Docker and use the latest Node-RED **official** image, the easiest way is run the following command:

```plaintext
docker run -it -p 1880:1880 -v node_red_data:/data --device /dev/gpiochip0:/dev/gpiochip0 --restart=unless-stopped --user node-red:997 --name NodeRed nodered/node-red
```

The `--device` can mount device to container, `gpiochip0`means the native GPIOs on Raspberry Pi. If you have a GPIO expander connected to your Raspberry Pi, or RAK7391 (comes with two on-board GPIO expanders), you need to change it, for example `gpiochip2`. Check section 2.1 to see how to get the name of the gpiochip. 

The `--name` can add an user with specified group. Before add node-red user to the local **gpio** group, you need to verify the group number via running command below on your host:

```
cat /etc/group | grep gpio | awk -F: '{print $3}'
```

On a Raspbian-based-image, the group id of GPIO is usually defined as 997.

Once the container is up, there is one more thing you need to do, and this step is critical. Since the offical Node-RED image doesn't have **libgpiod-dev** pre-installed, users have to install it manually inside the container, otherwise, users won'e be able to install the **node-red-contrib-libgpiod** node  :

```
apk add libgpiod-dev
```

Or, you can use a [customized Node-RED image provided by RAKwireless](link-to-docker-hub), it comes with the **libgpiod-dev** package installed:

```
docker run -it -p 1880:1880 -v node_red_data:/data --device /dev/gpiochip0:/dev/gpiochip0 --restart=unless-stopped --user node-red:997 --name NodeRed sheng2216/nodered-docker:1.1
```

#### 2.2.2. Using Docker Compose

If you are going to use the Node-RED docker container, you can bring up the service by using the docker-compose.yml file provided below:

```
version: '3.7'

services:

  nodered:
    image: sheng2216/nodered-docker:1.1
    container_name: NodeRed
    user: node-red
    group_add:
      - 997
    restart: unless-stopped
    devices:
      - "/dev/gpiochip0:/dev/gpiochip0"
    volumes:
      - 'node-red-data:/data'
    ports:
      - "1880:1880"

volumes:
  node-red-data:
```

To bring up the service, save the above file into a file called **docker-compose.yml**, and in the same directory, run `docker-compose up`. To stop the service, just press **ctrl+c** to exit and then run `docker-compose down` to stop the services defined in the Compose file, and also remove the networks defined.

If you want to use the latest official image from Node-RED, feel free to change the image defined in the docker-compose file to the offical one: `nodered/node-red`. But don't forget to install **libgpiod-dev** once the container is up (check section 2.2.1). Then you need to install **node-red-contrib-libgpiod**.



## 3. I2C 

In some applications, the sensor/modules we used supports I2C protocol, thus we need to make sure we have access to I2C devices outside containers, to be more specific, you need to make sure the Node-RED user inside container has access to the i2c bus (/dev/i2c-1 by default) on your host machine. 

If you are running Node-RED using docker, you need to mount `/dev/i2c-1` device to the Node-RED container using the docker command we provided below;  If you use the Portainer template provided by us, you don't need to change anything, as we have already mounted the device for you.

#### 3.1. Using Docker Command Line

To run Node-RED in Docker and use the latest Node-RED **official** image, the easiest way is run the following command:

```plaintext
docker run -it -p 1880:1880 -v node_red_data:/data --device /dev/i2c-1:/dev/i2c-1 --restart=unless-stopped --user node-red:998 --name NodeRed nodered/node-red
```

The `--device` can mount device to container, `/dev/i2c-1`means the I2C bus 1, and typically, bus 1 is the default one used by Raspberry Pi, but you can adjust it any bus you need. 

The `--name` can add an user with specified group. Before add node-red user to the local **gpio** group, you need to verify the group number via running command below on your host:

```
cat /etc/group | grep i2c | awk -F: '{print $3}'
```

On a Raspbian-based-image, the group id of i2c is usually defined as 998.

Or, you can use a [customized Node-RED image provided by RAKwireless](link-to-docker-hub), it comes with the **libgpiod-dev** package installed,so you don't need to worry GPIO permission issue if you happen to need access to GPIO devices:

```
docker run -it -p 1880:1880 -v node_red_data:/data --device /dev/i2c-1:/dev/i2c-1 --restart=unless-stopped --user node-red:998 --name NodeRed sheng2216/nodered-docker:1.1
```

#### 3.2. Using Docker Compose

If you are going to use the Node-RED docker container, you can bring up the service by using the docker-compose.yml file provided below:

```
version: '3.7'

services:

  nodered:
    image: sheng2216/nodered-docker:1.1
    container_name: NodeRed
    user: node-red
    group_add:
      - 998
    restart: unless-stopped
    devices:
      - "/dev/i2c-1:/dev/i2c-1"
    volumes:
      - 'node-red-data:/data'
    ports:
      - "1880:1880"

volumes:
  node-red-data:
```

To bring up the service, save the above file into a file called **docker-compose.yml**, and in the same directory, run `docker-compose up`. To stop the service, just press **ctrl+c** to exit and then run `docker-compose down` to stop the services defined in the Compose file, and also remove the networks defined.

If you want to use the latest official image from Node-RED, feel free to change the image defined in the docker-compose file to the offical one: `nodered/node-red`. 



## 4. Serial port

In some applications, the sensor/modules we used supports serial port protocol , thus we need to make sure we have access to Serial port devices outside containers, to be more specific, you need to make sure the Node-RED user inside container has access to serial port on your host machine. 

#### 4.1. Using Docker Command Line

For RAK7391, to run in Docker in its simplest form just run:

```plaintext
docker run -it -p 1880:1880 -v node_red_data:/data --name NodeRed --device /dev/ttyUSB0:/dev/ttyUSB0 --device /dev/ttyUSB1:/dev/ttyUSB1 --user node-red:dialout nodered/node-red
```

For raspberry pi 4B, to run in Docker in its simplest form just run:

```
docker run -it -p 1880:1880 -v node_red_data:/data --name NodeRed --device /dev/ttyS0:/dev/ttyS0 --user node-red:dialout nodered/node-red
```

In the command above, the `--device` can mount serial device to container, and `--name` can add an user with specified group.

#### 4.2. Using Docker Compose

If you are going to use the Node-RED docker container, you can bring up the service by using the docker-compose.yml file.

For RAK7391:

```
version: "3.9"

services:

  nodered:
    image: nodered/node-red
    container_name: NodeRed
    user: node-red
    group_add:
      - dialout
    restart: unless-stopped
    volumes:
        - 'node_red_data:/data'
    ports:
        - "1880:1880/tcp"
    devices:
        - "/dev/ttyUSB0:/dev/ttyUSB0"
        - "/dev/ttyUSB1:/dev/ttyUSB1"

volumes:
  node_red_data:

```

For Raspberry pi 4B:

```
version: "3.9"

services:

  nodered:
    image: nodered/node-red
    container_name: NodeRed
    user: node-red
    group_add:
      - dialout
    restart: unless-stopped
    volumes:
        - 'node_red_data:/data'
    ports:
        - "1880:1880/tcp"
    devices:
        - "/dev/ttyUSB0:/dev/ttyS0"

volumes:
  node_red_data:
```

To bring up the service, save the above file into a file called **docker-compose.yml**, and in the same directory, run `docker-compose up`. To stop the service, just press **ctrl+c** to exit and then run `docker-compose down` to stop the services defined in the Compose file, and also remove the networks defined.

If you want to use the latest official image from Node-RED, feel free to change the image defined in the docker-compose file to the offical one: `nodered/node-red`.



## 5. Portainer

Portainer is a self-service container service delivery platform. If you have Portainer installed on your Raspberry Pi or RAK7391,  you can use the [Portainer template](link-to-the-portainer-template) provided by RAKwireless. In this case, you won't need to make any changes to the configurations, just deploy a Node-Red container using the template (shown below):

![image-20220304093748592](assets/portainer-node-red.png)



## 6. All-in-One

#### 5.1. Using Docker Command Line

When you need access to multiple interfaces, for example, i2c and GPIO at the same time, you can use the following `docker-run` command to use the official Node-RED image.

For RAK7391:

```
docker run -it -p 1880:1880 -v node_red_data:/data --device /dev/gpiochip0:/dev/gpiochip0 --device /dev/i2c-1:/dev/i2c-1 --device /dev/ttyUSB0:/dev/ttyUSB0 --device /dev/ttyUSB1:/dev/ttyUSB1 --restart=unless-stopped --user node-red:997 --group-add 998 --name NodeRed nodered/node-red 
```

For raspberry pi 4B:

```
docker run -it -p 1880:1880 -v node_red_data:/data --device /dev/gpiochip0:/dev/gpiochip0 --device /dev/i2c-1:/dev/i2c-1  --device /dev/ttyS0:/dev/ttyS0 --restart=unless-stopped --user node-red:997 --group-add 998 --name NodeRed nodered/node-red
```

 

#### 5.2. Using Docker Compose

 You can also use the docker-compose file.

For RAK7391:

```
version: '3.7'

services:

  nodered:
    image: sheng2216/nodered-docker:1.1
    container_name: NodeRed
    user: node-red
    group_add:
      - 997
      - 998
    restart: unless-stopped
    devices:
      - "/dev/gpiochip0:/dev/gpiochip0"
      - "/dev/i2c-1:/dev/i2c-1"
      - "/dev/ttyUSB0:/dev/ttyUSB0"
      - "/dev/ttyUSB1:/dev/ttyUSB1"
    volumes:
      - 'node-red-data:/data'
    ports:
      - "1880:1880"

volumes:
  node-red-data:
```

For raspberry pi 4B:

```
version: '3.7'

services:

  nodered:
    image: sheng2216/nodered-docker:1.1
    container_name: NodeRed
    user: node-red
    group_add:
      - 997
      - 998
    restart: unless-stopped
    devices:
      - "/dev/gpiochip0:/dev/gpiochip0"
      - "/dev/i2c-1:/dev/i2c-1"
      - "/dev/ttyS0:/dev/ttyS0"
    volumes:
      - 'node-red-data:/data'
    ports:
      - "1880:1880"

volumes:
  node-red-data:
```



## 7. License

This project is licensed under MIT license.
