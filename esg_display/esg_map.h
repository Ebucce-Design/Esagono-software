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
    int                 src_node;  
    int                 dst_node;
}esg_node_port_t;

typedef struct 
{
    esg_node_port_t ports[6];
}esg_node_t;


esg_node_t nodes_map[] = 
{//port 0             //port1            //port2            //port3             //port4             //port5
 {{{8,  END,   0, 0}, {2,  END,   0, 0}, {9, START,  0, 0}, {-1, END,    0, 0}, {-1, END,    0, 0}, {-1, END,   0, 0}}}, //node 0
 {{{7,  END,   0, 0}, {3,  END,   0, 0}, {8, START,  0, 0}, {-1, END,    0, 0}, {-1, END,    0, 0}, {-1, END,   0, 0}}}, //node 1
 {{{8,  END,   0, 0}, {2,  END,   0, 0}, {9, START,  0, 0}, {-1, END,    0, 0}, {-1, END,    0, 0}, {-1, END,   0, 0}}}, //node 2
 {{{0,  START, 0, 0}, {1,  START, 0, 0}, {2, START,  0, 0}, {3,  START,  0, 0}, {4,  START,  0, 0}, {5,  START, 0, 0}}}, //node 3
 {{{9,  END,   0, 0}, {1,  END,   0, 0}, {14, END,   0, 0}, {12, END,    0, 0}, {10, START,  0, 0}, {-1, END,   0, 0}}}, //node 4
 {{{13, END,   0, 0}, {14, START, 0, 0}, {0,  END,   0, 0}, {15, START,  0, 0}, {16, START,  0, 0}, {-1, END,   0, 0}}}, //node 5
 {{{5,  END,   0, 0}, {6,  START, 0, 0}, {-1, START, 0, 0}, {-1, START,  0, 0}, {17, END,    0, 0}, {15, END,   0, 0}}}, //node 6
 {{{11, END,   0, 0}, {12, START, 0, 0}, {13, START, 0, 0}, {20, END,    0, 0}, {21, START,  0, 0}, {-1, END,   0, 0}}}, //node 7
 {{{10, END,   0, 0}, {11, START, 0, 0}, {-1, START, 0, 0}, {-1, END,    0, 0}, {-1, START,  0, 0}, {-1, END,   0, 0}}}, //node 8
 {{{21, END,   0, 0}, {22, START, 0, 0}, {23, START, 0, 0}, {-1, END,    0, 0}, {-1, START,  0, 0}, {-1, END,   0, 0}}}, //node 9
 {{{-1, END,   0, 0}, {19, END,   0, 0}, {26, END,   0, 0}, {24, END,    0, 0}, {22, END,    0, 0}, {20, START, 0, 0}}}, //node 10
 {{{23, END,   0, 0}, {24, START, 0, 0}, {25, START, 0, 0}, {-1, END,    0, 0}, {-1, END,    0, 0}, {-1, START, 0, 0}}}, //node 11
 {{{16, END,   0, 0}, {17, START, 0, 0}, {-1, END,   0, 0}, {18, START,  0, 0}, {27, END,    0, 0}, {19, START, 0, 0}}}, //node 12
 {{{25, END,   0, 0}, {26, START, 0, 0}, {27, START, 0, 0}, {28, START,  0, 0}, {-1, END,    0, 0}, {-1, START, 0, 0}}}, //node 13
 {{{28, END,   0, 0}, {18, END,   0, 0}, {-1, END,   0, 0}, {-1, END,    0, 0}, {-1, END,    0, 0}, {-1, START, 0, 0}}}, //node 14
};
#endif