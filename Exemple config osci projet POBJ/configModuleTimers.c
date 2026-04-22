//----------------------------------------------------------------------------//
// Nom du projet 		:	Reglage Encodeur
// Nom du fichier 		:   configOscillateur.h 
// Date de création 	:   03.12.2020
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
//      Hardware de la carte => ATTENTION PIC MONTER SUR LA CARTE DSPIC33FJ128MC802
//      K:\ES\PROJETS\SLO\1601x_HalfBridge3x\doc 
//
//	    doc pour le DSP : 
//		K:\ES\PROJETS\SLO\1601x_HalfBridge3x\doc\datasheets
// 
//----------------------------------------------------------------------------//
//--- librairie à inclure ---// 
#if defined(__dsPIC33F__)
#include "p33Fxxxx.h"
#endif

#include "xc.h"                     // appel de directive du compilateur C pour le Dspic 
#include "configTimer.h"


//----------------------------------------------------------------------------//
//-- nom 				: Timer1Init
//-- entrée - sortie 	: - / - 
//-- description 		: configuration Timer1 
//-- remarque 			: datasheet DSPIC -> timer section 12 -> p195
//                        configuration du timer1 à 100kHz 
//                        Fcy : 36.85 [MHz]
//                        nb_tic = Fcyc / Ft1 
//                        36.85 x 10^6 / 100 x 10^3 = 36.85
//-- modification       : 03.12.2020
//----------------------------------------------------------------------------//
void Timer1Init(void)
{
    T1CON = 0;              // Timer reset
 	IFS0bits.T1IF = 0;      // Reset Timer1 interrupt flag
	IPC0bits.T1IP = 6;      // Timer1 Interrupt priority level=4
 	IEC0bits.T1IE = 1;      // Enable Timer1 interrupt
 	TMR1=  0x0000;  	
	PR1 = 368;               // Timer1 period register = ?????
	T1CONbits.TON = 1;      // Enable Timer1 and start the counter
    
    
    
    //-- registre T1CON -> p.197 --//
	/*T1CONbits.TON = 0;          // Désactive / Stop le timer pour la configuration.
    T1CONbits.TSIDL = 0;        // Désactivé, car Stop pendant Idle Mode non-nécessaire.
    T1CONbits.TGATE = 0;        // Temps de porte désactivé.
    T1CONbits.TCKPS = 0;        // Prescaler à 1.
    T1CONbits.TCS = 0;          // Source de clock interne.
    
    //-- configuration de la valeur de départ du Timer TMR1 --// 
    TMR1 = 0;       // registre de 16bits 
    
    //-- configuration de la valeur max du registre PR1 = nb de tic : 37 --//
    PR1 = 37; 
    
    //-- appel de la fonction de configuration des interruptions liés timer1 --//
    InterruptTimer1Init(); 
    
	T1CONbits.TON = 1;  // Start le Timer1 après Init. ou non.
    
    
    /* 
        T1CON = 0;              // Timer reset
 	IFS0bits.T1IF = 0;      // Reset Timer1 interrupt flag
	IPC0bits.T1IP = 6;      // Timer1 Interrupt priority level=4
 	IEC0bits.T1IE = 1;      // Enable Timer1 interrupt
 	TMR1=  0x0000;  	
	PR1 = 0xFFFF;           // Timer1 period register = ?????
	T1CONbits.TON = 1;      // Enable Timer1 and start the counter
     */
}

//----------------------------------------------------------------------------//
//-- nom 				: InterruptTimer1Init
//-- entrée - sortie 	: - / - 
//-- description 		: configuration interruption lié au Timer1   
//-- remarque 			: datasheet DSPIC -> timer section 7 -> p89
//
//-- modification       : 03.12.2020
//----------------------------------------------------------------------------//
void InterruptTimer1Init(void)
{
    //-- configuration des 3 registres lié au timer 1 --// 
    IFS0bits.T1IF = 0;      // interruption pas activée => remise à zéro
    IPC0bits.T1IP = 1;      // priorité 1 (moins élevée)
    IEC0bits.T2IE = 1;      // activer l'interruption 
}

//----------------------------------------------------------------------------//
//-- nom 				: InterruptTimer1Init
//-- entrée - sortie 	: - / - 
//-- description 		: configuration interruption lié au Timer1   
//-- remarque 			: datasheet DSPIC -> timer section 7 -> p89
//
//-- modification       : 03.12.2020
//----------------------------------------------------------------------------//
/*void __attribute__((interrupt, no_auto_psv)) _T1Interrupt( void )
{
   
    TRISBbits.TRISB15 = 0; 
    
    LATBbits.LATB15 = (PORTBbits.RB15 ^ 0x1); 
    

    
   /* if(TRISBbits.TRISB15 == 0)
        TRISBbits.TRISB15 = 0; 
    else 
        TRISBbits.TRISB15 = 1; 
    */    
        
    //T1CONbits.TON = 1;  // Start le Timer1 après Init. ou non.
    //IFS0bits.T1IF = 0;      // interruption pas activée => remise à zéro
/*}*/
