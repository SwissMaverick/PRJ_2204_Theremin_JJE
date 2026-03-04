//----------------------------------------------------------------------------//
// Nom du projet 		:	Reglage Encodeur
// Nom du fichier 		:   configModuleOscillateur.c
// Date de création 	:   02.12.2020
// Date de modification : 	27.01.2021
// 
// Auteur 				: 	Philou (Ph. Bovey) 
//                      :   Michel Bonzon 
//                      :   Kevin Bougnon 
//
// Description 			: 	Canevas pour le laboratoire de réglage avec la carte
//                          1601_HalfBridge 
//                         
// Remarques			: 
// 	    chemin pour trouver le headerfile 
//		C:\Program Files\Microchip\MPLAB C30\support\dsPIC33F\h
//
//      Hardware de la carte 
//      K:\ES\PROJETS\SLO\1601x_HalfBridge3x\doc 
//
//	    doc pour le DSP : 
//		K:\ES\PROJETS\SLO\1601x_HalfBridge3x\doc\datasheets
// 
//----------------------------------------------------------------------------//

#include <xc.h>
#include "configOscillateur.h" // Include your header if it exists

//----------------------------------------------------------------------------//
// CONFIGURATION BITS (Merged and Fixed)
//----------------------------------------------------------------------------//

// CONFIG
#pragma config PLLDIV = 5       // Divise 20MHz crystal by 5 to get 4MHz for PLL input
#pragma config CPUDIV = OSC1_PLL2 // CPU Clock: 96MHz PLL / 2 = 48MHz
#pragma config USBDIV = 2       // USB Clock: 96MHz PLL / 2 = 48MHz

// CONFIG1H
#pragma config FOSC = HSPLL_HS  // HS oscillator, PLL enabled (High Speed Crystal)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF       // Oscillator Switchover mode disabled

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer disabled
#pragma config BOR = ON         // Brown-out Reset enabled
#pragma config BORV = 3         // Brown-out Reset Voltage = 2.05V
#pragma config VREGEN = ON      // USB Voltage Regulator enabled (Required for USB)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer DISABLED (Turn ON if needed, but usually OFF for debug)
#pragma config WDTPS = 32768    // Watchdog Postscaler

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 input/output is multiplexed with RC1
#pragma config PBADEN = OFF     // PORTB<4:0> pins are configured as DIGITAL I/O on Reset
#pragma config LPT1OSC = OFF    // Timer1 configured for higher power operation
#pragma config MCLRE = ON       // MCLR pin enabled

// CONFIG4L
#pragma config STVREN = ON      // Stack full/underflow will cause Reset
#pragma config LVP = OFF        // Single-Supply ICSP DISABLED (Important: keeps RB5 usable)
#pragma config ICPRT = OFF      // ICPORT disabled
#pragma config XINST = OFF      // Extended Instruction Set disabled

// CONFIG5L - CONFIG7H (Code Protection)
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF
#pragma config CPB = OFF, CPD = OFF
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF
#pragma config WRTC = OFF, WRTB = OFF, WRTD = OFF
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF
#pragma config EBTRB = OFF