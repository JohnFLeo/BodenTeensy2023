#include "Arduino.h"
#include "Bodensensoren.h"
Bodensensoren::Bodensensoren() {
    boden = TSL2584TSV();
}
void Bodensensoren::init() {
    TSL2584TSV current;
    for(int i = 0; i < 32; i++){
        current = getBoden(i);
        current.begin();
    }
    setThresholds();
}
void Bodensensoren::tcaselect(uint8_t i, int addr) {
    if (i > 7) return;
    Wire.beginTransmission(addr);
    Wire.write(1 << i);
    Wire.endTransmission();
}
TSL2584TSV Bodensensoren::getBoden(int i){//19(Chanel 3 Multiplexer [2] 0x72) und 26(Chanel 2 Multiplexer [3] 0x73) funktionieren nicht
    //for deciding which Multiplexer should be used
    tcareset();
    tcaselect(i%8, multiplexerAddr[i/8]);
    return boden;
}
void Bodensensoren::tcareset() {
    for(int i = 0;i<4;i++){
        Wire.beginTransmission(multiplexerAddr[i]);
        Wire.write(0);
        Wire.endTransmission();
    }
}
void Bodensensoren::setThresholds() {
    TSL2584TSV current;
    for (int i = 0; i < 32; i++) {
        current = getBoden(i);
        current.setThreshold(calibValues[i]);
    }
}
int Bodensensoren::resetInterrupt() {
    int ret = 0;
    for (int i = 0; i < 32; i++) {
        TSL2584TSV current = getBoden(i);
        current.resetInterrupt();
        //if (current.getLuminosity(0) > calibValues[i]){
        //    ret = 1;
        //}
    }
    return ret;
}