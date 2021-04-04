#include "SPI.h"
#include "RF24.h"

#include <Button.h>


// Radio, choose any pins on your micro-controller to use as the CE and CSN pins.
#define CE_PIN 3
#define CSN_PIN 7

const uint8_t ADDRESS[6] = "CKeyB";


const int MAX_SHORTCUT_KEYS = 4;  // Maximum number of keys a shortcut can be, or max number of keys to send through the NRF24L01 per button.


const int BTN_SHORTCUT_SIZE = (3 * MAX_SHORTCUT_KEYS) + (MAX_SHORTCUT_KEYS - 1) + 1;

struct ButtonInfo {
    int btnPin;
    char btnShortcut[BTN_SHORTCUT_SIZE];
};


// All shortcuts are represented using their ASCII decimal/int value. Seperate each key by a space.
// This can be found here: www.asciitable.com
const ButtonInfo BUTTONS_INFO[] = {{2, "80 119 82"},
                                   {1, "128 99"},
                                   {0, "128 118"},
                                   };


const int N_BUTTONS = sizeof(BUTTONS_INFO) / sizeof(BUTTONS_INFO[0]);

const uint16_t DEBOUNCE_MS = 10;


RF24 radio(CE_PIN, CSN_PIN);


Button *buttonObjs[N_BUTTONS];



void initRadio() {
    
    if (!radio.begin()) {
        while (1) {}
    }

    radio.setPALevel(RF24_PA_LOW);
    radio.setPayloadSize(BTN_SHORTCUT_SIZE);
    radio.openWritingPipe(ADDRESS);
    radio.setDataRate(RF24_1MBPS);
    
    radio.stopListening();
}

void initButtons() {
    for(int i = 0; i < N_BUTTONS; i++) {
        ButtonInfo btnInfo = BUTTONS_INFO[i];
        buttonObjs[i] = new Button(btnInfo.btnPin, DEBOUNCE_MS);
        buttonObjs[i] -> begin();
    }
}


void setup() {

    initRadio();
    initButtons();
    
}

void loop() {

    // Loop through all buttons and detect if any pressed, then send the button keys through radio using the nRF24L01.
    for(int i = 0; i < N_BUTTONS; i++) {
        if(buttonObjs[i] -> pressed()) {
            ButtonInfo btnInfo = BUTTONS_INFO[i];
            
            radio.write(&btnInfo.btnShortcut, sizeof(btnInfo.btnShortcut));
            
            break;
        }
    }
}
