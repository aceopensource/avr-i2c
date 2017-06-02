/**
 * I2C Master Library
 * Christopher Bero <berocs@acedb.co>
 */

#include "avr-i2c.h"

/**
 * TWSR Status Codes (m328p datasheet p218)
 * 0x08 - START condition has been transmitted
 * 0x10 - Repeated START condition has been transmitted
 * 0x18 - SLA+W transmitted; ACK received
 * 0x20 - SLA+W transmitted; NACK received
 * 0x28 - Data byte transmitted; ACK received
 * 0x30 - Data byte transmitted; NACK received
 * 0x38 - Arbitration lost in SLA+W or data transmit
 */

void i2c_init()
{
	// atmel Datasheet p232
	TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
	// atmel Datasheet p212
	TWBR = (uint8_t)TWBR_val;
}

// https://community.nxp.com/thread/322977
void i2c_recover()
{
//	DDRC |= (1 << PC5);
	DDRC &= ~(1 << PC5);
	DDRC &= ~(1 << PC4);
	PORTC &= ~(1 << PC4);
	PORTC &= ~(1 << PC5);
//	if(I2C_LOUD) printf("PortC: 0x%02X, DDRC: 0x%02X, PINC: 0x%02X\n", PORTC, DDRC, PINC);
//	printf("PortC: 0x%02X, DDRC: 0x%02X, PINC: 0x%02X\n", PORTC, DDRC, PINC);
#ifdef I2C_MASTER_DEBUG
	printf("PortC: 0x%02X, DDRC: 0x%02X, PINC: 0x%02X\n", PORTC, DDRC, PINC);
#endif
//	while (!(PINC & (1<<PINC4)))
//	{
//		if(I2C_LOUD) printf("SDA line still low\n");
//		PINC = (1<<PINC5); // auto XOR output by writing a 1
//	}
}

// Soft reset http://www.microchip.com/forums/m487486.aspx
//i2c_start(PN532_I2C_ADDRESS | I2C_WRITE);
//i2c_write(1);
//i2c_write(1);
//i2c_write(1);
//i2c_write(1);
//i2c_write(1);
//i2c_write(1);
//i2c_write(1);
//i2c_write(1);
//i2c_write(1);
//i2c_start(PN532_I2C_ADDRESS | I2C_WRITE);
//i2c_stop();

// http://www.avrfreaks.net/forum/twi-reset-when-communication-no-response
//void i2c_recover()
//{
//	if(I2C_LOUD) printf("Recover i2c.\n");
//	uint8_t index;
//
//	TWSR = 0x00;
//	TWCR = 0x00;
//	DDRC |= (1 << PC5);
//	DDRC &= ~(1 << PC4);
//	PORTC &= ~(1 << PC4);
//	PORTC &= ~(1 << PC5);
//
//	for(index = 0; index < 9; index++)
//	{
//		PORTC |= (1 << PC5);
//		_delay_ms(1);
//		if (PINC & (1 << PC4)) // Look for SDA high
//		{
//			break;
//		}
//		PORTC &= ~(1 << PC5);
//	}
//
//	DDRC &= ~((1 << PC5) | (1 << PC4));
//	i2c_init();
//}

uint8_t i2c_start(uint8_t address, uint8_t mode)
{
	uint8_t timeout;

	// Check mode
	if (mode != I2C_READ && mode != I2C_WRITE)
	{
		return 1;
	}

	// reset TWI control register
	TWCR = 0;
	// transmit START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	// wait for end of transmission
	timeout = 0;

	while( !(TWCR & (1<<TWINT)) )
	{
		_delay_us(50);
		timeout++;
		if (timeout > 20)
		{
#ifdef I2C_MASTER_DEBUG
			printf("i2c_start: Interrupt timeout 1. Status: 0x%02X\n", TW_STATUS);
#endif
			return TW_STATUS;
		}
	}

	// check if the start condition was successfully transmitted
	if(TW_STATUS != TW_START)
	{
#ifdef I2C_MASTER_DEBUG
			printf("i2c_start: No start status. Status: 0x%02X\n", TW_STATUS);
#endif
		return TW_STATUS;
	}

	// load slave address into data register
	TWDR = ((address<<1) | mode);
	// start transmission of address
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	timeout = 0;

	while( !(TWCR & (1<<TWINT)) )
	{
		_delay_us(50);
		timeout++;
		if (timeout > 20)
		{
#ifdef I2C_MASTER_DEBUG
			printf("i2c_start: Interrupt timeout 2. Status: 0x%02X\n", TW_STATUS);
#endif
			return TW_STATUS;
		}
	}

	// check if the device has acknowledged the READ / WRITE mode
	if ( (TW_STATUS != TW_MT_SLA_ACK) && (TW_STATUS != TW_MR_SLA_ACK) )
	{
#ifdef I2C_MASTER_DEBUG
			printf("i2c_start: No ack status. Status: 0x%02X\n", TW_STATUS);
#endif
		return TW_STATUS;
	}

	return 0;
}

