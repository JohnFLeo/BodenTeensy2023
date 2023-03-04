#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <../lib/Farbsensoren_alt/Bodensensoren.h>

#define LED 13
//d
Adafruit_NeoPixel strip(32,LED);
Bodensensoren boden;

bool Calibrieren;
int firstInterrupt = 32;
u_int32_t timestamp;
TSL2584TSV tsl2584Tsv;
uint16_t maxValues[32] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
uint16_t minValues[32] = {
        10000,10000,10000,10000,10000,10000,10000,10000,
        10000,10000,10000,10000,10000,10000,10000,10000,
        10000,10000,10000,10000,10000,10000,10000,10000,
        10000,10000,10000,10000,10000,10000,10000,10000
};

void ThresholdHerausfinden();
void testInterrupts();
void testValues();
void nurTest();
void isInterrupt();
/*
void setup(){//RGB programm
    pinMode(LED, OUTPUT);
    strip.begin();
    strip.setBrightness(150);
    strip.fill(Adafruit_NeoPixel().Color(255,255,255));
    strip.show();
}
void loop(){
    strip.fill(Adafruit_NeoPixel().Color(255,255,0));
    strip.show();
    delay(1000);
    strip.fill(Adafruit_NeoPixel().Color(255,0,255));
    strip.show();
    delay(1000);
    strip.fill(Adafruit_NeoPixel().Color(0,255,255));
    strip.show();
    delay(1000);
}
*/
void setup() {
    //region InitLED
    pinMode(LED, OUTPUT);
    strip.begin();
    strip.setBrightness(150);
    strip.fill(Adafruit_NeoPixel().Color(255,255,255));
    strip.show();
    //endregion
    //region InitComunication
    Serial.begin(9600);
    Serial1.begin(9600);
    while (!Serial1){}
    //endregion
    Wire.begin();
    Wire.setClock(400000);
    Calibrieren = false;
    //firstInterrupt = -1;
    boden = Bodensensoren();
    boden.init();
    boden.resetInterrupt();
    //tsl2584Tsv = TSL2584TSV();
    //tsl2584Tsv.begin();
}
void loop() {
    isInterrupt();
    boden.resetInterrupt();
    int length = Serial1.available();
    for(int i=0; i<length;i++) {
        int msg = Serial1.read();
        switch (msg) {
            case 255:
                //boden.resetInterrupt();
                Serial.println("Reset Interrupt");
                break;
            case 128:
                Calibrieren = !Calibrieren;
                break;
            default:
                break;
        }
    }//hinten =2;vorne = 4;links = 3;rechts=1
    if(Calibrieren){
        testInterrupts();
        //testValues();
        //ThresholdHerausfinden();
        //nurTest();
    }
}
//region tests
void ThresholdHerausfinden(){
    for (int i = 0; i < 32; i++) {
        maxValues[i] = max(maxValues[i], boden.getBoden(i).getLuminosity(0));
        minValues[i] = min(minValues[i], boden.getBoden(i).getLuminosity(0));
        boden.calibValues[i] = minValues[i] + ((maxValues[i] - minValues[i]) / 2);
        Serial.print(boden.calibValues[i]);Serial.println(", ");
    }
    boden.setThresholds();
    Serial.println();
}
void nurTest(){
    boden.tcareset();
    //boden.tcaselect(3,0x72);
    boden.tcaselect(1,0x73);
    Serial.println(tsl2584Tsv.getLuminosity(0));
}
void testValues(){
    for(int i = 0; i < 32; i++) {
        if(!(i==3||i==8||i==11||i==13||i==14||i==16||i==18||i==23||i==25||i==28)){
            Serial.print(boden.getBoden(i).getLuminosity(0));
            Serial.print(", ");
        }
    }Serial.println();
}
void testInterrupts(){
    for(int i = 0; i < 32; i++) {
        if(!(i==11||i==18||i==19||i==26||i==28||i==8||i==2||i==9||i==10||i==21||i==23||i==24||i==25||i==27||i==30||i==31)) {
        //if(!(i==3||i==8||i==11||i==13||i==14||i==16||i==18||i==23||i==25||i==28)){//für teensy mit "1"3,8,11,13,14,16,18,23,25
            Serial.print(i);
            Serial.print(": ");
            Serial.print(boden.getBoden(i).getControllRegister() & 0b00100011, BIN);
            Serial.print(", ");
        }
    }Serial.println();
}
//endregion
void isInterrupt(){
    Serial1.write(boden.iToDirection[firstInterrupt]);
    Serial.println(boden.iToDirection[firstInterrupt]);
    //wenn 200ms nach firstInterrupt vergangen reset firstInterrupt und timestamp
    if((millis() - timestamp) > 300 && firstInterrupt != 32){
        firstInterrupt = 32;
        timestamp = 0;
    }
    for (int i = 0; i < 32; ++i) {
        if(!(i==11||i==18||i==19||i==26||i==28||i==8||i==2||i==9||i==10||i==21||i==23||i==24||i==25||i==27||i==30||i==31)){
        //if(!(i==3||i==8||i==11||i==13||i==14||i==16||i==18||i==23||i==25||i==28)){//für Bodenplatine mit "1"
            if((boden.getBoden(i).getControllRegister() & 0b00100011) == 0b00100011){//Interrupt an Sensor i
                //Kontrolle ob erster Interrupt
                if(firstInterrupt == 32){//32 bedeutet kein erster Interrupt gespeichert
                    firstInterrupt = i;//Sensor i eintragen als firstInterrupt
                    timestamp = millis();//erzeuge timestamp0
                }
            }
        }
    }
}
