/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */
//#include <xc.h> // include processor files - each processor file is guarded. 
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "configuration_bitsSettings.h"
#include "hardware.h"

#define LED_DS1 LATEbits.LATE2
#define LED_BAT_RED  LATEbits.LATE1 
#define LED_BAT_GRN  LATEbits.LATE0 
#define BAT_READ     LATAbits.LATA0

// Variables globales (volatile = modifiées en tâche de fond par l'interruption)
volatile uint16_t temps_depart = 0;
volatile uint16_t temps_fin = 0;
volatile uint8_t  mesure_prete = 0;

void __interrupt() ISR(void) 
{
    // Si l'interruption vient du module de Capture CCP1 (Capteur Ultrason)
    if (PIR1bits.CCP1IF) 
    {
        PIR1bits.CCP1IF = 0; // Remise à zéro du drapeau

        // Si on surveillait le FRONT MONTANT (L'écho vient de s'activer)
        if (CCP1CONbits.CCP1M0 == 1) 
        {
            temps_depart = (CCPR1H << 8) | CCPR1L; // On prend en photo le Timer1
            CCP1CON = 0b00000100;                  // On inverse : on surveille maintenant le FRONT DESCENDANT
        } 
        // Sinon, c'est le FRONT DESCENDANT (L'écho vient de se couper)
        else 
        {
            temps_fin = (CCPR1H << 8) | CCPR1L;    // On prend la 2ème photo du Timer1
            mesure_prete = 1;                      // On signale au main.c que le calcul est possible
            CCP1CON = 0b00000101;                  // On se remet sur FRONT MONTANT pour la prochaine fois
        }
    }
}

// ==============================================================================
// FONCTION DE DÉCLENCHEMENT DU CAPTEUR
// ==============================================================================
void envoyer_trigger(void) 
{
    TRIG_H = 1;
    __delay_us(10); // Impulsion de 10 microsecondes requise par le HC-SR04
    TRIG_H = 0;
}

// ==============================================================================
// PROGRAMME PRINCIPAL
// ==============================================================================
void main(void) 
{
    Init_Hardware();

    uint16_t duree_ticks = 0;
    float distance_cm = 0;

    while(1) 
    {
        envoyer_trigger(); // On lance un ping
        mesure_prete = 0;  // On remet notre signal à zéro
        
        // Le HC-SR04 a besoin de 60ms de repos entre deux tirs pour que
        // l'écho ne rebondisse pas partout dans la pièce
        __delay_ms(60); 

        // Une fois les 60ms passées, l'interruption matérielle a largement eu le 
        // temps de faire son travail en tâche de fond. On peut calculer :
        if (mesure_prete == 1) 
        {
            // Calcul de la différence de temps (gestion du redémarrage du timer)
            if (temps_fin >= temps_depart) {
                duree_ticks = temps_fin - temps_depart;
            } else {
                duree_ticks = (65535 - temps_depart) + temps_fin; 
            }

            // Explication de la formule mathématique :
            // Le Timer1 avance toutes les 2µs (grâce au Prescaler 1:4).
            // Distance standard = Temps(µs) / 58
            // Ici : Distance = (ticks * 2) / 58 => Distance = ticks / 29
            distance_cm = (float)duree_ticks / 29.0f;

            // --- LE TEST ---
            // Si la main est entre 2 cm (minimum du capteur) et 15 cm
            if (distance_cm > 2.0f && distance_cm < 15.0f) {
                LED_DS1 = 1; // Allume la LED
            } else {
                LED_DS1 = 0; // Éteint la LED
            }
        }
    }
}