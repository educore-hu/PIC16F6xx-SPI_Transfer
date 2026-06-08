/*
 * File:        SPI.c
 * Author:      Educore
 * Target:      PIC16F690
 * Compiler:    XC8
 * Description: SPI (Serial Peripheral Interface) driver master módban.
 *              Tartalmazza az automatikus Enable (CS) láb kezelést 
 *              a logikai analizátorok pontos csomagdekódolásához.
 * 
 * Created on 2026
 * Revision history: 1.2 (Added Auto-CS for Logic Analyzers)
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
#include <xc.h>
#include "SPI.h"

/* ===== LOGIKAI ANALIZÁTOR CS / ENABLE LÁB DEFINÍCIÓ ===== */
// Az RA5-ös lábat (2-es fizikai láb) használjuk Chip Select (CS / SS) jelként.
// Az SPI perifériák (és az analizátor) aktív alacsony jelszintet várnak (Active Low).
#define SPI_CS_TRIS   TRISCbits.TRISC6
#define SPI_CS_PORT   PORTCbits.RC6

#define SPI_CS_LOW()  (SPI_CS_PORT = 0) // Kiválasztás / Csoport indítása
#define SPI_CS_HIGH() (SPI_CS_PORT = 1) // Elengedés / Csoport lezárása

/**
 * @brief Inicializálja az MSSP modult SPI Master módba és beállítja a CS lábat.
 */
void SPI_Init(void)
{
    // 1. Teljes leállítás és konfigurációs regiszterek lenullázása
    SSPCON = 0x00;   // SSPEN = 0, és az összes SSPM bit kényszerített NULLA!
    SSPSTAT = 0x00;

    // 2. Analóg funkciók letiltása (Hogy az RB6 és RB4 digitális legyen)
    ANSELHbits.ANS10 = 0; // RB4 digitális bemenet (MISO)
    ANSELHbits.ANS11 = 0; // RB6 digitális kimenet (SCK)

    // 3. I/O irányok beállítása
    TRISBbits.TRISB6 = 0; // SCK (RB6, 11-es láb) -> Kimenet (Órajel generátor)
    TRISCbits.TRISC7 = 0; // SDO (RC7, 9-es láb)  -> Kimenet (MOSI)
    TRISBbits.TRISB4 = 1; // SDI (RB4, 13-as láb) -> Bemenet (MISO)

    // ===== ENABLE / CS LÁB INICIALIZÁLÁSA =====
    SPI_CS_TRIS = 0;      // RA5 (2-es láb) -> Kimenet (Enable / CS)
    SPI_CS_HIGH();        // Alaphelyzetben Magas szint (Inaktív)

    // 4. SPI Id?zítések (Mode 0)
    SSPSTATbits.SMP = 0;  // Mintavétel az adat-id?tartam közepén
    SSPSTATbits.CKE = 1;  // Aktívból passzívba váltáskor ad le jelet

    // 5. Órajel és Mód beállítása (Master mód, Fosc/4)
    //Bit 5 (SSPEN) = 1 -> Engedélyezés
    
    // Bit 3-0 (SSPM) = 0000 -> SPI Master mód, órajel = Fosc/4  
    SSPCONbits.SSPM = 0b0000; // Master mode, Fosc/4
    //Engedélyezése
    SSPCONbits.SSPEN = 1;
    // Vagy egyben megadva: SSPCON = 0x20; 
}

/**
 * @brief Egy bájt kiküldése hardveres Enable (CS) lábvezérléssel.
 * @param data: A kiküldend? 8-bites adat (pl. ASCII karakter).
 * @return uint8_t: A fogadott 8-bites adat.
 */
uint8_t SPI_Transfer(uint8_t data)
{
    // ===== SPI flags =====
    SSPCONbits.SSPOV = 0;
    SSPCONbits.WCOL  = 0;

    // 1. CS láb lehúzása Alacsony szintre (0V) -> Jelzi az analizátornak az átvitel kezdetét
    SPI_CS_LOW();

    // 2. Adat beírása a pufferbe -> Ez elindítja a hardveres órajelet és adatküldést
    SSPBUF = data;        

    // 3. Várakozás, amíg mind a 8 bit átvitele befejez?dik (BF flag 1 lesz)
    while(!SSPSTATbits.BF);  

    // 4. CS láb visszaállítása Magas szintre (5V/3.3V) -> Csomag lezárása
    SPI_CS_HIGH();

    // 5. A fogadott bájt visszaadása (Kiolvasása kötelez?, ez törli a BF flag-et)
    return SSPBUF;                  
}
