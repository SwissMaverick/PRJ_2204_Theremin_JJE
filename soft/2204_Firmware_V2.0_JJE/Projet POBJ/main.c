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

void main(void)
{
    //declaration des variable
    // Pour une fréquence de 8 MHz
    OSCCONbits.IRCF = 0b111;
    // Utiliser l'oscillateur interne
    OSCCONbits.SCS = 1;

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

/* THE END */
void delay(unsigned long cntr)
{
    while (--cntr != 0);
}

void ADC_Init(void) {
    ADCON1 = 0x0E;    // AN0 analog, others digital
    ADCON2 = 0xA9;    // Right justify, 12 TAD, Fosc/8
    ADCON0 = 0x01;    // Enable ADC, select channel AN0
}

// Lecture ADC sur AN0
uint16_t ADC_Read_AN0(void) 
{
    ADCON0bits.GO = 1;                 // Start conversion
    while (ADCON0bits.GO);      // Wait for conversion complete
    return (uint16_t)((ADRESH << 8) + ADRESL);  // Return 10-bit result
}

// Convertit la valeur ADC en tension (en volts)
float Lire_Tension_Batterie(void) {
    uint16_t adc_value;
    float tension;
    
    adc_value = ADC_Read_AN0();
    tension = (adc_value * 5.0f) / 1023.0f;  // Si Vref = 5V
    return tension;
}

// Fonction qui surveille la tension de la batterie et allume la LED si < 1.23V
void Surveiller_Tension_Batterie(void)
{
    float tension;
    
    tension = Lire_Tension_Batterie();  

    if (tension < 1.23f)
    {
        LATEbits.LATE2 = 1;  
    }
    else
    {
        LATEbits.LATE2 = 0;
    }
}
