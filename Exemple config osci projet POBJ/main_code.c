//----------------------------------------------------------------------------//
// Nom du projet 		:	Reglage Encodeur
// Nom du fichier 		:   main_code.c
// Date de création 	:   04.12.2019
// Date de modification : 	02.12.2020
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
//      doc pour la configuration du PWM : 
//      https://ww1.microchip.com/downloads/en/DeviceDoc/70187E.pdf
//
//      doc pour la configuration du module de quadrature 
//      https://ww1.microchip.com/downloads/en/DeviceDoc/70208C.pdf
//
//----------------------------------------------------------------------------//

//--- librairie à inclure ---// 
#if defined(__dsPIC33F__)
#include "p33Fxxxx.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>                             // appel de directive du compilateur C pour le Dspic 
#include "math.h"

#include "configuration_bitsSettings.h"     // configuration des bits settings pour l'oscillateur 
#include "configOscillateur.h"              // configuration de l'oscillateur interne du Dspic
#include "configTimer.h"                    // configuration des timers


#define NBR_PAS_MAX_ECHANTILLONER 1024

int8_t flagInterrupt = 0;

unsigned int Count;

void __attribute__((interrupt,no_auto_psv)) _T1Interrupt( void )
{
    
	
    IFS0bits.T1IF = 0;
    T1CONbits.TON = 0;
    
    
    
    if (flagInterrupt == 0) 
    {
        //LATBbits.LATB15 = 0;//~LATBbits.LATB15;
        flagInterrupt = 1; 
    }
    
    ~LATBbits.LATB15; //LATBbits.LATB15 = 0;//~LATBbits.LATB15;

          

   	
    //	
    
    /*Count++;
	if(Count == 1000)
	{
		    Count		   = 0;  
			LATBbits.LATB15 = ~LATBbits.LATB15;		    
	}*/

    TMR1          = 0;
	T1CONbits.TON = 1;
	
    /*IFS0bits.T1IF = 0;
    T1CONbits.TON = 0;
    
        TMR1          = 0;
	T1CONbits.TON = 1; */
	/* reset Timer 1 interrupt flag */
 	
}

