/*
 * ============================================================================
 * File        : main.c
 * Author      : Educore
 * Target MCU  : PIC16F690
 * Compiler    : XC8
 * IDE         : MPLAB X IDE
 *
 * Description :
 * SPI transfer example
 *
 * Tested with:
 * - MPLAB X IDE
 * - XC8 Compiler
 * - PIC16F690
 * ============================================================================
 */

/*
 * ============================================================================
 * PIC16F690 - DIP20 PINOUT
 * ============================================================================
 *
 *                  +----\/----+
 *   VDD        --- |1      20| --- VSS
 *   RA5/T1CKI  --- |2      19| --- RA0/AN0
 *   RA4/AN3    --- |3      18| --- RA1/AN1
 *   RA3/MCLR   --- |4      17| --- RA2/AN2/VREF
 *   RC5        --- |5      16| --- RC0
 *   RC4        --- |6      15| --- RC1
 *   RC3        --- |7      14| --- RC2
 *   RC6/TX     --- |8      13| --- RB4
 *   RC7/RX     --- |9      12| --- RB5
 *   RB7        --- |10     11| --- RB6/ICSPCLK
 *                  +----------+
 *
 * ============================================================================
 */

// CONFIG
#pragma config FOSC = INTRCIO   // Internal oscillator, I/O function on RA4 and RA5
//#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA4/OSC2/CLKOUT and RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer disabled
#pragma config PWRTE = OFF      // Power-up Timer disabled
#pragma config MCLRE = ON       // MCLR pin function is MCLR
#pragma config CP = OFF         // Code Protection disabled
#pragma config CPD = OFF        // Data Code Protection disabled
#pragma config BOREN = ON       // Brown-out Reset enabled
#pragma config IESO = OFF       // Internal External Switchover disabled
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor disabled

#include <xc.h>
#include"SPI.h"

//Függvényprototípusok:
//void Oscillator(void);


void main(void) {
    
    Oscillator();
    
    
// Belso oszcillátor beállítása 4 MHz-re (biztonság kedvéért)
   OSCCONbits.IRCF = 0b110; 
    
    
    // SPI modul inicializálása (Master mód, Fosc/4, Mode 0)
    SPI_Init();
    
    // A kiküldend? karaktertömb (String)
    // A lezáró nullát ('\0') nem küldjük ki, csak a látható karaktereket.
    const char teszt_szoveg[] = "Educore"; 
    while(1)
    {
        // Végighaladunk a tömb elemein
        for(uint8_t i = 0; teszt_szoveg[i] != '\0'; i++)
        {
            // Kiküldjük az aktuális karaktert ASCII kódként
            // Ha össze van kötve az SDI és SDO (Loopback), a 'fogadott_adat'-ban visszakapjuk.
            //uint8_t fogadott_adat = SPI_Transfer((uint8_t)teszt_szoveg[i]);
            SPI_Transfer((uint8_t)teszt_szoveg[i]);
        }   
    }
    
}

void Oscillator(void)
{
    //OSCCONbits.IRCF=0b111;
    //Internal oszcillátor használatban, ami a config az elején megadott.
    //1 az internal, 0 a külso
    OSCCONbits.SCS=0;
    
    //Start up a külso oszcillátorról. 1 ha igen, 0 ha nem.
    OSCCONbits.OSTS=1;
}
