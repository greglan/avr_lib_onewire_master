/*
 * Main file. It is assumed only one DS18B20 is connected to the bus.
 * See datasheet to connect the sensors.
 * The data pin can be changed in the config.h file
 * No crc verification is made.
 */

#include "config.h"
#include "onewire.h"
#ifdef DEBUG
    #include "uart.h" // uart lib
#endif
#include <stdio.h>

int main(void)
{
	uint8_t data[9] = {0};

	#ifdef DEBUG
		uart_init(BAUD_RATE);
	#endif

	for(;;){
		uint8_t i; // Counter

		// Init and select device
		if(ow_reset())
            ow_skip_rom();


		// Start the temp conversion
		ow_write_byte(0x44);

		// Give time for the temp capture
		_delay_ms(1000);

		// Select device
		if(ow_reset())
            ow_skip_rom();

		// Read Scratchpad
		ow_write_byte(0xbe);

		// Read the reply
		for (i = 0; i < 10; i++)
		{
			data[i] = ow_read_byte();
		}

		// Conversion fot temperature
		i = ( data[1] << 4 ) | ( data[0] >> 4);

		// Prints out the measured temp
		#ifdef DEBUG
			sprintf(data, "%d", i);
			uart_tx_str(data);
			uart_newline();
		#endif

		// Select device
		if(ow_reset())
			ow_skip_rom();

		// Prints the config of the probe
		#ifdef DEBUG
			sprintf(data, "%d %d %d", data[2], data[3], data[4]);
			uart_tx_str(data);
			uart_newline();
		#endif

		// Read power supply
		ow_read_power_supply();
		i = ow_read_bit();

		#ifdef DEBUG
			uart_tx_char(i + 0x30); // ASCII offset
			uart_newline();
			uart_newline();
		#endif

		// Wait a little.
		_delay_ms(1000);
	}
	return 1;
}
