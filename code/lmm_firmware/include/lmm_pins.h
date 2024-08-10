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



#ifndef LMM_PINS__
#define LMM_PINS__

//////////////////////////////////
//Outputs:

#define LED_1_PIN				5
#define LED_2_PIN				6
#define LED_ERROR_PIN			7

#define RELAY_PIN				4
//////////////////////////////////


//////////////////////////////////
//Inputs:

#define SIG_SENSE_PIN			A0
#define TEMP_SENSE_PIN			A1
#define BATT_VSENSE_PIN			A6

#define BTN_1_PIN				2
#define BTN_2_PIN				3
//////////////////////////////////

#endif