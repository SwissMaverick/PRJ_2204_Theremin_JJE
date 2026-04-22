//----------------------------------------------------------------------------//
// Nom du projet 		:	Reglage Encodeur
// Nom du fichier 		:   configOscillateur.h 
// Date de création 	:   03.12.2020
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
//      Hardware de la carte => ATTENTION PIC MONTER SUR LA CARTE DSPIC33FJ128MC802
//      K:\ES\PROJETS\SLO\1601x_HalfBridge3x\doc 
//
//	    doc pour le DSP : 
//		K:\ES\PROJETS\SLO\1601x_HalfBridge3x\doc\datasheets
// 
//----------------------------------------------------------------------------//

#ifndef CONFIGTIMER_H
#define	CONFIGTIMER_H

void Timer1Init(void); 
void InterruptTimer1Init(void); 

//void __attribute__((interrupt, no_auto_psv)) _T1Interrupt( void ); 

#endif	/* CONFIGTIMER_H */

