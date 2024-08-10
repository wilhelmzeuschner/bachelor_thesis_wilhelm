/*
This file is part of the LMM-Retrofit project.

Created on: 10. Jun 2022
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

#include "lmm_pins.h"
#include "lmm_config.h"
#include "lmm_basic_io.h"
#include "lmm_uart_config.h"
#include "lmm_dac_sig.h"
#include "lmm_digi_pot.h"

/*********************************************************************/
/**
 \brief Sets an output voltage on the SIG-Pin
		DAC101C085CIMM/NOPB
		https://www.ti.com/lit/ds/symlink/dac101c081.pdf Page 19

 \param   voltage desired output voltage of the DAC & Op-Amp in mV

 \return  -

 **********************************************************************/
void setSIGVoltage(float outputVoltage)
{
	//  Calculate code for the DAC
	float vBATT = measureVBATTmV() / 1000.0;

	uint16_t code = DAC_MAX_BITS * (outputVoltage * ((DAC_REF_R1 + DAC_REF_R2) / (vBATT * DAC_REF_R2 * DAC_OPAMP_GAIN))) * (vBATT / DAC_VBATT_NOMINAL);

	// Serial.print("vBATT: ");
	// Serial.println(vBATT);
	// Serial.print("outputVoltage: ");
	// Serial.println(outputVoltage);
	// Serial.print("code: ");
	// Serial.println(code);
	// Serial.print("code BIN:\t");
	// Serial.println(code, BIN);

	uint8_t upperByte = (code >> 6); // Upper 4 bits are always zero (& 0x0F)
	uint8_t lowerByte = (code << 2); // Lowest 2 bits are always zero (& 0xFC)

	// Serial.print("Bits:\t\t");
	// Serial.print(upperByte, BIN);Serial.print(".");
	// Serial.println(lowerByte, BIN);

	Wire.beginTransmission(DAC_ADDR1);

	Wire.write(upperByte);
	Wire.write(lowerByte);

	Wire.endTransmission();
}

/*********************************************************************/
/**
 \brief Calculates the corresponding output voltage for a given ADC measurement
		Assumes a regression with degree = 2
		Uses activeLMMSlot to select the correct / desired coefficients
		Implements a hysteresis and special off-output that matches the actual
		output voltage of the mech. sensor better that the result of the coefficients

 \param airADCVal ADC measurement
 \param	activeLMMSlot current slot of the LMM curve parameters coefficients

 \return SIGVoltage calculated voltage

 **********************************************************************/
float calcSIGVoltageFromADC(uint16_t *airADCVal, uint8_t *activeLMMSlot)
{
	float currSIGVoltage = 0;
	static float SIGVoltage = 0;
	static uint8_t dacOutputState = 1;

	// Hysteresis for the initial start of the vehicle and when the engine gets turned off
	if (*airADCVal < (DAC_THRESH_NO_OUTPUT - DAC_HYSTERESIS_NO_OUTPUT) && dacOutputState)
	{
		dacOutputState = 0;
	}
	else if (*airADCVal > (DAC_THRESH_NO_OUTPUT + DAC_HYSTERESIS_NO_OUTPUT) && !dacOutputState)
	{
		dacOutputState = 1;
	}

	// Calculate the output voltage based on which slot (for air-bypass compensation) is selected
	if (dacOutputState)
	{
		switch (*activeLMMSlot)
		{
		case 0:
			currSIGVoltage = (DAC_COEFF_X2_SLOT1 * (float)*airADCVal * (float)*airADCVal) + (DAC_COEFF_X1_SLOT1 * (float)*airADCVal) + DAC_COEFF_OFFSET_SLOT1;
			break;
		case 1:
			currSIGVoltage = (DAC_COEFF_X2_SLOT2 * (float)*airADCVal * (float)*airADCVal) + (DAC_COEFF_X1_SLOT2 * (float)*airADCVal) + DAC_COEFF_OFFSET_SLOT2;
			break;
		case 2:
			currSIGVoltage = (DAC_COEFF_X2_SLOT3 * (float)*airADCVal * (float)*airADCVal) + (DAC_COEFF_X1_SLOT3 * (float)*airADCVal) + DAC_COEFF_OFFSET_SLOT3;
			break;

		default:
			currSIGVoltage = (DAC_COEFF_X2_SLOT1 * (float)*airADCVal * (float)*airADCVal) + (DAC_COEFF_X1_SLOT1 * (float)*airADCVal) + DAC_COEFF_OFFSET_SLOT1;
			break;
		}
	}
	else
	{
		currSIGVoltage = DAC_NO_OUTPUT_LEVEL;
	}

	//Serial.print("airADCVal: ");
	//Serial.println(*airADCVal);

	// Serial.print("currSIGVoltage: ");
	// Serial.println(currSIGVoltage);

	// Simple moving average
	SIGVoltage = SIGVoltage * (1 - DAC_MOVING_AVERAGE_GAIN) + currSIGVoltage * DAC_MOVING_AVERAGE_GAIN;

	// Serial.print("SIGVoltage: ");
	// Serial.println(SIGVoltage);

	return SIGVoltage;
}