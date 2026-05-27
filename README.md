Caractéristiques du PIC18 :  
Fréquence : 48MHz  
Sources d'interruption : 20  
I/O Ports : A, B, C, D, E  
Timers : 4  
PWM modules : 1  
Comm série : USART  
USB modules : 1  
Comparateurs : 2  
Boîtiers : 40-Pin PDIP, 44-Pin QFN, 44-Pin TQFP  


# README du projet 2204 Thérémine version 3
## Objectif du projet

## Versionnning
Version de l'IDE : v6.15  
Version du compilateur : XC8 (v2.46)  
PIC utilisé : PIC18F4550-I/PT  

## Mise en place du projet

### Déploiement

## Explication sur les fonctions implémentées :
### Initialisation  
```C
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
```


### Communication -> trames

### Calculs

### Gestion des datas

## Validation des fonctions implémentées

### Testing :
