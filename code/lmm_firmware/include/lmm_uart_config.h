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

#ifndef LMM_UART_CONFIG__
#define LMM_UART_CONFIG__

#define UART_ENABLE true

#if UART_ENABLE
#define UART_SPEED 115200

#define UART_PROJECT_NAME			"LMM-Retrofit"
#define UART_FIRMWARE_VERSION 		"1.0"
#define UART_FIRMWARE_RELEASE_DATE 	"29. Jul. 2022"
#define UART_FIRMWARE_AUTHOR 		"Hans Wilhelm Zeuschner"
#define UART_WEBSITE_URL			"www.wizeus.de/lmm-ba"
#define UART_COMPILER_VERSION 		__VERSION__

#endif
#endif