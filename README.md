# README du projet 2204 Thérémine version 3
## Objectif du projet
Le Théremin est un appareil permettant de générer un signal audible, dont l'amplitude et la fréquence sont contrôlées par l'utilisateur au moyen de deux capteurs de distance.  L'appareil peut être utilisé sur secteur ou de manière nomade grâce à une batterie interne.  

Grâce la gestuelle des mains par rapport aux capteurs, la fréquence et l'amplitude du signal audio peuvent être modifiées.  

## Versionning
Version de l'IDE : v6.15  
Version du compilateur : XC8 (v2.46)  
PIC utilisé : PIC18F4550-I/PT  
### Caractéristique du PIC18
Fréquence : 48MHz  
Sources d'interruption : 20  
I/O Ports : A, B, C, D, E  
Timers : 4  
PWM modules : 1  
Comm série : USART  
USB modules : 1  
Comparateurs : 2  
Boîtiers : 40-Pin PDIP, 44-Pin QFN, 44-Pin TQFP  

## Mise en place du projet
### Déploiement
### Fréquence du PIC18
```C
#define _XTAL_FREQ 8000000
```
### Bits de configuration
```C
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
```
### Définition des broches
```C
// Définition des broches
#define LED_DS1      LATEbits.LATE2
#define LED_BAT_RED  LATEbits.LATE1 
#define LED_BAT_GRN  LATEbits.LATE0 
#define BAT_READ     LATAbits.LATA0

// CAPTEUR ULTRASON HORIZONTAL
#define TRIG_H LATCbits.LATC0  // Sortie (On envoie l'impulsion)
#define ECHO_H PORTCbits.RC2   // Entrée (On lit le retour)

// DAC AUDIO (MCP4901)
#define DAC_CS   LATDbits.LATD5 // Chip Select (Active le DAC)
#define DAC_SCK  LATBbits.LATB1 // Horloge SPI
#define DAC_SDI  LATBbits.LATB0 // Données SPI (Sortie logicielle)
```
### Prototypes des fonctions
```C
void Init_Hardware(void);
void DAC_Write(uint8_t valeur);
void ADC_Init(void);
uint16_t ADC_Read_AN1(void);
float Lire_Tension_Batterie(void);
void Surveiller_Tension_Batterie(void);
```

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
### Fonction principale
```C
void main(void) 
{
    Init_Hardware();

    uint16_t duree_ticks = 0;
    float distance_cm = 0;
    uint8_t amplitude = 0;

    while(1) 
    {
        mesure_prete = 0;  // On remet notre signal à zéro
        envoyer_trigger(); // On lance un ping
        
        // Le HC-SR04 a besoin de 60ms de repos entre deux tirs pour que
        // l'écho ne rebondisse pas partout dans la pièce
        __delay_ms(60); 

        // Une fois les 60ms passées, l'interruption matérielle a largement eu le 
        // temps de faire son travail en tâche de fond. On peut calculer :
        if (mesure_prete == 1) 
        {
            // Calcul de la différence de temps (gestion du redémarrage du timer)
            if (temps_fin >= temps_depart) 
            {
                duree_ticks = temps_fin - temps_depart;
            } 
            else 
            {
                duree_ticks = (65535 - temps_depart) + temps_fin; 
            }

            // Explication de la formule mathématique :
            // Le Timer1 avance toutes les 2µs (grâce au Prescaler 1:4).
            // Distance standard = Temps(µs) / 58
            // Ici : Distance = (ticks * 2) / 58 => Distance = ticks / 29
            distance_cm = (float)duree_ticks / 29.0f;

            // --- LE TEST ---
            // Si la main est entre 2 cm (minimum du capteur) et 15 cm
            if (distance_cm >= 2 && distance_cm <= 15)
            {
                LED_DS1 = 1; // Allumé : L'obstacle est dans la zone !
            }
            else 
            {
                LED_DS1 = 0; // Éteint : L'obstacle est trop loin
            }
        }
        else
        {
            // MODE ERREUR : L'interruption n'a pas détecté la fin de l'écho !
            // On fait clignoter la LED très brièvement pour signaler un problème
            LED_DS1 = 1;
            __delay_ms(10);
            LED_DS1 = 0;
        }
    }
}
```
### Fonction d'envoi d'impulsion pour le capteur ultrason
```C
void envoyer_trigger(void) 
{
    TRIG_H = 1;
    __delay_us(10); // Impulsion de 10 microsecondes requise par le HC-SR04
    TRIG_H = 0;
}
```
### Fonction d'interruption
```C
void __interrupt() ISR(void) 
{
    // Si l'interruption vient du module de Capture CCP1 (Capteur Ultrason)
    if (PIR1bits.CCP1IF) 
    {
        PIR1bits.CCP1IF = 0; // Remise à zéro du vrai drapeau

        // Si on surveillait le FRONT MONTANT (L'écho vient de s'activer)
        if (CCP1CONbits.CCP1M0 == 1) 
        {
            temps_depart = ((uint16_t)CCPR1H << 8) | CCPR1L; 
            
            PIE1bits.CCP1IE = 0;  // 1. On désactive l'interruption
            CCP1CON = 0b00000100; // 2. On change de direction (Front Descendant)
            PIR1bits.CCP1IF = 0;  // 3. On efface le "faux" drapeau généré
            PIE1bits.CCP1IE = 1;  // 4. On réactive l'interruption
        } 
        // Sinon, c'est le FRONT DESCENDANT (L'écho vient de se couper)
        else 
        {
            temps_fin = ((uint16_t)CCPR1H << 8) | CCPR1L;    
            mesure_prete = 1;
            
            PIE1bits.CCP1IE = 0;  
            CCP1CON = 0b00000101; 
            PIR1bits.CCP1IF = 0;  
            PIE1bits.CCP1IE = 1;  
        }
    }
}
```

### Communication -> trames

### Calculs

### Gestion des datas

## Validation des fonctions implémentées

### Testing :
