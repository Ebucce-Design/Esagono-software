#include <FastLED.h>
#include <stdio.h>
#include "esagono_mapping.h"
#define STRIPS_NUM  29
#define LEDS_PER_STRIP  10

#define NUM_LEDS    (STRIPS_NUM * LEDS_PER_STRIP)
#define DATA_PIN    A0
#define DELAY_MS    50
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

CRGB leds[NUM_LEDS];
CRGB colors[] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::White, CRGB::Black };


int esagono_get_led_index(u8 strip, u8 pos);
void strip_turn_off(u8 strip_num, u16 delay_ms);
void strip_turn_on(u8 strip_num, CRGB color, u16 delay_ms);
void esagono_node_blow(esagono_node_t * node, CRGB color, u16 delay_ms);
void setup() 
{ 
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  
  //FastLED.setBrightness(16);
  Serial.begin(115200); 
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
  }
  FastLED.show();

  Serial.write("\r\nEsagono test and setup sketch.");
  Serial.write("\r\nPress + to light up the next LED strip, press - to turn it off.");



  for(u8 i = 0; i < ARRAY_SIZE(nodes);i++)
  {
    esagono_node_blow(&nodes[i], CRGB::Green, 0);
    esagono_node_blow(&nodes[i], CRGB::Black, 0);
  }
  Serial.write("\r\nNodes testing also available.");
}



int index = -1;
u8 c = 0;

void loop() 
{
   if (Serial.available() > 0) 
  {
      c = Serial.read();
      switch(c)
      {
        case '+':
        {
          if(index < STRIPS_NUM - 1)
          {  
            index++;
            Serial.print("\r\nTurning ON strip ");
            Serial.print(index);
            strip_turn_on(index, CRGB::Magenta, 20);
          }
          break;
        }
        case '-':
        {
          if(index > -1)
          {  
            Serial.print("\r\nTurning OFF strip ");
            Serial.print(index);
            strip_turn_off(index, 20);
            index--;
          }
          break;
        }
      }
  }
}

int esagono_get_led_index(u8 strip, u8 pos)
{
  if (pos > 4) pos = 9 - (pos - 5);
  return (strip * 10 + pos); 
}

int distance_from_node_to_led_index(esagono_node_t * node, u8 port, u8 distance)
{
  if(node->dir[port] == 0) return -1;
  if(node->dir[port] == 1) return esagono_get_led_index(node->id[port], distance);
  if(node->dir[port] == 2) return esagono_get_led_index(node->id[port], 9 - distance);
}

void strip_turn_on(u8 strip_num, CRGB color, u16 delay_ms)
{
  for(u8 i = 0; i < LEDS_PER_STRIP; i++)
  {
    leds[esagono_get_led_index(strip_num, i)] = color;
    FastLED.show();
    delay(delay_ms);
  }
}


void strip_turn_off(u8 strip_num, u16 delay_ms)
{
  for(int i = LEDS_PER_STRIP - 1; i >= 0; i--)
  {
    leds[esagono_get_led_index(strip_num, i)] = CRGB::Black;
    FastLED.show();
    delay(delay_ms);
  }
}

void esagono_node_blow(esagono_node_t * node, CRGB color, u16 delay_ms)
{
  for(u8 dist = 0; dist < LEDS_PER_STRIP; dist++)
  {
 //  Serial.print("\r\nDistance from the node: ");
 //   Serial.print(dist);
      
    for(u8 port = 0; port < 6; port++)
    {
    if(node->dir[port])
    {
 //    int index = distance_from_node_to_led_index(node, port, dist);
 //    Serial.print("\r\nFound strip on port ");
 //    Serial.print(port);
 //    Serial.print("\r\nLed index is ");
 //    Serial.print(index);
      leds[distance_from_node_to_led_index(node, port, dist)] = color;
    }
    FastLED.show();
    delay(delay_ms);
    }
  }
}
