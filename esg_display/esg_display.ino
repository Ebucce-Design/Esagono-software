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
  for(int i = 0; i < STRIPS_NUM; i++)
  {
    Serial.print("\r\nStrip ");
    Serial.print(i);
    Serial.print(" starts at node ");
    Serial.print(strips[i].start_node);
    Serial.print(" , port ");
    Serial.print(strips[i].start_port);
    Serial.print(" ends at node ");
    Serial.print(strips[i].end_node);
    Serial.print(" , port ");
    Serial.print(strips[i].end_port);
  }
  while(1){}
}