int main(int argc, char** argv) 
{
   //-- déclaration de variable --// 
    int16_t compteurQuadra = 0; 
    
    int32_t posk = 0, posk_1 = 0, retenu = 0;
    
    
    int32_t position_angle, compteur = 0;
    
    float position; 
    
	RCONbits.SWDTEN=0;            /* Disable Watch Dog Timer*/
	
    // Configure Oscillator to operate the device at 40Mhz
    // Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
    // Fosc= 8M*40/(2*2)=80Mhz for 8M input clock
    //OSCCONbits.COSC = 0;                // Fast RC Oscillateur avec PLL
    //OSCCONbits.CLKLOCK	= 1;			// ne veut pas changer la clock après configuration 
    //OSCCONbits.OSWEN = 1;               // ne veut pas changer la source de la clock (interne/externe)
	
    /*PLLFBD=158;					// M=38
	CLKDIVbits.PLLPOST=5;		// N1=2
	CLKDIVbits.PLLPRE=0;		// N2=2*/
    
    PLLFBDbits.PLLDIV 	= 158;			// M = 158 => 158+2 => 160   -> voir remarque 
    CLKDIVbits.PLLPRE 	= 5; 			// N1 = 5 => 5+2 = 7	-> voir remarque 
    CLKDIVbits.PLLPOST 	= 0; 			// N2 = 2 	-> voir remarque 
	
    //-- registre PLLFBD --//
    

    // Clock switching to incorporate PLL
	__builtin_write_OSCCONH(0x01);		// Initiate Clock Switch to Primary
                                        // Oscillator with PLL (NOSC=0b011)
	__builtin_write_OSCCONL(0x01);		// Start clock switching
	//while (OSCCONbits.COSC != 0b011);	// Wait for Clock switch to occur	


   /*Initialize the Ports */    
    //-- initialisation de ports A et B tout en sortie 
    TRISA = 0x0000;
    //TRISB = 0x0000;
    
    //-- initilaisation de entrée en quadrature --// 
    // entrée en mode quadrature RP6 -> QA
    //RPINR14bits.QEA1R = 6;     // entrée RP6 = QA 
    // entrée en mode quadrature RP5 -> QB
    //RPINR14bits.QEA1R = 5;     // entrée RP5 = QB
    
    //QEI1CONbits.UPDN_SRC = 1;   // définit le sens de lecture 
    //QEI1CONbits.TQCS = 0;       // clock source Tcyc = 36.85MHz
    
    
    //LATA  = 0x0000;
    //PORTA = 0x0000;
 
    Timer1Init();
    /*InitOscillateur(); */
    //Timer1Init();
    
    
    //configuration pwm
    
    P1TCONbits.PTMOD = 0;   // free Running mode
    P1TCONbits.PTCKPS = 0;  // prescelaire configurer à 1 : 1 / fcyc = tcyc 
    P1TCONbits.PTOPS = 0;   // postscaler 1 : 1 
    
    PWM1CON1 = 0;           // pour un clear du regitstre 
    PWM1CON1bits.PEN1H = 1; 
    PWM1CON1bits.PEN1L = 0; 
    PWM1CON1bits.PMOD1 = 1; 
    
    
    // registre de 16bits pour la mise à jour de la péridode du PWM 
    // période : 100kHz même chose que le timer 1 => 10us (T_PWM)
    // tcyc = 1 / Fcyc = 1 / 36.85 Mhz = 27ns 
    // N-Tic_PWM = T_PWM / Tcyc = 10us / 27ns = 370 tic 
    P1TPER = 370; 
    
    // PWM à 60% (la mise à jour ce fait à 2 x la valeur de la période)
    // ex : (2 x P1TPER + 2) * rapport cyclique (60/100)
    P1DC1 = (((2 * P1TPER) + 2) * 60)/100; 
    
    // MAJ du Timer Counter lié au PWM à zéro + activation mode PWM  
    P1TMR = 0; 
    P1TCONbits.PTEN = 1;        // activer le PWM 
    
    
    //-- configuration module quadrature 
    //-- configuration des entres A et B encodeur 
    RPINR14bits.QEA1R = 6;      // correspond à la pin RP6 => A
    RPINR14bits.QEB1R = 5;      // correspond à la pin RP5 => B 
    
    
    // QEIxCON 
    QEI1CONbits.QEIM = 7;   // Quadrature mode 4 
    QEI1CONbits.PCDOUT = 1; 
    QEI1CONbits.POSRES = 0; 
    //QEI1CONbits.UPDN_SRC = 1; 
    
    DFLT1CONbits.QEOUT = 0; 
    
    
    MAX1CNT = 65535; 
    
    //-- lecture du registre compteur --// 
    //compteurQuadra = POS1CNT; 
    
    
    
    
    
    
    
    
    // DFLTxCON
    
    
    // POSxCNT 
    
    // MAXxCNT 
     
    
    
    while(1)
    {
        //-- lecture du flag d'interruption 
        if(flagInterrupt == 1)
        {
            posk = POS1CNT; 
            
            
            
            
            if (posk - posk_1 >= 32768)
            { 
                retenu -= 1;  
            }
            else if(posk - posk_1 <= -32768)
            {
                retenu += 1; 
            }
            
            position = retenu * pow(2, 16) + posk; 

            posk_1 = posk; 
            
            flagInterrupt = 0; 
            
            if(compteur >= 100000)
            {
                compteurQuadra = 1; 
            }
            
            compteur++; 
        }
        
        
        //LATBbits.LATB15 = 0;//~LATBbits.LATB15;
        //LATBbits.LATB15 = 1;//~LATBbits.LATB15;
    }

    return (EXIT_SUCCESS);
}






//----------------------------------------------------------------------------//
//-- nom 				: InitOscillateur
//-- entrée - sortie 	: - / - 
//-- description 		: configuration oscillateur | clock à choix  
//-- remarque 			: datasheet DSPIC -> section 9
//                        oscillateur a 10 MHz - FRC => sans config 7.37MHz
//-- modification       : 04.12.2019
//----------------------------------------------------------------------------//
/*void InitOscillateurX(void)
{
    
	//--- configuration de l'oscillateur interne à 32768 Hz ---//
    //-- configuration du mode FRC (Fast RC Oscollateur)
    //-- configuration registre OSCON 
    OSCCONbits.COSC = 0;                // Fast RC Oscillateur 
    OSCCONbits.CLKLOCK	= 0;			// modification de la clock 
    OSCCONbits.OSWEN = 0;               // Commutation du switch 
    
    //-- configuration registre CLKDIV 
    CLKDIVbits.ROI = 0;                 // pas d'interruption après 
    
    CLKDIVbits.ROI	= 0; 				// pas d'effet si il y a interruption 
	CLKDIVbits.DOZE	= 0; 				// pas de réduction sur l'horloge Fcy /1
	CLKDIVbits.DOZEN	= 0; 		
	CLKDIVbits.PLLPRE 	= 0; 			// N1 = 2	
    CLKDIVbits.PLLPOST 	= 0; 			// N2 = 2 	
	
	PLLFBDbits.PLLDIV 	= 38;			// M = 40  

    OSCTUNbits.TUN = 0                  // pas d'excursion de fréquence
	
    __builtin_write_OSCCONH(0x03); 		// fonction appelant du code assembleur 
										// configuration du registre NOSC (OSCCON) --> 
										// 011 = Primary Oscillator with PLL (XTPLL, HSPLL, ECPLL)
	__builtin_write_OSCCONL(0x01);		// Active la commutation de la clock
	while(OSCCONbits.COSC != 0b011); 	//
     */
/*}*/