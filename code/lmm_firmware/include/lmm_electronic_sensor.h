/*
This file is part of the LMM-Retrofit project.

Created on: 18. Jun 2022
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

#ifndef LMM_ELECTRONIC_SENSOR__
#define LMM_ELECTRONIC_SENSOR__

#include <Arduino.h>

// Coefficients were acquired from the trendline generated in Excel
//  * 1000 in the end, since the calculation will later be in milli °C
#define EL_SENSOR_TEMPERATURE_COEFF_X -0.13 * 1000
#define EL_SENSOR_TEMPERATURE_COEFF_OFFSET 93.241 * 1000

uint16_t readElectrSensTemperatureMilliDegreeCelsius();
void readElectrSensAir(uint16_t *airADCVal);

#endif