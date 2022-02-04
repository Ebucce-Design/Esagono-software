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
CRGB colors[] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::White};


int esagono_get_led_index(u8 strip, u8 pos);
void strip_turn_off(u8 strip_num, u16 delay_ms);
void strip_turn_on(u8 strip_num, CRGB color, u16 delay_ms);
void esagono_node_blow(esagono_node_t * node, CRGB color, u16 delay_ms);
void esagono_explore_nodes();
u8 strip_id_to_node_id(u8 strip, u8 * start_node, u8 * end_node);
void setup()
{
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(115200);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  Serial.write("\r\nEsagono misc stuff sketch\r\n");
}


void esagono_node_blow(esagono_node_t * node, CRGB color, u16 delay_ms)
{
  for (u8 dist = 0; dist < LEDS_PER_STRIP; dist++)
  {
    for (u8 port = 0; port < 6; port++)
    {
      if (node->dir[port])
      {
        leds[distance_from_node_to_led_index(node, port, dist)] = color;
      }
    }
     FastLED.show();
     delay(delay_ms);
  }
}


void loop()
{
u8 start_node = 0;
u8 end_node = 0;

for(u8 strip = 0; strip < STRIPS_NUM; strip++)
{
  if(strip_id_to_node_id(strip, &start_node, &end_node))
  {
    Serial.print("\r\nSomething wrong with strip ");
    Serial.print(strip);
  }
  else
  {
    Serial.print("\r\nStrip ");
    Serial.print(strip);
    Serial.print(" starts at node ");
    Serial.print(start_node);
    Serial.print(" ,ends at node ");
    Serial.print(end_node);
  }
}
esagono_explore_nodes();

for(u8 n = 0; n < ARRAY_SIZE(nodes); n++)
{
  Serial.print("\r\nNode ");
  Serial.print(n);
  Serial.print(" leads to nodes: ");
  for(u8 port = 0; port < 6; port++)
  {
    if(nodes[n].dir[port])
    {
     Serial.print(nodes[n].next_node_id[port]);
     Serial.print(" ");
    }
  }
}
while(1){}

}

inline int esagono_get_led_index(u8 strip, u8 pos)
{
  if (pos > 4) pos = 14 - pos;
  return (strip * 10 + pos);
}

int distance_from_node_to_led_index(esagono_node_t * node, u8 port, u8 distance)
{
  if (node->dir[port] == 0) return -1;
  if (node->dir[port] == 1) return esagono_get_led_index(node->id[port], distance);
  if (node->dir[port] == 2) return esagono_get_led_index(node->id[port], 9 - distance);
}


void strip_turn_on(u8 strip_num, CRGB color, u16 delay_ms)
{
  for (u8 i = 0; i < LEDS_PER_STRIP; i++)
  {
    leds[esagono_get_led_index(strip_num, i)] = color;
    FastLED.show();
    delay(delay_ms);
  }
}


void strip_turn_off(u8 strip_num, u16 delay_ms)
{
  for (int i = LEDS_PER_STRIP - 1; i >= 0; i--)
  {
    leds[esagono_get_led_index(strip_num, i)] = CRGB::Black;
    FastLED.show();
    delay(delay_ms);
  }
}

u8 strip_id_to_node_id(u8 strip, u8 * start_node, u8 * end_node)
{
  u8 flag = 0;
  for(u8 n = 0; n < ARRAY_SIZE(nodes); n++)
  {
    for(u8 port = 0; port < 6; port++)
    {
      {
        if(nodes[n].id[port] == strip)
        {
          if(nodes[n].dir[port] == 1) 
          {
            *start_node = n;
            flag++;
          }
          if(nodes[n].dir[port] == 2) 
          {
            *end_node = n;
            flag++;  
          }
          if(flag == 2) return 0;
        }
      }
    }
  }
  return 1;
}


void esagono_explore_nodes()
{
  u8 tmp;
  for(u8 n = 0; n < ARRAY_SIZE(nodes); n++)
  {
    for(u8 port = 0; port < 6; port++)
    {
      if (nodes[n].dir[port] == 1) // if port has the beginning of the strip we should look for its end
      {  
        strip_id_to_node_id(nodes[n].id[port], &tmp, &nodes[n].next_node_id[port]);
      }
      if (nodes[n].dir[port] == 2) // if port has the beginning of the strip we should look for its end
      {  
        strip_id_to_node_id(nodes[n].id[port], &nodes[n].next_node_id[port], &tmp);
      }
    }
  }
}
