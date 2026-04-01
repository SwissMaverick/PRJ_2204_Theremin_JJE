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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <p18cxxx.h>
#include <pic18.h>
#include  "main.h"
#include "configuration_bitsSettings.h"

#define LED_BAT_RED  LATAbits.LATA1
#define LED_BAT_RED  LATEbits.LATE1
#define LED_BAT_GRN  LATEbits.LATE0
#define BAT_READ     LATAbits.LATA0

void main(void)
{
    //declaration des variable

    TRISEbits.TRISE2 = 1;  // Output
    TRISEbits.TRISE2 = 0;  // Configure RE2 en sortie
    LATEbits.LATE2 = 0;    // Éteint la LED au démarrage
    ADC_Init();            // Initialise l'ADC
    
 while(1)
 {

     Surveiller_Tension_Batterie();

     delay(10000); //pour éviter de surcharger la mesure
     
  
//          LED_PWR_State = 0;
//          for ( int x=0; x<=10; x++ )
//            {
//              delay (1000);
//              //__delay_ms(20);   // 20x10 200ms
//
//            }
//           LED_PWR_State = 1;
//
//          for ( int x=0; x<=10; x++ )
//
//            {
//              delay (1000);
//             // __delay_ms(20);
//
//            }
     
  }
}

void init_microcontroleur(void) 
{
    // --- CRITÈRES 1 & 2 : Oscillateur interne à la vitesse MAX (32 MHz) ---
    OSCCONbits.IRCF = 0b111;  // Sélectionne la source interne à 8 MHz
    OSCCONbits.SCS = 0b00;    // Utilise le bloc d'oscillateur primaire (avec PLL)

    // --- CRITÈRE 3 : Toutes les pinoches du port A en numérique ---
    ADCON1 = 0x0F;            // 0x0F = Toutes les broches ANx sont numériques pures
    CMCON = 0x07;             // Désactive les comparateurs analogiques
}

void ADC_Init(void) 
{
    ADCON1 = 0x0D;    // AN0 analog, others digital
    ADCON2 = 0xA9;    // Right justify, 12 TAD, Fosc/8
    ADCON0 = 0x05;    // Enable ADC, select channel AN0
}

// Lecture ADC sur AN0
uint16_t ADC_Read_AN0(void) 
{
    ADCON0bits.GO = 1;          // Démarre la conversion
    while (ADCON0bits.GO);      // Attend la fin de la conversion
    return (uint16_t)((ADRESH << 8) + ADRESL); // Retourne le résultat sur 10 bits
}

// Convertit la valeur ADC en tension (en volts)
float Lire_Tension_Batterie(void) 
{
    uint16_t adc_value;
    float tension;
    
    // Le schéma montre un transistor Q1 (BSS138) piloté par BAT_READ[cite: 175, 177].
    // Il faut activer BAT_READ (mettre à 1) pour fermer le pont diviseur et pouvoir lire !
    BAT_READ = 1;
    delay(100); // Laisse le temps à la tension de se stabiliser
    
    adc_value = ADC_Read_AN1();
    
    BAT_READ = 0; // On coupe la lecture pour économiser la batterie
    
    // CORRECTION DU 5V : Ton PIC est alimenté en 3.3V ! 
    // La tension de référence maximale est donc de 3.3V.
    tension = (adc_value * 3.3f) / 1023.0f;  
    
    // Attention : Comme il y a un pont diviseur physique sur ta carte (R16/R17)[cite: 185], 
    // la valeur lue ici n'est qu'une fraction de la vraie VBAT. 
    // Tu devras multiplier 'tension' par le ratio de ton pont diviseur plus tard.
    
    return tension;
}

// Fonction qui surveille la tension de la batterie et allume la LED si < 1.23V
void Surveiller_Tension_Batterie(void)
{
    float tension = Lire_Tension_Batterie();  

    // Allume la LED rouge (RE1) si tension critique, sinon l'éteint 
    if (tension < 1.23f)
    {
        LED_BAT_RED = 1;  
    }
    else
    {
        LED_BAT_RED = 0;
    }
}

/* THE END */
void delay(unsigned long cntr)
{
    while (--cntr != 0);
}