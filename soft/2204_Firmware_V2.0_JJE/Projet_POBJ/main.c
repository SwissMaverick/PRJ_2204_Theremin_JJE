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

#define LED_STATE LATDbits.LATD7
#define LED_BAT_RED  LATEbits.LATE1 
#define LED_BAT_GRN  LATEbits.LATE0 
#define BAT_READ     LATAbits.LATA0

void main(void)
{
    Init_Hardware();

    // Variable pour ajuster la vitesse de clignotement
    int compteur = 0;
    
    while(1) 
    {
        // On vérifie si le Timer0 a débordé (Flag à 1)
        if (INTCONbits.TMR0IF == 1)
        {
            // Étape très importante : on remet le drapeau à 0 manuellement
            INTCONbits.TMR0IF = 0; 
            
            compteur++;
            
            // Avec Fosc=8MHz et Prescaler=256, le Timer déborde environ toutes
            // les 8.3 secondes. 
            // Pour ce test, on inverse la LED à chaque débordement.
            // (Si c'est trop lent, on ajustera le Prescaler dans configModuleTimers.c !)
            if(compteur >= 1) 
            { 
                LED_STATE = ~LED_STATE; // Inverse l'état de la LED
                compteur = 0;
            }
        }
    }  
}