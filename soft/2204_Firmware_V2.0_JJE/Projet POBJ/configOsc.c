/*
 * File:   configOscillateur.c
 * Author: 
 *
 */
//--- librairie à inclure ---// 
#if defined(__dsPIC18F__)
#include "p33F4550.h"
#endif


#include"configOsc.h"
#include "xc.h"                     // appel de directive du compilateur C pour le Dspic 

/*
void InitOscillateur(void)
{
    OSCCONbits.IRCF = 0b111;  // 8 MHz
    OSCCONbits.SCS = 0b10;    // Sélectionne l'oscillateur interne (INTOSC)
}*/