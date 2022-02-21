#include "esg_map.h"

#define ESG_GET_LED_INDEX(strip, pos)  ( ((pos) > 4) ? ((strip) * 10 + (14 - (pos))) : ((strip) * 10 + (pos)) )

#define increment_limit_max(val, max) ((val < max) ? (val++): (val = max))
#define decrement_limit_min(val, min) ((val > min) ? (val--): (val = min))
#define increment_rollover(val, min, max) ((val < max) ? (val++) : (val = min))
#define decrement_rollover(val, min, max) ((val > min) ? (val--) : (val = max))

#define port_increment(port) ((port < 6) ? (port++):(port = 0))
#define port_decrement(port) ((port > 0) ? (port--):(port = 5))
typedef struct
{
   int side_id[3]; //Numbers of sides in the array sides_map
}esg_triangle_t;

void esg_triangles_sort(esg_triangle_t * t);

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

//returns id of the strip that connects two nodes.
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


//returns id of the node that connects two strips.
//if no such node found returns -1
int esg_get_node_between_strips(int strip1_id, int strip2_id)
{
  if((strip1_id < 0)||(strip2_id < 0)) return -1;
  if((strip1_id == strip2_id)) return -1;
  int common_node = -1;
  
  esg_strip_t * s1 = &strips_map[strip1_id];
  esg_strip_t * s2 = &strips_map[strip2_id];

  if(s1->end_node_id    == s2->end_node_id)      common_node = s1->end_node_id;
  if(s1->start_node_id  == s2->start_node_id)    common_node = s1->start_node_id;
  if(s1->end_node_id    == s2->start_node_id)    common_node = s1->end_node_id;
  if(s1->start_node_id  == s2->end_node_id)      common_node = s1->start_node_id;

  return common_node;
}

int esg_get_triangles_from_strip(int strip_id, esg_triangle_t * t_array)
{
  u8 n = 0;
  esg_strip_t * strip      = &strips_map[strip_id];
  esg_node_t  * start_node = &nodes_map[strip->start_node_id];
  esg_node_t  * end_node   = &nodes_map[strip->end_node_id];
  u8 port1_id = strip->start_port;
  u8 port2_id = strip->end_port;
  increment_rollover(port1_id, 0, 5); 
  decrement_rollover(port2_id, 0, 5);

  if(esg_get_node_between_strips(start_node->ports[port1_id].strip_id, end_node->ports[port2_id].strip_id) >= 0)
  {
    t_array[n].side_id[0] = strip_id;
    t_array[n].side_id[1] = start_node->ports[port1_id].strip_id;
    t_array[n].side_id[2] = end_node->ports[port2_id].strip_id;
    n++;
  }

  port1_id = strip->start_port;
  port2_id = strip->end_port;
  decrement_rollover(port1_id, 0, 5); 
  increment_rollover(port2_id, 0, 5);

  if(esg_get_node_between_strips(start_node->ports[port1_id].strip_id, end_node->ports[port2_id].strip_id) >= 0)
  { 
    t_array[n].side_id[0] = strip_id;
    t_array[n].side_id[1] = start_node->ports[port1_id].strip_id;
    t_array[n].side_id[2] = end_node->ports[port2_id].strip_id;
    n++;
  }
  return n;
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
  //port = (port + 1) % 6;
  port_increment(port);

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
  esg_triangles_sort(t);
  return 0;
}


void esg_triangles_sort(esg_triangle_t * t)
{
  int n;
  if(t->side_id[0] > t->side_id[1])
  {
    n = t->side_id[1];
    t->side_id[1] = t->side_id[0];
    t->side_id[0] = n;
  }

  if(t->side_id[0] > t->side_id[2])
  {
    n = t->side_id[2];
    t->side_id[2] = t->side_id[0];
    t->side_id[0] = n;
  }
  if(t->side_id[2] < t->side_id[1])
  {
    n = t->side_id[2];
    t->side_id[2] = t->side_id[1];
    t->side_id[1] = n;
  }
}

//returns 1 if triangles are the same
//returns 0 if provided triangles are in fact different
int esg_check_triangles_equal(esg_triangle_t * t1, esg_triangle_t * t2)
{
  if((t1->side_id[0]) == (t2->side_id[0]) && (t1->side_id[1]) == (t2->side_id[1])) return 1;
  else return 0;
}
///////////////////////
////Debug functions////
///////////////////////

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

void print_triangle(esg_triangle_t * t)
{
  Serial.print("\r\nTriangle data:");
  print_strip(t->side_id[0]);
  print_strip(t->side_id[1]);
  print_strip(t->side_id[2]);
}