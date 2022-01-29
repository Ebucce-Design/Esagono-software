#include <FastLED.h>
#include <stdio.h>
#define STRIPS_NUM  14
#define LEDS_PER_STRIP  10

#define NUM_LEDS    (STRIPS_NUM * LEDS_PER_STRIP)
#define DATA_PIN    A0
#define DELAY_MS    50
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
CRGB leds[NUM_LEDS];
typedef struct 
{
  unsigned char id[6];  //Number of the strip that is connected to respective port of the node
  unsigned char dir[6]; //0 if no strip connected
                        //1 if the input of the strip is connected to the node
                        //2 if the output of the strip is connected to the node
}esagono_node_t;

esagono_node_t node_0 = 
{
  {0,1,2,3,4,5},
  {1,1,1,1,1,1}
};



int esagono_get_led_index(u8 strip, u8 pos);

void setup() 
{ 
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  
  Serial.begin(115200); 
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  Serial.write("\r\nEsagono test and setup sketch.");
  Serial.write("\r\nPress Q to go to the next LED, press W to go one LED back.");
  Serial.write("\r\nPress A to go to the next LED strip, press S to go one strip back.");
}

CRGB colors[] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::White, CRGB::Black };

int index = 0;
u8 c = 0;

void loop() 
{
  if (Serial.available() > 0) 
  {
      c = Serial.read();
      switch(c)
      {
        case 'q':
        {
          if(index < NUM_LEDS)
          {
            index++;
          }
          break;
        }
        case 'w':
        {
          if(index > 0)
          {
            index--;
          }
          break;
        }
        case 'a':
        {
          if (index + 10 < NUM_LEDS)
          {
            index += 10;
          } 
          else 
          {
            index = NUM_LEDS;
          }
          break;
        }
        case 's':
        {
          if(index > 10)
          {
            index -= 10;
          }
          else 
          {
            index = 0;
          }
          break;
        }
      }
      for(int i = 0; i < index; i++)
      {
        leds[i] = CRGB::White;
      }
      for(int i = index; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::Black;
      }
      Serial.print("\r\nIndex: ");
      Serial.print(index);
      FastLED.show();
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
