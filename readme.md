# I2C Master Library

## Table of contents

1. What is this library?
2. Functions in this library
3. Links and References

------------------------------------------------------------------------------------

### 1. What is this library?
This library is an I2C master library which uses the TWI peripheral inside the
AVR microcontroller to establish connections using the I2C protocol.
It uses only avr-libc backends (no Arduino.h)

------------------------------------------------------------------------------------

### 2. Functions in this library

* void i2c_init(void)
* uint8_t i2c_start(uint8_t address)
* uint8_t i2c_write(uint8_t data)
* uint8_t i2c_read_ack(void)
* uint8_t i2c_read_nack(void)
* uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length)
* uint8_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length)
* void i2c_stop(void)

##### void I2C_init(void)
This function needs to be called only once to set up the correct SCL frequency
for the bus

##### uint8_t I2C_start(uint8_t address)
This function needs to be called any time a connection to a new slave device should
be established. The function returns 1 if an error has occurred, otherwise it returns
0.

The syntax to start a operation write to a device is either:
`I2C_start(SLAVE_ADDRESS+I2C_WRITE);`
or
`I2C_start(SLAVE_WRITE_ADDRESS);`

The syntax to start a read operation from a device is either:
`I2C_start(SLAVE_ADDRESS+I2C_READ);`
or
`I2C_start(SLAVE_READ_ADDRESS);`

##### uint8_t I2C_write(uint8_t data)
This function is used to write data to the currently active device.
The only parameter this function takes is the 8 bit unsigned integer to be sent.
The function returns 1 if an error has occurred, otherwise it returns
0.

##### uint8_t I2C_read_ack(void)
This function is used to read one byte from a device and request another byte of data
after the transmission is complete by sending the acknowledge bit.
This function returns the received byte.

##### uint8_t I2C_read_nack(void)
This function is used to read one byte from a device an then not requesting another
byte and therefore stopping the current transmission.
This function returns the received byte.

##### uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length);
This function is used to transmit [length] number of bytes to an I2C device with the given
I2C address from [data].
The [address] passed to this function is the 7-bit slave address, left
shifted by one bit (i.e. 7-bit slave address is `0x2F` -> `(0x2F)<<1` = `0x5E`)

##### uint8_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length);
This function is used to read [length] number of bytes from the I2C device with the given
I2C address into the [data].
The [address] passed to this function is the 7-bit slave address, left
shifted by one bit (i.e. 7-bit slave address is `0x2F` -> `(0x2F)<<1` = `0x5E`)

##### void I2C_stop(void)
This function disables the TWI peripheral completely
and therefore disconnects the device from the bus.

------------------------------------------------------------------------------------

### 3. Links and References

* [https://github.com/g4lvanix/I2C-master-lib](https://github.com/g4lvanix/I2C-master-lib)
	* Reference code.
* [http://www.embedds.com/programming-avr-i2c-interface/](http://www.embedds.com/programming-avr-i2c-interface/)
	* I2C overview and data transmission
* [http://homepage.hispeed.ch/peterfleury/avr-software.html](http://homepage.hispeed.ch/peterfleury/avr-software.html)
	* Peter Fleury's old website
* [http://homepage.hispeed.ch/peterfleury/doxygen/avr-gcc-libraries/group__pfleury__ic2master.html](http://homepage.hispeed.ch/peterfleury/doxygen/avr-gcc-libraries/group__pfleury__ic2master.html)
	* Peter Fleury's new website
* [http://web.engr.oregonstate.edu/~traylor/ece473/lectures/twi.pdf](http://web.engr.oregonstate.edu/~traylor/ece473/lectures/twi.pdf)
	* A nice TWI presentation
* [http://www.avrfreaks.net/forum/i2c-twint-never-gets-set-after-start-bit-sent](http://www.avrfreaks.net/forum/i2c-twint-never-gets-set-after-start-bit-sent)
	* Invaluable tip. Last read command over I2C has a NACK.
* [http://www.i2c-bus.org/i2c-primer/common-problems/](http://www.i2c-bus.org/i2c-primer/common-problems/)
	* Common troubleshooting
* [http://www.forward.com.au/pfod/ArduinoProgramming/I2C_ClearBus/index.html](http://www.forward.com.au/pfod/ArduinoProgramming/I2C_ClearBus/index.html)
	* An interesting function that's supposed to clear the I2C bus after it gets mucked-up.