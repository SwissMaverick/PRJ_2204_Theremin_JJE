#include "hardware.h"

// ==============================================================================
// FONCTION D'INITIALISATION DU MATERIEL
// ==============================================================================
void Init_Hardware(void) 
{
    // --- OSCILLATEUR (8 MHz) ---
    OSCCONbits.IRCF = 0b111;  
    OSCCONbits.SCS = 0b00;  
    while(!OSCCONbits.IOFS);
    
    // --- GPIO & NUMÉRIQUE ---
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

    // --- TIMERS (Timer0 pour Blinky) ---
    T0CON = 0b10100001; 
    INTCONbits.TMR0IF = 0; 
    
    // --- DAC AUDIO (SPI Logiciel) ---
    TRISDbits.TRISD5 = 0; // CS en sortie
    TRISBbits.TRISB1 = 0; // SCK en sortie
    TRISBbits.TRISB0 = 0; // SDI en sortie (on force l'entrée matérielle à devenir une sortie !)
    DAC_CS = 1;  // CS désactivé (le DAC est inactif à l'état haut)
    DAC_SCK = 0; // Horloge au repos
    DAC_SDI = 0; // Ligne de données à 0
    
    // Timer 0 (Pour les délais ou futurs clignotements)
    T0CON = 0b10000011; // Prescaler 1:16
    INTCONbits.TMR0IF = 0; 

    // Timer 1 (Le chronomètre pour le capteur Ultrason)
    // 16-bit, Horloge interne, Prescaler 1:4, Timer ON
    T1CON = 0b10100001; 

    // Module de Capture CCP1 (Relié à RC2 / ECHO_H)
    // Mode Capture : se déclenche sur chaque FRONT MONTANT au repos
    CCP1CON = 0b00000101; 

    // Configuration des Interruptions
    PIR1bits.CCP1IF = 0;  // Remise à zéro du drapeau de capture
    PIE1bits.CCP1IE = 1;  // Activation de l'interruption pour CCP1
    
    // Activation des interruptions générales du PIC
    INTCONbits.PEIE = 1;  // Autorise les interruptions périphériques
    INTCONbits.GIE = 1;   // Autorise les interruptions globales
}

// ==============================================================================
// FONCTION D'ECRITURE DANS LE DAC
// ==============================================================================
void DAC_Write(uint8_t valeur) 
{
    // Le MCP4901 attend 16 bits : 
    // 4 bits de configuration (0x3 = 0011 -> Unbuffered, Gain 1x, Active)
    // 8 bits de données (ta valeur audio de 0 à 255)
    // 4 bits de remplissage à la fin (zéros)
    uint16_t trame = 0x3000 | ((uint16_t)valeur << 4); 
    
    DAC_CS = 0; // État bas : On réveille le DAC et on commence la transmission
    
    // On envoie les 16 bits, du plus fort (15) au plus faible (0)
    for (int8_t i = 15; i >= 0; i--) 
    {
        // On place le bit de donnée sur la broche SDI
        if ((trame >> i) & 0x0001)
        {
            DAC_SDI = 1;
        } 
        else
        {
            DAC_SDI = 0;
        }
        
        // On donne un coup d'horloge (le DAC lit la donnée quand SCK passe à 1)
        DAC_SCK = 1;
        DAC_SCK = 0;
    }
    
    DAC_CS = 1; // État haut : Fin de transmission, le DAC met à jour sa tension de sortie
}