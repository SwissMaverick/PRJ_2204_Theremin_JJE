#include "hardware.h"

void Init_Hardware(void) 
{
    // --- 1. OSCILLATEUR (8 MHz) ---
    OSCCONbits.IRCF = 0b111;  
    OSCCONbits.SCS = 0b00;  
    while(!OSCCONbits.IOFS);
    
    // --- 2. GPIO & NUMÉRIQUE ---
    ADCON1 = 0x0F; // Tout en numérique par défaut       
    CMCON = 0x07;             
    
    // Config de la LED de test et des broches de batterie
    TRISEbits.TRISE2 = 0; // RE2 configuré en SORTIE
    LED_DS1 = 0;          // On s'assure qu'elle est éteinte au départ        
    
    TRISEbits.TRISE1 = 0; // LED_BAT_RED en sortie
    TRISEbits.TRISE0 = 0; // LED_BAT_GRN en sortie 
    TRISAbits.TRISA0 = 0; // BAT_READ en sortie
    TRISAbits.TRISA1 = 1; // BAT_LVL (AN1) en entrée
    
    // Config du capteur Ultrason Horizontal
    TRISCbits.TRISC0 = 0; // TRIG_H en SORTIE (On envoie le signal)
    TRISCbits.TRISC2 = 1; // ECHO_H en ENTRÉE (On écoute le capteur)
    TRIG_H = 0;           // On s'assure que le Trigger est à 0 par défaut

    // --- 3. TIMERS (Timer0 pour Blinky) ---
    T0CON = 0b10100001; 
    INTCONbits.TMR0IF = 0; 
    
    // Configuration des Interruptions
    PIR1bits.CCP1IF = 0;  // Remise à zéro du drapeau de capture
    PIE1bits.CCP1IE = 1;  // Activation de l'interruption pour CCP1
    
    // Activation des interruptions générales du PIC
    INTCONbits.PEIE = 1;  // Autorise les interruptions périphériques
    INTCONbits.GIE = 1;   // Autorise les interruptions globales
}

/*
void ADC_Init(void) 
{
    // On reconfigure AN0 et AN1 en analogique, le reste en numérique
    ADCON1 = 0x0D;    
    ADCON2 = 0xA9;    // Justification à droite, 12 TAD, Fosc/8
    ADCON0 = 0x05;    // Active l'ADC et sélectionne le canal AN1 (BAT_LVL)
}

uint16_t ADC_Read_AN1(void) 
{
    ADCON0bits.GO = 1;          // Démarre la conversion
    while (ADCON0bits.GO);      // Attend la fin de la conversion
    return (uint16_t)((ADRESH << 8) + ADRESL); // Retourne le résultat sur 10 bits
}

float Lire_Tension_Batterie(void) 
{
    uint16_t adc_value;
    float tension;
    
    BAT_READ = 1;   // Active la lecture (ferme le pont diviseur)
    __delay_ms(1);  // On utilise la vraie fonction de délai de xc.h (1ms suffit largement ici)
    
    adc_value = ADC_Read_AN1();
    
    BAT_READ = 0;   // On coupe la lecture pour économiser la batterie
    
    tension = (adc_value * 3.3f) / 1023.0f;  
    
    return tension;
}

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
*/