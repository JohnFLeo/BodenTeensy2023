#ifndef BODENSENSOREN_H
#define BODENSENSOREN_H

#include "Arduino.h"
#include "TSL2584TSV.h"
#include <Wire.h>
class Bodensensoren
{
private:
    TSL2584TSV boden;
    int multiplexerAddr[4] ={//0x71-0x77
            0x70,
            0x71,
            0x72,
            0x73
    };

public:
    void tcaselect(uint8_t i, int addr);
    void tcareset();
    Bodensensoren();
    void init();
    TSL2584TSV getBoden(int i);
    void setThresholds();
    int resetInterrupt();
    int iToDirection[33] = {
            8,//0
            1,//1
            1,//2
            1,//3
            2,//4
            2,//5
            2,//6
            2,//7
            3,//8
            3,//9
            3,//10
            3,//11
            4,//12
            4,//13
            4,//14
            4,//15
            5,//16
            5,//17
            5,//18
            5,//19
            6,//20
            6,//21
            6,//22
            6,//23
            7,//24
            7,//25
            7,//26
            7,//27
            8,//28
            8,//29
            8,//30
            8,//31
            0,//wenn kein sensor einen Interruptmeldet
    };
    uint16_t calibValues[32] = {
            800,//0
            800,//1
            800,//2
            800,//3
            800,//4
            800,//5
            800,//6
            800,//7
            800,//8
            800,//9
            800,//10
            800,//11
            800,//12
            800,//13
            800,//14
            800,//15
            800,//16
            800,//17
            800,//18
            800,//19
            800,//20
            800,//21
            800,//22
            800,//23
            800,//24
            800,//25
            800,//26
            800,//27
            800,//28
            800,//29
            800,//30
            800,//31
    };
};

#endif