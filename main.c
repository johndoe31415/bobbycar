#include <stdio.h>
#include <stdbool.h>

#include "system.h"

static void delay(uint32_t duration) {
	volatile uint32_t ctr = duration;
	while (ctr--);
}

int main(void) {
	while (true) {
		led_green_toggle();
		delay(1000000);

		led_orange_toggle();
		delay(1000000);

		led_red_toggle();
		delay(1000000);
	}
}
