#include "FastLED.h"
FASTLED_USING_NAMESPACE


#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    3  //CHANGE BASED ON WHERE YOU PLUG IN YOUR DATA PIN(The one that connects to the DI on the led strip)
#define HEAT_PIN    4 //Pin for detecting if printer is heating -- Check my octoprint plugin on github

#define LED_TYPE    WS2811 //Type of LED Strip - Uncertain if the code will work with any other type
#define COLOR_ORDER GRB
#define NUM_LEDS    6  //CHANGE BASED ON THE NUMBER OF LEDS YOU HAVE
CRGB leds[NUM_LEDS];
#define BRIGHTNESS          96 //BRIGHTNESS, Although it doesn't seem to do anything
#define FRAMES_PER_SECOND  100 //FPS of the patterns, 100 is recommended


#define LIGHTSTATE        false //set to true for integration with other leds
#ifdef LIGHTSTATE 
#define LIGHTPIN        5
#endif

bool check = true;
int prev_message = -1;
unsigned long previousMillis =  0;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600);
}

uint8_t gHue = 0;

void loop() {
  if (LIGHTSTATE) {
    if (digitalRead(LIGHTPIN) == HIGH) {
      check = true;
    } else {
      check = false;
    }
  } else {
    check = true;
  }
  if(check) {
    while(Serial.available() > 0) {
      int message = Serial.parseInt();
      if(message == 0){
        FastLED.clear();
        prev_message = 0;
      } else if(message == 1) {
        fill_solid(leds, NUM_LEDS, CRGB::White);
        prev_message = 1;
      } else if(message == 3) {
        fill_solid(leds, NUM_LEDS, CRGB::Green);
        prev_message = 3;
      } else if(message == 2) {
        prev_message = 2;
        rainbow();
      } else if(message == 4) {
        prev_message = 4;
        fill_solid(leds, NUM_LEDS, CRGB::Orange);
      } else if((prev_message == 1 || prev_message == 3) && (digitalRead(HEAT_PIN) == HIGH)) {
        fill_solid(leds, NUM_LEDS, CRGB::Red);
        prev_message = 5;
      } else {
        switch (prev_message) {
          case 0:
            FastLED.clear();
            break;
          case 1:
            fill_solid(leds, NUM_LEDS, CRGB::White);
            break;
          case 2:
            rainbow();
            break;
          case 3:
            fill_solid(leds, NUM_LEDS, CRGB::Green);
            break;
          case 4:
            fill_solid(leds, NUM_LEDS, CRGB::Orange);
            break;
          case 5:
            fill_solid(leds, NUM_LEDS, CRGB::Red);    
            break;
        }
      }
    }
  }
  FastLED.show();  
}

void rainbow() {
  while(true) {
    if(Serial.available() > 0) {
      return;
    }
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 1000 / FRAMES_PER_SECOND) {
      fill_rainbow(leds, NUM_LEDS, gHue, 7);
      FastLED.show();
      EVERY_N_MILLISECONDS( 20 ) {
        gHue++;
      }
      previousMillis = currentMillis;
    }
  }
}

