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

#ifndef LMM_DIGI_POT__
#define LMM_DIGI_POT__

#include <Arduino.h>

//I²C address
#define DIGI_POT_ADDR		0x2C
#define DIGI_POT_RES_REG	0x00

#define DIGI_POT_MAX_R		53500

void writeDigiPot(uint16_t resistance);


#endif