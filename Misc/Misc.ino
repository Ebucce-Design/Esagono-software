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
u8 esagono_count_node_triangles(u8 n);
int esagono_check_nodes_connected(u8 id1, u8 id2);
void esagono_node_blow(esagono_node_t * node, CRGB color, u16 delay_ms);
u8 esagono_get_triangle_from_node_port(u8 n, u8 port, esagono_triangle_t * t);
void esagono_triangle_fill(esagono_triangle_t * t, CRGB color);
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
  esagono_explore_nodes();
}

void loop()
{ 
  u8 port = 0;
  u8 n  = 0;
  esagono_triangle_t  t = {0};
  while(1)
  {
  if(esagono_get_triangle_from_node_port(12, port, &t))
  {
     esagono_triangle_fill(&t, CRGB::Red);
     FastLED.show();
     delay(500);
     esagono_triangle_fill(&t, CRGB::Black);
     FastLED.show();
     delay(500);
  }
  port = (port + 1) % 6;
  }
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


u8 esagono_count_node_triangles(u8 n)
{
  u8 t = 0;
  for(u8 port = 0; port < 6; port++)
  {
    u8 next_port = (port + 1) % 6;
    if((nodes[n].dir[port])&&(nodes[n].dir[next_port]))
    {
      if(esagono_check_nodes_connected(nodes[n].next_node_id[port],nodes[n].next_node_id[next_port])!=0xFFFF) {t++;}
    }  
  }
  return t;
}



//returns 1 if triangle can be created
//else returns 0
u8 esagono_get_triangle_from_node_port(u8 n, u8 port, esagono_triangle_t * t)
{
  u8 next_port = (port + 1) % 6;
  if((nodes[n].dir[port])&&(nodes[n].dir[next_port]))
    {
      u8 b = esagono_check_nodes_connected(nodes[n].next_node_id[port],nodes[n].next_node_id[next_port]);
      if(b!=0xFFFF)
      {
       u8 tmp;
       t->vertex_nodes[0] = n;
       t->vertex_nodes[1] = nodes[n].next_node_id[port];
       t->vertex_nodes[2] = nodes[n].next_node_id[next_port];
       t->sides[0] = nodes[n].id[port];
       t->sides[1] = b;
       t->sides[2] = nodes[n].id[next_port]; 
       return 1;
     }
    }  
  return 0;
}

void esagono_triangle_fill(esagono_triangle_t * t, CRGB color)
{
  for(u8 i = 0; i<3; i++)
  {
  fill_solid(leds + (t->sides[i]*10), 10, color);
  }
}

//returns 0xFFFF is nodes are not connected
//returns id of the strip that connects two nodes
int esagono_check_nodes_connected(u8 id1, u8 id2)
{
  for(u8 port1 = 0; port1 < 6; port1++)
  {
   if(nodes[id1].dir[port1])
   {
     for(u8 port2 = 0; port2 < 6; port2++)
     {
      if(nodes[id2].dir[port2])
      {
      if(nodes[id1].id[port1] == nodes[id2].id[port2]) {return nodes[id1].id[port1];}
      }
    }
   }
  }
return 0xFFFF;
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
