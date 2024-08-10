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

#include "Arduino.h"
#include <EEPROM.h>

#include "lmm_pins.h"
#include "lmm_basic_io.h"
#include "lmm_uart_config.h"
#include "lmm_config.h"
#include "lmm_dac_sig.h"

extern volatile uint8_t btnPressState;

/*********************************************************************/
/**
 \brief Initializes all IO pins to their respective functions (input / output) and configures the relevant IO-interrupts

 \param   -

 \return  -

 **********************************************************************/
void initIOPins()
{
	// Outputs:
	pinMode(LED_1_PIN, OUTPUT);
	pinMode(LED_2_PIN, OUTPUT);
	pinMode(LED_ERROR_PIN, OUTPUT);
	pinMode(RELAY_PIN, OUTPUT);
	pinMode(13, OUTPUT);

	// Inputs:
	pinMode(SIG_SENSE_PIN, INPUT);
	pinMode(TEMP_SENSE_PIN, INPUT);
	pinMode(BATT_VSENSE_PIN, INPUT);
	pinMode(BTN_1_PIN, INPUT);
	pinMode(BTN_2_PIN, INPUT);

	// Interrupts
	attachInterrupt(digitalPinToInterrupt(BTN_1_PIN), buttonOneISR, FALLING);
	attachInterrupt(digitalPinToInterrupt(BTN_2_PIN), buttonTwoISR, FALLING);
}

/*********************************************************************/
/**
 \brief Initializes the UART port and sends some basic info about the program

 \param   -

 \return  -

 **********************************************************************/
void initUART(uint8_t *activeLMMSlot)
{
	Serial.begin(UART_SPEED);

	Serial.println(F("\n\n------------"));
	Serial.println(F(UART_PROJECT_NAME));
	Serial.print(F("Firmware Version: "));
	Serial.println(F(UART_FIRMWARE_VERSION));
	Serial.print(F("Firmware Release Date: "));
	Serial.println(F(UART_FIRMWARE_RELEASE_DATE));
	Serial.print(F("Author: "));
	Serial.println(F(UART_FIRMWARE_AUTHOR));
	Serial.println(F(UART_WEBSITE_URL));
	Serial.print(F("Compiled on: "));
	Serial.print(F(__DATE__));
	Serial.print(F(", "));
	Serial.println(F(__TIME__));
	Serial.print(F("Compiler Version: "));
	Serial.println(F(UART_COMPILER_VERSION));
	Serial.println(F(""));
	Serial.print(F("Loaded LMM Curve No. "));
	Serial.println(*activeLMMSlot);
	Serial.println(F("------------\n"));
}

/*********************************************************************/
/**
 \brief Handles the button presses
		Currently it's only used to select which profile for the characteristic curve is used
		There are 3 curves in total, see lmm_dac_sig.h for more information
		Button 2 advances to the next curve, Button 1 jumps back by one
		After the slot number has been changed, the LED blinks slot number-times

 \param   activeLMMSlot this is the currently active slot number

 \return  -

 \todo	Implement a guard against pressing the button (i.e. changing the profile) while the car is running? But where does the board get it's power from then?

 **********************************************************************/
void handleButtons(uint8_t *activeLMMSlot, uint8_t *ledState)
{
	static unsigned long blinkTimer = 0;
	static uint8_t blinkNumber = *activeLMMSlot * 2 + 2; // Set to activeLMMSlot so that the LEDs blink on boot

	// Check if the buttons were pressed
	// Set LED 1 to Off, if the button was just pressed
	// This ensures that it's always in a defined state and doesn't get stuck on "ON"
	// Button 2
	if (btnPressState & BTN_2_MASK)
	{
		if (*activeLMMSlot == (DAC_SLOTS_COUNT - 1))
		{
			*activeLMMSlot = 0;
		}
		else
		{
			*activeLMMSlot += 1;
		}
		blinkNumber = *activeLMMSlot * 2 + 2;
		// Serial.println("new slot:");
		Serial.println(*activeLMMSlot);

		btnPressState &= !BTN_2_MASK;
		blinkTimer = millis();

		*ledState &= !LED_1_BIT;
		setLEDs(ledState);

		EEPROM.write(CONFIG_EEPROM_SLOT_ADDR, *activeLMMSlot);
	}
	// Button 1
	else if (btnPressState & BTN_1_MASK)
	{
		if (*activeLMMSlot == 0)
		{
			*activeLMMSlot = (DAC_SLOTS_COUNT - 1);
		}
		else
		{
			*activeLMMSlot -= 1;
		}
		blinkNumber = *activeLMMSlot * 2 + 2;
		// Serial.println("new slot:");
		Serial.println(*activeLMMSlot);

		btnPressState &= !BTN_1_MASK;
		blinkTimer = millis();

		*ledState &= !LED_1_BIT;
		setLEDs(ledState);

		EEPROM.write(CONFIG_EEPROM_SLOT_ADDR, *activeLMMSlot);
	}

	// Blink LED 1
	if (blinkNumber && ((millis() - blinkTimer) >= LED_BLINK_SPEED_SLOT_CHANGE))
	{
		blinkNumber -= 1;
		*ledState ^= LED_1_BIT;
		setLEDs(ledState);
		blinkTimer = millis();
	}
}

