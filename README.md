# WisBlock examples using NodeRED

A collection of examples to interface WisBlock modules and sensors from [Node-RED](http://nodered.org).

These examples are meant to be used with RAK7391 (WisGate Developer Connect) and RAK6421 (WisBlock Pi Hat) boards.

# Hardware requirement

To be able to use the WisBlock module, the hardware must match the WisBlock form factor. Right now, RAKwireless provides two devices to support the WisBlock module for the Raspberry Pi platform. 

### RAK7391

RAK7391 is a powerful CM4 extension board. There are two WisBlock I/O connectors on the board already. Users can connect the WisBlock I/O module with RAK7391 directly. 

### RAK6421

RAK6421 is a Pi HAT board that includes the WisBlock connector. Users can put the WisBlock module on the Pi HAT and plug it on the Raspberry Pi board. 



## Installation

Every example has a README.md file with the information about:

* required modules
* special permissions (specially if you run NodeRED in a docker container)
* hardware and software setup

You will also have a `flow.json` file with the example code. You just have to import it in your instance of NodeRED (top right menu > import > select a fie to import).



## Examples

The repository structure follows that on the [RAKwireless store](https://store.rakwireless.com/pages/wisblock)

* display
  * [rak14003-example](display/rak14003-example)

* Interface
    * [RAK5801 example](interface/rak5801/)
    * [RAK5802 ModBUS example](interface/rak5802/rak5802_modbus/)
    * [RAK5811 example](interface/rak5811/)
    * [RAK13004 example](interface/rak13004/)
    * [RAK13005 example](interface/rak13005/)
    * [RAK13006 example](interface/rak13006/)
    * [RAK16001 example](interface/rak16001/)
* Other
    * [ADS1115 example](other/ads1115/ads1115-read/)
    * [GPIO toggle LED example](other/gpio/gpio-toggle-led/)
    * [I2C EEPROM example](other/i2c/i2c-eeprom/)
    * [PI4IOE5V96224 toggle LED example](other/pi4ioe5v/pi4ioe5v-toggle-led/)
    * [libgpiod example](other/libgpiod/libgpiod-blink/)
* Sensors
    * [RAK1902 example](sensors/rak1902/rak1902-read)
    * [RAK1903 example](sensors/rak1903/rak1903-read)
    * [RAK12004 example](sensors/rak12004/rak12004-reading)
    * [RAK12009 example](sensors/rak12009/rak12009-reading)
    * [RAK12015 example](sensors/rak12015/rak12015-tampering-detector)
    * [RAK12019 example](sensors/rak12019/12019-reading)
    * [RAK12037 example](sensors/rak12037)
    * [RAK16000 example](sensors/rak16000)
    * [RAK1901 example](sensors/rak1901/rak1901-shtc3-read)
* Wireless
    * [RAK13600 example](wireless/rak13600)



## Contributing

For simple typos and single line fixes please just raise an issue pointing out our mistakes. 

If you need to raise a pull request please read our [contribution guidelines](https://github.com/RAKwireless/wisblock-node-red/blob/master/CONTRIBUTING.md) before doing so.



## Copyright and license

Copyright (c) 2022 RAKwireless, under MIT License.
