#include <FastLED.h>
#include <stdio.h>
#include "esagono.h"

#define NUM_LEDS    (STRIPS_NUM * LEDS_PER_STRIP)
#define DATA_PIN    A0
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

CRGB leds[NUM_LEDS];


void print_node(int n);
void print_strip(int s);
void print_triangle(esg_triangle_t * t);

void triangle_fill_solid(esg_triangle_t * t, const struct CRGB& color)
{
  fill_solid(leds + t->side_id[0] * LEDS_PER_STRIP, LEDS_PER_STRIP, color);
  fill_solid(leds + t->side_id[1] * LEDS_PER_STRIP, LEDS_PER_STRIP, color);
  fill_solid(leds + t->side_id[2] * LEDS_PER_STRIP, LEDS_PER_STRIP, color);
}

void setup()
{
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  fill_solid(leds,NUM_LEDS,CRGB::Black);
  FastLED.show();

  Serial.begin(115200);
  Serial.write("\r\nEsagono misc stuff sketch\r\n");
  esg_explore();
}

void loop()
{ 
  esg_triangle_t t;
  for(int node = 0; node < ARRAY_SIZE(nodes_map);node++)
  {
    for(u8 port = 0; port < 6; port++)
    {
      if(esg_get_triangle(node,port, &t) >= 0)
      { 
        Serial.print(node);
        Serial.print("/");
        Serial.print(port);
        print_triangle(&t);
        triangle_fill_solid(&t, CRGB::Magenta);
        FastLED.show();
        delay(300);
        triangle_fill_solid(&t, CRGB::Black);
      }
    }
  }
}

void print_node(int n)
{
  if((n > ARRAY_SIZE(nodes_map))||(n < 0)) return;
  Serial.print("\r\nNode ");
  Serial.print(n);
  for(u8 p = 0; p < 6; p++)
  {
    Serial.print("\r\nPort ");
    Serial.print(p);
    if(nodes_map[n].ports[p].strip_id >=0)
    {
      Serial.print(" connected to strip's ");
      Serial.print(nodes_map[n].ports[p].strip_id);
      if(nodes_map[n].ports[p].strip_dir == START)
      {
        Serial.print(" START, ");
      }
      else
      {
        Serial.print(" END, ");
      }
      print_strip(nodes_map[n].ports[p].strip_id);
    }
    else
    {
      Serial.print(" not connected anywhere");
    }
  }
}

void print_strip(int s)
{
  if((s > STRIPS_NUM)||(s < 0))return;
  Serial.print("\r\nStrip ");
  Serial.print(s);
  Serial.print(" starts at node ");
  Serial.print(strips_map[s].start_node_id);
  Serial.print(" port ");
  Serial.print(strips_map[s].start_port);
  Serial.print(", ends at node ");
  Serial.print(strips_map[s].end_node_id);
  Serial.print(" port ");
  Serial.print(strips_map[s].end_port);
}

void print_triangle(esg_triangle_t * t)
{
  Serial.print("\r\nTriangle data:");
  print_strip(t->side_id[0]);
  print_strip(t->side_id[1]);
  print_strip(t->side_id[2]);
}