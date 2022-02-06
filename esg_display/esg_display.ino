#include <FastLED.h>
#include <stdio.h>
#include "esagono.h"

#define NUM_LEDS    (STRIPS_NUM * LEDS_PER_STRIP)
#define DATA_PIN    A0
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

CRGB leds[NUM_LEDS];

void setup()
{
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(115200);
  Serial.write("\r\nEsagono misc stuff sketch\r\n");
}

void loop()
{ 
  for(u8 i = 0; i < LEDS_PER_STRIP; i++)
  {
      Serial.println(ESG_GET_LED_INDEX(1, i));
  }
  while(1){}
}










