
#include "esg_map.h"

#define ESG_GET_LED_INDEX(strip, pos)  ( ((pos) > 4) ? ((strip) * 10 + (14 - (pos))) : ((strip) * 10 + (pos)) )

typedef struct
{
   int side_id[3]; //Numbers of sides in the array sides_map
}esg_triangle_t;


//This function completes the mapping data based on user defined mapping array.
//This should be called before using esagono
void esg_explore()
{
  esg_node_t * node = nodes_map;
  for(int i = 0; i < ARRAY_SIZE(nodes_map); i++)
  {
    for(uint8_t p = 0; p < 6; p++)
    {
      if(node->ports[p].strip_id >= 0)
      {
        if(node->ports[p].strip_dir == START)
        {
          strips_map[node->ports[p].strip_id].start_node_id = i;
          strips_map[node->ports[p].strip_id].start_port    = p;          
        }
        else
        {
          strips_map[node->ports[p].strip_id].end_node_id = i;
          strips_map[node->ports[p].strip_id].end_port    = p;
        }
      }
    }
    node++;
  }
}

//returns is of the strip that connects two nodes.
//if no such strip found returns -1
int esg_get_strip_between_nodes(int node1_id, int node2_id)
{
  if(node1_id > ARRAY_SIZE(nodes_map)) return -1;
  if(node2_id > ARRAY_SIZE(nodes_map)) return -1;
  for(int i = 0; i < STRIPS_NUM; i++)
  {
    if((strips_map[i].start_node_id == node1_id)||(strips_map[i].end_node_id == node1_id))
    {
      if((strips_map[i].start_node_id == node2_id)||(strips_map[i].end_node_id == node2_id))
      {
        return i;
      }
    }
  }
  return -1;
}

//returns -1 if no triangle can be found
//returns 0 if triangle was found
//all the triangles should have their sides sorted in the ascending order
//for the ease of further comparing

int esg_get_triangle(int node_id, u8 port, esg_triangle_t * t)
{ 
  if(node_id > ARRAY_SIZE(nodes_map)) return -1;
 
  esg_node_t * node = &nodes_map[node_id];
  
  int node_id2,node_id3;
  
  int sid = node->ports[port].strip_id; 
  if (sid < 0) return -1;
  t->side_id[0] = sid;
  if(node->ports[port].strip_dir == START)
  {
    node_id2 = strips_map[sid].end_node_id; 
  }
  else   
  {
    node_id2 = strips_map[sid].start_node_id;
  }

  port = (port + 1) % 6;
  sid = node->ports[port].strip_id;
  if (sid < 0) return -1;

  t->side_id[1] = sid;
  if(node->ports[port].strip_dir == START)
  {
    node_id3 = strips_map[sid].end_node_id;
  }
  else   
  {
    node_id3 = strips_map[sid].start_node_id;
  }

  sid = esg_get_strip_between_nodes(node_id2,node_id3);
  if(sid < 0) return -1;
  t->side_id[2] = sid;
  return 0;
}
