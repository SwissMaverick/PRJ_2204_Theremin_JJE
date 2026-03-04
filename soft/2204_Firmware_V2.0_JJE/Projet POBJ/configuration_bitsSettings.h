//----------------------------------------------------------------------------//
// Nom du projet 		:	Reglage Encodeur
// Nom du fichier 		:   configuration_bitsSettings.h 
// Date de création 	:   02.12.2020
// Date de modification : 	xx.xx.20xx
// 
// Auteur 				: 	Philou (Ph. Bovey) 
//                      :   Kevin Bougnon 
//
// Description 			: 	Canevas pour le laboratoire de réglage avec la carte
//                          1601_HalfBridge 
//                         
// Remarques			: 
// 	    chemin pour trouver le headerfile 
//		C:\Program Files\Microchip\MPLAB C30\support\dsPIC33F\h
//
//      Hardware de la carte ATTENTION PIC MONTER SUR LA CARTE DSPIC33FJ128MC802
//      K:\ES\PROJETS\SLO\1601x_HalfBridge3x\doc 
//
//	    doc pour le DSP : 
//		K:\ES\PROJETS\SLO\1601x_HalfBridge3x\doc\datasheets
// 
//----------------------------------------------------------------------------//
#ifndef CONFIGURATION_BITSSETTINGS_H
#define	CONFIGURATION_BITSSETTINGS_H

//--- librairie à inclure ---// 
#include    "main.h"
// DSPIC33FJ128MC802 Configuration Bit Settings
// 'C' source line config statements

/*Definition des LEDS*/
#define LED_BAT_GRN LATEbits.LATE0
#define LED_BAT_RED LATEbits.LATE1
#define LED_PWR_State LATEbits.LATE2
#define LED_STATE LATDbits.LATD7

/*Definition des State Battery*/
#define BAT_PG PORTDbits.RD4
#define BAT_STAT2 PORTDbits.RD5
//#define BAT_STAT2 PORTDbits.RD6

/*Definition des LEDS*/

/*Definition des LEDS*/

/*Definition des LEDS*/

/*Definition des LEDS*/

/*Definition des LEDS*/

//// FBS
//#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
//#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)
//#pragma config RBS = NO_RAM             // Boot Segment RAM Protection (No Boot RAM)




#endif	/* CONFIGURATION_BITSSETTINGS_H */

