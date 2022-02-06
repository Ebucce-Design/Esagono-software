
#include "esg_map.h"
typedef struct
{
  int start_node;
  int end_node;
  u8 start_port;
  u8 end_port;
}

#define ESG_GET_LED_INDEX(strip, pos)  ( ((pos) > 4) ? ((strip) * 10 + (14 - (pos))) : ((strip) * 10 + (pos)) )


// int distance_from_node_to_led_index(esagono_node_t * node, u8 port, u8 distance)
// {
//   if (node->dir[port] == 0) return -1;
//   if (node->dir[port] == 1) return esagono_get_led_index(node->id[port], distance);
//   if (node->dir[port] == 2) return esagono_get_led_index(node->id[port], 9 - distance);
// }


//returns 0xFFFF is nodes are not connected
//returns id of the strip that connects two nodes
// int esagono_check_nodes_connected(u8 id1, u8 id2)
// {
//   for(u8 port1 = 0; port1 < 6; port1++)
//   {
//    if(nodes[id1].dir[port1])
//    {
//      for(u8 port2 = 0; port2 < 6; port2++)
//      {
//       if(nodes[id2].dir[port2])
//       {
//       if(nodes[id1].id[port1] == nodes[id2].id[port2]) {return nodes[id1].id[port1];}
//       }
//     }
//    }
//   }
// return 0xFFFF;
// }

//this function completes the map array.
//It should be called before prior to using esagono.
void esg_init()
{
  u8 tmp;
  esg_node_t * node = nodes_map;
  for(int i = 0; i < ARRAY_SIZE(nodes_map); i++)
  {
    for(u8 p = 0; p < 6; p++)
    {
      if((node->ports[p].strip_id) >= 0)
      

    }
  }
}
