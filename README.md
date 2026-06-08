# PIC16F690 SPI Master példa

Egyszerű SPI (Serial Peripheral Interface) példa PIC16F690 mikrokontrollerhez XC8 fordítóval.

A projekt bemutatja:

* Az MSSP modul SPI Master módban történő használatát
* Karakterek kiküldését SPI buszon
* Automatikus Chip Select (CS) kezelését
* Logikai analizátorral történő egyszerű vizsgálhatóságot

## Hardver

### Mikrokontroller

* PIC16F690
* Belső oszcillátor 4 MHz

### SPI jelek

| Funkció     | PIC16F690 láb |
| ----------- | ------------- |
| SCK         | RB6           |
| MOSI (SDO)  | RC7           |
| MISO (SDI)  | RB4           |
| CS / Enable | RC6           |

## Bekötés logikai analizátorhoz

A projekt elsődlegesen SPI kommunikáció vizsgálatára készült.

Ajánlott csatornák:

| Analizátor csatorna | Jel  |
| ------------------- | ---- |
| CH0                 | SCK  |
| CH1                 | MOSI |
| CH2                 | MISO |
| CH3                 | CS   |

A CS jel minden egyes SPI átvitel előtt alacsony szintre vált, majd az átvitel végén visszaáll magas szintre. Ez segíti a Saleae Logic és más analizátorok SPI dekódolását.

## Működés

A program folyamatosan elküldi az alábbi szöveget SPI buszon:

```text
Educore
```

Minden karakter külön SPI tranzakcióként kerül kiküldésre.

A küldött ASCII karakterek:

```text
E d u c o r e
```

Hexadecimális formában:

```text
45 64 75 63 6F 72 65
```

## SPI konfiguráció

* Master mód
* SPI Mode 0
* Órajel: Fosc/4
* Mintavétel: adatablak közepe
* CKE = 1

## Fordítás

Fejlesztőkörnyezet:

* MPLAB X IDE
* XC8 Compiler

A projektet MPLAB X IDE alatt lehet megnyitni és fordítani.

## Megjegyzések

* A CS láb használata opcionális SPI eszközök esetén, de logikai analizátor használatakor erősen ajánlott.
* A példaprogram nem használ megszakításokat.
* A kód oktatási és demonstrációs célokra készült.

## Szerző

Educore

2026
