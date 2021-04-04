#include "SPI.h"
#include "RF24.h"

#include <Keyboard.h>

#define CE_PIN 5
#define CSN_PIN 6

RF24 radio(CE_PIN, CSN_PIN);


uint8_t ADDRESS[6] = "CKeyB";


const int MAX_SHORTCUT_KEYS = 4;  // Maximum number of keys a shortcut can be, or max number of keys to send through the NRF24L01.


const int BTN_SHORTCUT_SIZE = (3 * MAX_SHORTCUT_KEYS) + (MAX_SHORTCUT_KEYS - 1) + 1;

char btnShortcut[BTN_SHORTCUT_SIZE];

char * key;
int keyInt;



void initRadio() {
    if (!radio.begin()) {
        while (1) {}
    }
    
    radio.setPALevel(RF24_PA_LOW);
    radio.setPayloadSize(BTN_SHORTCUT_SIZE);
    radio.openReadingPipe(1, ADDRESS);
    radio.setDataRate(RF24_1MBPS);
    
    radio.startListening();
}



void setup() {
    Serial.begin(115200);
    Serial.println("Started");

    Keyboard.begin();
    
    initRadio();
}



void loop() {
    if (radio.available()) {
        radio.read(&btnShortcut, sizeof(btnShortcut));
        
        // Excract each key integer value from the incoming string and press each button.
        key = strtok(btnShortcut, " ");
        
        while(key != NULL) {
            keyInt = atoi(key);    
            Keyboard.press(keyInt);
            
            key = strtok(NULL, " ");
        }
        
        delay(10);
        Keyboard.releaseAll();
    }
}
