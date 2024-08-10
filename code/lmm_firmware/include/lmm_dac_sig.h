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

#ifndef LMM_DAC_SIG__
#define LMM_DAC_SIG__

#include <Arduino.h>

// DAC101C085CIMM/NOPB
// https://www.ti.com/lit/ds/symlink/dac101c081.pdf

#define DAC_ADDR1 0x09
#define DAC_ADDR2 0x44			// Don't know the purpose of this
#define DAC_ADDR_BROADCAST 0x48 // Broadcast address

#define DAC_MAX_BITS 1023 // 0x3FF

// These are soldered onto the PCB and generate the reference voltage for the DAC
#define DAC_REF_R1 200 + 182
#define DAC_REF_R2 182

// Coefficients for converting an ADC reading of the new electronic sensor into an output voltage equivalent to the old mechanical one
// Take these from the Jupyter Notebook's ouput
// The different "slots" can be selected via the pushbuttons
//Slot No.	| Info
//1			| STANDARD		(from Jupyter)
//2			| Lower			(constant offset)
//3			| Higher		(constant offset)
#define DAC_SLOTS_COUNT 	3

#define DAC_COEFF_X2_SLOT1 -6.92143381546649e-06
#define DAC_COEFF_X1_SLOT1 0.013847126171459853
#define DAC_COEFF_OFFSET_SLOT1 0.5006561119637878

#define DAC_COEFF_X2_SLOT2 -6.92143381546649e-06
#define DAC_COEFF_X1_SLOT2 0.013847126171459853
#define DAC_COEFF_OFFSET_SLOT2 (DAC_COEFF_OFFSET_SLOT1 - 0.5)

#define DAC_COEFF_X2_SLOT3 -6.92143381546649e-06
#define DAC_COEFF_X1_SLOT3 0.013847126171459853
#define DAC_COEFF_OFFSET_SLOT3 (DAC_COEFF_OFFSET_SLOT1 + 0.5)


#define	DAC_VBATT_NOMINAL 12.0


//In the real world the motor always uses a certain amount of air
//The engine can't and won't run below this threshold
//DAC_THRESH_NO_OUTPUT sets this point  
#define DAC_THRESH_NO_OUTPUT 		330		//(ADC Value)
#define DAC_HYSTERESIS_NO_OUTPUT 	60		//(ADC Value)
//Voltage that the DAC outputs if the input is below the threshold
#define DAC_NO_OUTPUT_LEVEL		1.8		//(Volt)

#define DAC_MOVING_AVERAGE_GAIN	0.5

#define DAC_OPAMP_GAIN 2

void setSIGVoltage(float outputVoltage);
float calcSIGVoltageFromADC(uint16_t *airADCVal, uint8_t * activeLMMSlot);

#endif