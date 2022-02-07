#ifndef __ESG_MAP
#define __ESG_MAP

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define STRIPS_NUM      29 //User defined
#define LEDS_PER_STRIP  10

typedef enum 
{
    START = 1,
    END   = 2,
} esg_strip_dir_t;

typedef struct
{
    int                 strip_id;   // If no strip is connected to the port it should be -1
    esg_strip_dir_t     strip_dir;   
}esg_node_port_t;

typedef struct 
{
    esg_node_port_t ports[6];
}esg_node_t;

typedef struct
{
  int start_node_id;
  int end_node_id;
  u8 start_port;
  u8 end_port;
} esg_strip_t;

esg_strip_t strips_map[STRIPS_NUM]; //This array will be filled by software

esg_node_t nodes_map[] = 
{//port 0       //port1      //port2      //port3      //port4      //port5
 {{{8,  END  }, {2,  END  }, {9,  START}, {-1, END  }, {-1, END  }, {-1, END  }}}, //node 0
 {{{7,  END  }, {3,  END  }, {8,  START}, {-1, END  }, {-1, END  }, {-1, END  }}}, //node 1
 {{{6,  END  }, {4,  END  }, {7,  START}, {-1, END  }, {-1, END  }, {-1, END  }}}, //node 2
 {{{0,  START}, {1,  START}, {2,  START}, {3,  START}, {4,  START}, {5,  START}}}, //node 3
 {{{9,  END  }, {1,  END  }, {14, END  }, {12, END  }, {10, START}, {-1, END  }}}, //node 4
 {{{13, END  }, {14, START}, {0,  END  }, {15, START}, {16, START}, {-1, END  }}}, //node 5
 {{{5,  END  }, {6,  START}, {-1, START}, {-1, START}, {17, END  }, {15, END  }}}, //node 6
 {{{11, END  }, {12, START}, {13, START}, {20, END  }, {21, START}, {-1, END  }}}, //node 7
 {{{10, END  }, {11, START}, {-1, START}, {-1, END  }, {-1, START}, {-1, END  }}}, //node 8
 {{{21, END  }, {22, START}, {23, START}, {-1, END  }, {-1, START}, {-1, END  }}}, //node 9
 {{{-1, END  }, {19, END  }, {26, END  }, {24, END  }, {22, END  }, {20, START}}}, //node 10
 {{{23, END  }, {24, START}, {25, START}, {-1, END  }, {-1, END  }, {-1, START}}}, //node 11
 {{{16, END  }, {17, START}, {-1, END  }, {18, START}, {27, END  }, {19, START}}}, //node 12
 {{{25, END  }, {26, START}, {27, START}, {28, START}, {-1, END  }, {-1, START}}}, //node 13
 {{{28, END  }, {18, END  }, {-1, END  }, {-1, END  }, {-1, END  }, {-1, START}}}, //node 14
};
#endif