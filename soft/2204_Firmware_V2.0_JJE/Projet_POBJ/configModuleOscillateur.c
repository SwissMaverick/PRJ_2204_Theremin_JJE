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

// ==============================================================================
// BITS DE CONFIGURATION POUR PIC18F4550 (Thérémine)
// ==============================================================================

// CONFIG1H : Oscillateur
#pragma config FOSC = INTOSCIO_EC // Oscillateur interne, broches RA6 et RA7 configurées en I/O numériques
#pragma config FCMEN = OFF        // Moniteur d'horloge de sécurité désactivé
#pragma config IESO = OFF         // Basculement d'oscillateur désactivé

// CONFIG2L : Alimentation et Reset
#pragma config PWRT = ON          // Délai de démarrage activé (stabilise l'alim au boot)
#pragma config BOR = ON           // Reset si chute de tension (Brown-out Reset)
#pragma config BORV = 3           // Tension du BOR fixée à 2.05V minimum
#pragma config VREGEN = OFF       // Régulateur USB désactivé (pas d'USB utilisé sur le schéma)

// CONFIG2H : Chien de garde (Watchdog)
#pragma config WDT = OFF          // Watchdog désactivé (indispensable pour le débogage)
#pragma config WDTPS = 32768      // Diviseur du Watchdog (ignoré car WDT est OFF)

// CONFIG3H : Multiplexage et PORTB
#pragma config CCP2MX = ON        // L'entrée/sortie CCP2 est multiplexée avec RC1
#pragma config PBADEN = OFF       // PORTB<4:0> configurés en E/S NUMÉRIQUES au démarrage
#pragma config LPT1OSC = OFF      // Timer1 configuré pour une horloge haute puissance
#pragma config MCLRE = ON         // Broche MCLR activée (reliée au bouton SW1 sur ton schéma)

// CONFIG4L : Paramètres étendus
#pragma config STVREN = ON        // Reset sur débordement de la pile
#pragma config LVP = OFF          // Programmation basse tension DÉSACTIVÉE (Libère la broche RB5)
#pragma config ICPRT = OFF        // Port de débogage ICSP dédié désactivé
#pragma config XINST = OFF        // Jeu d'instructions étendu désactivé (code standard)

// CONFIG5L à CONFIG7H : Protections du code (Désactivées pour le développement)
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF
#pragma config CPB = OFF, CPD = OFF
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF
#pragma config WRTC = OFF, WRTB = OFF, WRTD = OFF
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF
#pragma config EBTRB = OFF