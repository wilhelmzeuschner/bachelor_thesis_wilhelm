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
#include "lmm_basic_io.h"
#include "lmm_uart_config.h"
#include "lmm_dac_sig.h"
#include "lmm_digi_pot.h"

/*********************************************************************/
/**
 \brief Sets the digital potentiometer's resistance to the supplied value (nearest possible one)

 \param   resistance

 \return  -

 **********************************************************************/
void writeDigiPot(uint16_t resistance)
{
	//Calculate equivalent code for the specified resistance
	uint8_t potData = 255 - ((float)resistance / (float)DIGI_POT_MAX_R) * 255;

	Wire.beginTransmission(DIGI_POT_ADDR);
	Wire.write(DIGI_POT_RES_REG);
	Wire.write(potData);
	Wire.endTransmission();
}