uint8_t i2c_write(uint8_t data)
{
	uint8_t timeout;
	// load data into data register
	TWDR = data;
	// start transmission of data
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	timeout = 0;

	while( !(TWCR & (1<<TWINT)) )
	{
		_delay_us(50);
		timeout++;
		if (timeout > 20)
		{
#ifdef I2C_MASTER_DEBUG
			printf("TWSR write() err: 0x%02X\n", TW_STATUS);
#endif
			return TW_STATUS;
		}
	}

	if( TW_STATUS != TW_MT_DATA_ACK )
	{
#ifdef I2C_MASTER_DEBUG
		printf("TWSR write() err: 0x%02X\n", TW_STATUS);
#endif
		return TW_STATUS;
	}

	return 0;
}

uint8_t i2c_read_ack(uint8_t * data)
{
	uint8_t timeout;
	// start TWI module and acknowledge data after reception
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	// wait for end of transmission
	timeout = 0;

	while( !(TWCR & (1<<TWINT)) )
	{
		_delay_us(50);
		timeout++;
		if (timeout > 20)
		{
#ifdef I2C_MASTER_DEBUG
			printf("TWSR read_ack() err: 0x%02X\n", TW_STATUS);
#endif
			return TW_STATUS;
		}
	}

	if (data)
		*data = TWDR;

	return 0;
}

uint8_t i2c_read_nack(uint8_t * data)
{
	uint8_t timeout;
	// start receiving without acknowledging reception
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	timeout = 0;

	while( !(TWCR & (1<<TWINT)) )
	{
		_delay_us(50);
		timeout++;
		if (timeout > 20)
		{
#ifdef I2C_MASTER_DEBUG
			printf("TWSR read_nack() done: 0x%02X\n", TW_STATUS);
#endif
			return TW_STATUS;
		}
	}

	if (data)
		*data = TWDR;

	return 0;
}

uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length)
{
	uint8_t err = 0;

	err=i2c_start(address, I2C_WRITE);
	if (err) return err;

	for (uint16_t i = 0; i < length; i++)
	{
		if (i2c_write(data[i]))
		{
#ifdef I2C_MASTER_DEBUG
			printf("TWSR transmit err: 0x%02X\n", TW_STATUS);
#endif
			return TW_STATUS;
		}
	}

	i2c_stop();

	return 0;
}

uint8_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length)
{
	uint8_t err = 0;

	err=i2c_start(address, I2C_READ);
	if (err) return err;

	for (uint16_t i = 0; i < (length-1); i++)
	{
		err=i2c_read_ack(data+i);
		if (err) return err;
	}
	err=i2c_read_nack(data+(length-1));
	if (err) return err;

	i2c_stop();

	return 0;
}

uint8_t i2c_writeReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length)
{
#ifdef I2C_MASTER_DEBUG
//	printf("i2c, begin writeReg\n");
//	printf("\tDeviceAddr: 0x%02X\n\tRegAddr: 0x%02X\n\tData: 0x%02X\n\tLen: 0x%02X\n", devaddr, regaddr, *data, length);
#endif

	uint8_t err = 0;
	if ((err = i2c_start(devaddr, TW_WRITE))) return err;

	if ((err=i2c_write(regaddr))) return err;

	for (uint16_t i = 0; i < length; i++)
	{
		err=i2c_write(data[i]);
		if (err) return err;
	}

	i2c_stop();

	return 0;
}

uint8_t i2c_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length)
{
#ifdef I2C_MASTER_DEBUG
//	printf("i2c, begin readReg\n");
//	printf("\tDeviceAddr: 0x%02X\n\tRegAddr: 0x%02X\n\tLen: 0x%02X\n", devaddr, regaddr, length);
#endif
	uint8_t err = 0;
	err=i2c_start(devaddr, TW_WRITE);
	if (err) return err;

	err=i2c_write(regaddr);
	if (err) return err;

	err=i2c_start(devaddr, TW_READ);
	if (err) return err;

	for (uint16_t i = 0; i < (length-1); i++)
	{
		err=i2c_read_ack(data+i);
		if (err) return err;
	}

	err=i2c_read_nack(data+(length-1));
	if (err) return err;

	i2c_stop();

	return 0;
}

void i2c_stop(void)
{
	// transmit STOP condition
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}
