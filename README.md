# defiant
This is the firmware for the modification to a learn-to-walk toy car, a [BIG
Bobby Car](https://www.big.de/en/products/big-bobby-car/), which I have built.
It's a toy for my toddler daughter. Using this modification, the car gets an
engine sound (including startup, idle and shutoff), headlights, turn signals
and a siren (Martinshorn, the German police/ambulance/fire engine siren).

## Hardware and Software
I'm running this on a custom development board I designed on an STM32F103C8
Cortex-M3 microcontroller. The schematics and Gerber files [are available as
well](https://github.com/johndoe31415/pcbs). To generate the HAL and
initialization code for the project, including the build system for the STM32
library, refer to the [mcuconfig
project](https://github.com/johndoe31415/mcuconfig).

The headlights are two WS2812 LEDs, which I drive pretty amateurishly using
locked interrupts and busy waiting. To create a high-speed 3.3V to 5V
conversion, I run the signal through a single-gate 74HC1G04GW inverter.

For audio playback, the device continuously reads samples from a Winbond 25Q64
SPI flash ROM in a double-buffered fashion using DMA. Then via a 11.025 kHz
interrupt, those samples are passed as the duty cycle values of second
high-speed timer. The high-speed timer output is then RCL-filtered and passed
to a class D amplifier (PAM8403), which drives the loudspeaker.

There's an 921600 baud USART serial terminal on PA9 and PA10, which initially
comes up as ASCII (a debugging frontend), but which can switch to full binary
mode. The `usartcomm` tool will use this binary interface to flash the flash ROM.

## Name
The car is named after the [USS Defiant,
NX-74205](https://en.wikipedia.org/wiki/USS_Defiant), because it's a [tough
little car](https://youtu.be/TOyOa3eExMk?t=8).

## License
GNU GPL-3.
