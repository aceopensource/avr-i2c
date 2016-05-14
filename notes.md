## TWI Notes

### ETC

* Last byte read by master has NACK, otherwise the slave (pn532) won't hang up, and refuses a new start code.
* Maybe this can be fixed with a repeated start?

### TWSR Status Register

TWSR Status Codes (m328p datasheet p218)

* 0x08 - START condition has been transmitted
* 0x10 - Repeated START condition has been transmitted
* 0x18 - SLA+W transmitted; ACK received
* 0x20 - SLA+W transmitted; NACK received
* 0x28 - Data byte transmitted; ACK received
* 0x30 - Data byte transmitted; NACK received
* 0x38 - Arbitration lost in SLA+W or data transmit

### Bit Rate

To calculate bit rate:
Start with the datasheet's formula (p212):

SCL Frequency *F_SCL* = (F_CPU) / (16 + 2(TWBR) * Prescaler)

With some algebra, this reduces to:

TWBR = (1 / (Prescaler * 2)) * ((F_CPU/F_SCL) - 16)