/*********************************************************************/
/**
 \brief  Sets the LEDs On or Off

 \param   ledState: Bit 0 -> LED_1, Bit 1 -> LED_2, Bit 3 -> LED_ERR

 \return  -

 **********************************************************************/
void setLEDs(uint8_t *ledState)
{
	digitalWrite(LED_1_PIN, (*ledState & LED_1_BIT));
	digitalWrite(LED_2_PIN, (*ledState & LED_2_BIT));
	digitalWrite(LED_ERROR_PIN, (*ledState & LED_ERR_BIT));
}

/*********************************************************************/
/**
 \brief  measures the board's supply voltage in mV

 \param   -

 \return  -

 **********************************************************************/
uint16_t measureVBATTmV()
{
	uint32_t adcVal = 0;
	for (uint8_t i = 0; i < CONFIG_VBATT_AVG_COUNT; i++)
	{
		adcVal = adcVal + analogRead(BATT_VSENSE_PIN);
	}

	return ((((float)adcVal / (float)CONFIG_VBATT_AVG_COUNT) * (float)readVcc() / (float)CONFIG_ADC_MAX) * ((float)CONFIG_R1_VBATT + (float)CONFIG_R2_VBATT)) / (float)CONFIG_R2_VBATT;
}

/*********************************************************************/
/**
 \brief Calculates the supply voltage of the IC using the internal 1.1V reference. Source: https://code.google.com/archive/p/tinkerit/wikis/SecretVoltmeter.wiki
		This function is inherently slow because of the built in delay of 2 ms!
 \param   -

 \return  result: Supply voltage of the IC in mV

 \version -

 **********************************************************************/
long readVcc()
{
	long result; // Read 1.1V reference against AVcc
	ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
	delay(2);			 // Wait for Vref to settle
	ADCSRA |= _BV(ADSC); // Convert
	while (bit_is_set(ADCSRA, ADSC))
		;
	result = ADCL;
	result |= ADCH << 8;
	result = 1126400L / result; // Back-calculate AVcc in mV
	return result;
}

/*********************************************************************/
/**
 \brief ISR for a pushbutton, sets a global flag that is read by the main loop

 \param   -

 \return  -

 \version -

 **********************************************************************/
void buttonOneISR()
{
	btnPressState = btnPressState | BTN_1_MASK;
}

/*********************************************************************/
/**
 \brief  ISR for a pushbutton, sets a global flag that is read by the main loop

 \param   -

 \return  -

 \version -

 **********************************************************************/
void buttonTwoISR()
{
	btnPressState = btnPressState | BTN_2_MASK;
}

/*********************************************************************/
/**
 \brief Verifies that all conditions are met and the board can work correctly
		Blinks the ERR LED if errors are found
		This function needs to be called continuously, otherwise the ERR LED might get stuck on 1.

		Checklist:								if condition is NOT met:
			- input voltage within spec			return: bit 1 set

 \param   -

 \return  	0: no problems
			!= 0: issues present (see above)

 **********************************************************************/
uint8_t checkForLMMErrors(uint8_t *ledState)
{
	static unsigned long errorLEDTimer = 0;
	uint8_t state = 0;
	uint16_t vBATT = measureVBATTmV();

	if (vBATT < CONFIG_VBATT_NOMINAL_LOWER || vBATT > CONFIG_VBATT_NOMINAL_UPPER)
	{
		state |= 1;
		// Serial.println("VBATT error!");
	}

	if (state)
	{
		// Issues present -> Blink Error LED
		if (millis() - CONFIG_ERR_LED_BLINK_INTERVAL > errorLEDTimer)
		{
			errorLEDTimer = millis();
			*ledState ^= LED_ERR_BIT; // Flip Error LED bit
		}
	}
	else
	{
		// Turn Error LED OFF
		if (*ledState & LED_ERR_BIT)
		{
			*ledState &= !LED_ERR_BIT;
		}
	}

	setLEDs(ledState);

	return state;
}

/*********************************************************************/
/**
 \brief Turns the relay either on or off based on the measured airflow.
		The threshold value and hysteresis is set in lmm_config.h

 \param   airADCVal current ADC reading of the electronic sensor

 \return  -

 **********************************************************************/
void setRelay(uint16_t *airADCVal)
{
	uint8_t currRelayState = digitalRead(RELAY_PIN);

	//Serial.print("airADCVal: ");
	//Serial.println(*airADCVal);

	// Turn the relay on if the ADC value is above the CONFIG_RELAY_ADC_ON_VAL + CONFIG_RELAY_HYSTERESIS
	if (!currRelayState && *airADCVal >= (CONFIG_RELAY_ADC_ON_VAL + CONFIG_RELAY_HYSTERESIS))
	{
		digitalWrite(RELAY_PIN, HIGH);
	}
	// Turn the relay off if the ADC value is below the CONFIG_RELAY_ADC_ON_VAL - CONFIG_RELAY_HYSTERESIS
	else if (currRelayState && *airADCVal <= (CONFIG_RELAY_ADC_ON_VAL - CONFIG_RELAY_HYSTERESIS))
	{
		digitalWrite(RELAY_PIN, LOW);
	}
}