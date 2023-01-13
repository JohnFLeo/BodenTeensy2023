
#include "Multiplexer.h"
Multiplexer::Multiplexer(){
    tsl = TSL2584TSV(TSL_ADDR);
}
void Multiplexer::resetIneterrupts(){
    for(int i = 0; i < 4; i++){//durchlaufe alle Multiplexer
        for(int j = 0; j < 8; j++){//durchlaufe die Sensoren
            tsl.resetInterrupt();
        }
    }
}
void Multiplexer::begin(){
    for(int i = 0; i < 4; i++){//durchlaufe alle Multiplexer
        for(int j = 0; j < 8; j++){//durchlaufe die Sensoren
            tsl.begin();
        }
    }
}
void Multiplexer::tcaselect(uint8_t i, int addr) {
    if (i > 7) return;
    Wire.beginTransmission(addr);
    Wire.write(1 << i);
    Wire.endTransmission();
}



