# WisBlock examples using NodeRED

A collection of examples to interface WisBlock modules and sensors from [Node-RED](http://nodered.org).

These examples are meant to be used with RAK7391 (WisGate Developer Connect) and RAK6421 (WisBlock Pi Hat) boards.

## Installation

Every example has a README.md file with the information about:

* required modules
* special permissions (specially if you run NodeRED in a docker container)
* hardware and firmware setup

You will also have a `flow.json` file with the example code. You just have to import it in your instance of NodeRED (top right menu > import > select a fie to import).

## Contributing

For simple typos and single line fixes please just raise an issue pointing out our mistakes. 

If you need to raise a pull request please read our [contribution guidelines](https://github.com/RAKwireless/wisblock-node-red/blob/master/CONTRIBUTING.md) before doing so.

## Copyright and license

Copyright (c) 2022 RAKwireless, under MIT License.

# Examples

The repository structure follows that on the [RAKwireless store](https://store.rakwireless.com/pages/wisblock)

* Interface
    * [RAK5801 example](interface/rak5801/)
    * [RAK5802 ModBUS example](interface/rak5802/rak5802_modbus/)
    * [RAK5811 example](interface/rak5811/)
    * [RAK13004 example](interface/rak13004/)
    * [RAK16001 example](interface/rak16001/)
* Other
    * [ADS1115 example](other/ads1115/ads1115-read/)
    * [GPIO toggle LED example](other/gpio/gpio-toggle-led/)
    * [I2C EEPROM example](other/i2c/i2c-eeprom/)
    * [PI4IOE5V96224 toggle LED example](other/pi4ioe5v/pi4ioe5v-toggle-led/)
* Sensors
    * [RAK12004 example](sensors/rak12004/rak12004-reading)
    * [RAK12015 example](sensors/rak12015/rak12015-tampering-detector)
    * [RAK16000 example](sensors/rak16000)
    * [SHTC3 example](sensors/shtc3/shtc3-read)


