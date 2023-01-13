
#ifndef BODENTEENSY_MULTIPLEXER_H
#define BODENTEENSY_MULTIPLEXER_H
#define MULTIPLEXER_0 0x70
#define MULTIPLEXER_1 0x71
#define MULTIPLEXER_2 0x72
#define MULTIPLEXER_3 0x73
#define TSL_ADDR 0x49

#include "Wire.h"
#include "../Farbsensoren_alt/TSL2584TSV.h"


class Multiplexer
{
private:
    TSL2584TSV tsl;
    int Addrmultiplexer [4] ={
            MULTIPLEXER_0,
            MULTIPLEXER_1,
            MULTIPLEXER_2,
            MULTIPLEXER_3
    };
    Multiplexer();
    void tcaselect(uint8_t i, int addr);
public:
    void resetIneterrupts();
    void begin();
};
#endif //BODENTEENSY_MULTIPLEXER_H
