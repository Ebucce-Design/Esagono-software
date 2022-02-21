#include <FastLED.h>
#include <stdio.h>
#include "esagono.h"
#include "Print.h"

#define NUM_LEDS    (STRIPS_NUM * LEDS_PER_STRIP)
#define DATA_PIN    A0

CRGB leds[NUM_LEDS];

FILE mystdout;

static int my_putchar(char c, FILE *stream) 
{
  Serial.write(c);
  return 0;
}

void triangle_fill_solid(esg_triangle_t * t, const struct CRGB& color);

void setup()
{
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(10);
  fill_solid(leds,NUM_LEDS,CRGB::Black);
  FastLED.show();
  Serial.begin(115200);
  mystdout.put = my_putchar;
  mystdout.flags = _FDEV_SETUP_WRITE;
  stdout = &mystdout;
  printf("\r\nESG DEBUG CONSOLE");
  esg_explore();
}

int index = -1;
char c;
esg_triangle_t t[2];
int n;
char flag = 0;
void loop()
{
  if (Serial.available() > 0)
  {
    c = Serial.read();
    switch (c)
    {
      case '+':
        {
          increment_limit_max(index, STRIPS_NUM - 1);
          flag = 1;
          break;
        }
      case '-':
        {
          decrement_limit_min(index, 0);
          flag = 1;
          break;
        }
    }

   if(flag)
   {
    flag = 0;
    n = esg_get_triangles_from_strip(index, t);
    printf("\r\nStrip %i is a part of %i triangles", index, n);
    fill_solid(leds, NUM_LEDS, CRGB::Black);

    if(n)
    {
      triangle_fill_solid(&t[0], CRGB::Red);
      print_triangle(&t[0]);
      n--;
    }
    if(n)
    {
      triangle_fill_solid(&t[1], CRGB::Red);
      print_triangle(&t[1]);
      n--;
    }

    fill_solid(leds + index * LEDS_PER_STRIP, LEDS_PER_STRIP, CRGB::Green);
    FastLED.show();
    } 
  }
}


void pick_random_triangle(esg_triangle_t * t)
{
  u8 node = 0;
  u8 port = 0;
  do
  {
    node = random8(ARRAY_SIZE(nodes_map) - 1);
    port = random8(5);
  } while(esg_get_triangle(node,port, t) < 0);
}

void triangle_fill_solid(esg_triangle_t * t, const struct CRGB& color)
{
  fill_solid(leds + t->side_id[0] * LEDS_PER_STRIP, LEDS_PER_STRIP, color);
  fill_solid(leds + t->side_id[1] * LEDS_PER_STRIP, LEDS_PER_STRIP, color);
  fill_solid(leds + t->side_id[2] * LEDS_PER_STRIP, LEDS_PER_STRIP, color);
}