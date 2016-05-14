/**
 * I2C Master Library
 * Christopher Bero <berocs@acedb.co>
 */

#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#ifndef  F_CPU
#	error i2c_master.h needs you to define F_CPU
#endif

#include <avr/io.h>
#include <util/twi.h>
#include <stdint.h>
#include <util/delay.h>

#define F_SCL 100000UL // SCL frequency
#define Prescaler 1 // TWPS0 = TWPS1 = 0x0
#define TWBR_val ( ( (F_CPU / F_SCL) - 16 ) / (Prescaler * 2) )

#define I2C_READ 0x01
#define I2C_WRITE 0x00

void i2c_init(void);
uint8_t i2c_start(uint8_t address);
uint8_t i2c_write(uint8_t data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);
uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length);
uint8_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length);
uint8_t i2c_writeReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length);
uint8_t i2c_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length);
void i2c_stop(void);

#endif // I2C_MASTER_H
