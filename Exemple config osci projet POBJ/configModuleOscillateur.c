//----------------------------------------------------------------------------//
// Nom du projet 		:	Reglage Encodeur
// Nom du fichier 		:   configModuleOscillateur.c
// Date de création 	:   02.12.2020
// Date de modification : 	03.12.2020
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
//      Hardware de la carte 
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
#include "configOscillateur.h"      // configuration de l'oscillateur interne du Dspic

//----------------------------------------------------------------------------//
//-- nom 				: InitOscillateur
//-- entrée - sortie 	: - / - 
//-- description 		: configuration oscillateur | clock à choix  
//-- remarque 			: datasheet DSPIC -> section 9 -> p.145
//                        On aimerait Fosc <= 80MHz 
//                        choix oscillateur FRC (7.37MHz)
//                        Fosc = Fin * (M / (N1 * N2))
//                        7.37*10^6 *(160 / (7 * 2)) = 73.7 [MHz]
//                        Fcy = Fosc / 2 => 73.7 x 10^6 / 2 = 36.85[MHz]
//-- modification       : 03.12.2020
//----------------------------------------------------------------------------//
void InitOscillateur(void)
{
    //-- mixe entre code Kevin & Philou --// 
    //-- Registre de contrôl globale de l'Oscillateur : 
    //-- Registre OSCCON -> p.149 --//
    //OSCCONbits.COSC = 0;                // Fast RC Oscillateur avec PLL
    //OSCCONbits.CLKLOCK	= 1;			// ne veut pas changer la clock après configuration 
    //OSCCONbits.OSWEN = 0;               // ne veut pas changer la source de la clock (interne/externe)
   
    //-- registre CLKDIV --//
    CLKDIVbits.ROI = 0;                 // interruption qui doit pas activer le mode DOZE (consommation)
	CLKDIVbits.DOZEN	= 0; 		    // ne veut pas d'un mécanisme de réduction d'alimentation 
	CLKDIVbits.PLLPRE 	= 5; 			// N1 = 5 => 5+2 = 7	-> voir remarque 
    CLKDIVbits.PLLPOST 	= 0; 			// N2 = 2 	-> voir remarque 
	
    //-- registre PLLFBD --//
    PLLFBDbits.PLLDIV 	= 158;			// M = 158 => 158+2 => 160   -> voir remarque 

    //-- registre OSCTUN --//
    OSCTUNbits.TUN = 0;  
        
    // Clock Switch to incorporate PLL
	//__builtin_write_OSCCONH(0x01);		// Initiate Clock Switch to 
										// FRC with PLL (NOSC=0b001)
    __builtin_write_OSCCONL(0x01);		// Start clock switching
    //while (OSCCONbits.COSC != 0b001);	// Wait for Clock switch to occur	

// Wait for PLL to lock
	//while(OSCCONbits.LOCK!=1) {};
    
                   // pas d'excursion de fréquence
	
    //-- resgistre ACLKON --//          // pas de besoin de configurere le resgistre pour des sources auxiliaire 
}
