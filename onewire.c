/**
*	Functions for the One Wire bus
*	Officials timing taken from http://www.maximintegrated.com/en/app-notes/index.mvp/id/126
**/

#include "onewire.h"

/*	Utility functions	*/
void bus_low(void)
{
	OW_BUS_DDR  |= OW_BUS_PIN_MASK;
	OW_BUS_PORT &= ~OW_BUS_PIN_MASK;
}

void bus_release(void)
{
	OW_BUS_PORT |= OW_BUS_PIN_MASK;		// Enable the pull up resistor, or set the bus high
	OW_BUS_DDR  &= ~OW_BUS_PIN_MASK;	// Make the pin as input
}

uint8_t bus_read(void)
{
	return (OW_BUS_PIN & OW_BUS_PIN_MASK);
}

/*	One Wire functions	*/
uint8_t ow_reset(void)
{
	// Wait until the bus is high
	bus_release();
	while( !(OW_BUS_PIN & OW_BUS_PIN_MASK));
	uint8_t r = 0;

	bus_low();
	_delay_us(10*T);
	bus_release();
	_delay_us(2*T);
	r = !bus_read();
	_delay_us(6*T);

	return r;
}

void ow_write_bit(uint8_t bit)
{
	bus_low();
	if(bit)
	{
		_delay_us(T/6);		// Logic 1
		bus_release();
		_delay_us(5*T/6);	// Finish time slot
	}
	else
	{
		_delay_us(T);		// Logic 0
		bus_release();
	}
}

uint8_t	ow_read_bit(void)
{
	int8_t r;

	bus_low();
	_delay_us(T/6);
	bus_release();
	_delay_us(T/12);
	r = bus_read();
	_delay_us(T/12+T/6+T/2);

	return r;
}

void ow_write_byte(uint8_t byte)
{
	uint8_t i;
	for(i = 0; i < 8; i++)
	{
		if (byte & _BV(i))  	ow_write_bit(1);
		else					ow_write_bit(0);
	}
}

uint8_t	ow_read_byte(void)
{
	uint8_t data = 0;
	uint8_t i;

	// OneWire is LSB first
	for( i = 0; i<8 ; i++)
	{
		if ( ow_read_bit() )  	data |= _BV(i);
		else					data &= ~_BV(i);
	}
	return data;
}

void ow_skip_rom(void)
{
	ow_write_byte(0xcc);
}

void ow_read_rom(void)
{
	ow_write_byte(0x33);
}

void ow_read_power_supply(void)
{
	ow_write_byte(0xb4);
}