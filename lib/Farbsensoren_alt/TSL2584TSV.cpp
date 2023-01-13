#if defined ( ESP8266 )
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#include <util/delay.h>
#endif
#include <stdlib.h>
#include "TSL2584TSV.h"
#include <Wire.h>
TSL2584TSV::TSL2584TSV() {
    _addr = 0x29;
    _initialized = false;
    _integration = TSL2584TSV_INTEGRATIONTIME_2_7MS;
    _gain = TSL2584TSV_GAIN_8X;
}
//region setter
void TSL2584TSV::setThreshold(uint16_t value) {
    uint8_t lowerByte = (uint8_t)value; // Get lower byte of 16-bit var
    uint8_t upperByte = (uint8_t)(value >> 8); // Get upper byte of 16-bit var

    resetInterrupt();

    write8(_addr, TSL2584TSV_COMMAND_BIT | TSL2584TSV_REGISTER_THRESHHOLDL_LOW, 0);
    write8(_addr, TSL2584TSV_COMMAND_BIT | TSL2584TSV_REGISTER_THRESHHOLDL_HIGH, 0);

    write8(_addr, TSL2584TSV_COMMAND_BIT | TSL2584TSV_REGISTER_THRESHHOLDH_LOW, lowerByte);
    write8(_addr, TSL2584TSV_COMMAND_BIT | TSL2584TSV_REGISTER_THRESHHOLDH_HIGH, upperByte);

    write8(_addr, TSL2584TSV_COMMAND_BIT | TSL2584TSV_REGISTER_INTERRUPT, TSL2584TSV_SET_INTERRUPT);
}
boolean TSL2584TSV::begin() {
    _initialized = true;
    delay(2);
    disable();
    delay(2);
    setGain(_gain);
    delay(2);
    setTiming(_integration);
    delay(2);
    enable();
    delay(2);
    return true;
}
void TSL2584TSV::enable() {
    //if (!_initialized) begin();
    write8(_addr, TSL2584TSV_COMMAND_BIT | TSL2584TSV_REGISTER_CONTROL, TSL2584TSV_CONTROL_POWERON_ADCENABLE);
}
void TSL2584TSV::disable() {
    if (!_initialized) begin();
    write8(_addr, TSL2584TSV_COMMAND_BIT | TSL2584TSV_REGISTER_CONTROL, TSL2584TSV_CONTROL_POWERON_ADCDISABLE);
}
void TSL2584TSV::disableInterrupt() {
    write8(_addr, TSL2584TSV_COMMAND_BIT | TSL2584TSV_REGISTER_INTERRUPT, TSL2584TSV_DISABLE_INTERRUPT);
}
void TSL2584TSV::setGain(TSL2584TSVGain_t gain) {
    _gain = gain;
    write8(_addr, TSL2584TSV_COMMAND_BIT | TSL2584TSV_REGISTER_ANALOG, _gain);
}
void TSL2584TSV::setTiming(TSL2584TSVIntegrationTime_t integration) {
    _integration = integration;
    write8(_addr, TSL2584TSV_COMMAND_BIT | TSL2584TSV_REGISTER_TIMING, _integration);
}

void TSL2584TSV::resetInterrupt() {
    Wire.beginTransmission(_addr);
    Wire.write(TSL2584TSV_CLEAR_INTERRUPT);
    Wire.endTransmission();
}
//endregion
//region getter
uint16_t TSL2584TSV::getLuminosity (uint8_t channel) {
    uint16_t z = 0;
    if (channel == 0) {
        z = read16(_addr, TSL2584TSV_COMMAND_BIT | TSL2584TSV_REGISTER_DATA0LOW);
    }
    else if (channel == 1) {
        z = read16(_addr, TSL2584TSV_COMMAND_BIT | TSL2584TSV_REGISTER_DATA1LOW);
    }
    return z;
}
byte TSL2584TSV::getControllRegister() {
    return read8(_addr, TSL2584TSV_COMMAND_BIT | TSL2584TSV_REGISTER_CONTROL);
}
int TSL2584TSV::getThreshold() {
    return read16(_addr, TSL2584TSV_COMMAND_BIT | TSL2584TSV_REGISTER_THRESHHOLDH_LOW);
}
//endregion
//region BasicReadWrite
uint16_t TSL2584TSV::read8(uint8_t adress, uint8_t reg) {
    uint16_t value = 0;
    Wire.beginTransmission(adress);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(adress, (uint8_t)1);
    value = Wire.read();
    return value;
}
uint16_t TSL2584TSV::read16(uint8_t adress, uint8_t reg)
{
    uint16_t value;
    Wire.beginTransmission(adress);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(adress, (uint8_t)2);
    value = Wire.read();
    value += 256 * Wire.read();
    return value;
}
void TSL2584TSV::write8 (uint8_t adress, uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(adress);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}
//endregion