/**
*	Functions definitions for the One Wire bus
**/

#ifndef ONEWIRE_H_
#define ONEWIRE_H_
	#include "config.h"

    #include <avr/io.h>
    #include <util/delay.h>

    #define T 60 // Standard time slot


	/*	Utility functions	*/
	void	bus_low(void);
	void	bus_release(void);
	uint8_t	bus_read(void);

	/*	One Wire functions	*/
	uint8_t	ow_reset(void); // Resets the bus. Returns 1 if a device was found, 0 otherwise

	void	ow_write_bit(uint8_t bit);
	uint8_t	ow_read_bit(void);
	void	ow_write_byte(uint8_t byte);
	uint8_t	ow_read_byte(void);

	void	ow_skip_rom(void);
	void	ow_read_rom(void);
	void	ow_read_power_supply(void);
#endif
