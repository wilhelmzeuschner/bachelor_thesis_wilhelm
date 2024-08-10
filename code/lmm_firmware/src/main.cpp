/*
This file is part of the LMM-Retrofit project.

Created on: 09. Jun 2022
Author: Hans Wilhelm Zeuschner

Bachelorarbeit:
	"Retrofit eines modernen
	Luftmassenmessers in einen Ottomotor
	mit Bosch L-Jetronic ECU unter
	Verwendung eines Mikrocontrollers"


Fachhochschule Emden/Leer

Studiengang:
Elektrotechnik im Praxisverbund
Sommersemester 2022
Bearbeitungszeitraum: 14.04.2022 bis 22.07.2022

Betreuer: Prof. Dr. Carsten Koch
Zweitprüfer: B. Eng. Ewald Zimmermann
*/

#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include <avr/wdt.h>

#include "lmm_pins.h"
#include "lmm_basic_io.h"
#include "lmm_uart_config.h"
#include "lmm_dac_sig.h"
#include "lmm_digi_pot.h"
#include "lmm_electronic_sensor.h"
#include "lmm_config.h"

/*
 *  \mainpage LMM-Retrofit Firmware
 *
 *  <HR>
 *
 *  \section sec1 Overview
 *
 *  <HR>
 *
 *   \author	Hans Wilhelm Zeuschner
 *   \date		19.06.2022
 *   \version 	1
 */

// Global variables
volatile uint8_t btnPressState = 0;

void setup()
{
	uint8_t ledState = 0;
	uint16_t airADCVal = 0;
	uint8_t activeLMMSlot = EEPROM.read(CONFIG_EEPROM_SLOT_ADDR);

	unsigned int timer = 0;

	volatile unsigned int runtimeCounter = 0;

	Wire.begin();
	wdt_enable(WDTO_15MS); // Initialize watchdog timer to trip after 15ms
	initIOPins();
	initUART(&activeLMMSlot);
	setLEDs(&ledState); // Turn LEDs off

	// Main program loop
	while (1)
	{
		runtimeCounter++;
		wdt_reset(); // Reset watchdog timer

		checkForLMMErrors(&ledState);

		readElectrSensAir(&airADCVal);

		// Serial.println(airADCVal);
		setSIGVoltage(calcSIGVoltageFromADC(&airADCVal, &activeLMMSlot));

		handleButtons(&activeLMMSlot, &ledState);
		setRelay(&airADCVal);

		if (runtimeCounter == 1000)
		{
			runtimeCounter = 0;
			// Toggle Pin D13
			digitalWrite(13, !digitalRead(13));
		}
	}
}

// Loop is not used since it's slightly slower than a while() loop
void loop()
{
}