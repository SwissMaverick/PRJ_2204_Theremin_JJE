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
#include <xc.h>

// ==============================================================================
// BITS DE CONFIGURATION POUR PIC18F4550 (Projet Thérémine)
// ==============================================================================

// CONFIG1H : Oscillateur
#pragma config FOSC = INTOSCIO_EC // Oscillateur interne, RA6 et RA7 en E/S numériques
#pragma config FCMEN = OFF        // Moniteur d'horloge de sécurité désactivé
#pragma config IESO = OFF         // Basculement d'oscillateur désactivé

// CONFIG1L
#pragma config PLLDIV = 1       
#pragma config CPUDIV = OSC1_PLL2 
#pragma config USBDIV = 1

// CONFIG2L : Alimentation et Reset
#pragma config PWRT = ON          // Délai de démarrage activé (stabilise l'alim)
#pragma config BOR = ON           // Reset si chute de tension
#pragma config BORV = 3           // Tension du BOR fixée à 2.05V
#pragma config VREGEN = OFF       // Régulateur USB désactivé (pas d'USB utilisé)

// CONFIG2H : Chien de garde (Watchdog)
#pragma config WDT = OFF          // Watchdog désactivé pour le développement

// CONFIG3H : Multiplexage et PORTB
#pragma config CCP2MX = ON        // E/S CCP2 multiplexée avec RC1
#pragma config PBADEN = OFF       // PORTB<4:0> configurés en E/S NUMÉRIQUES au reset
#pragma config LPT1OSC = OFF      // Timer1 configuré pour haute puissance
#pragma config MCLRE = ON         // Broche MCLR activée (Reliée à SW1 sur le schéma)

// CONFIG4L : Paramètres étendus
#pragma config STVREN = ON        // Reset sur débordement de la pile
#pragma config LVP = OFF          // Programmation basse tension DÉSACTIVÉE (Libère RB5)
#pragma config ICPRT = OFF        // Port de débogage ICSP dédié désactivé
#pragma config XINST = OFF        // Jeu d'instructions étendu désactivé




#endif	/* CONFIGURATION_BITSSETTINGS_H */