#ifndef CONFIG_H
#define CONFIG_H__

#define COEFKEY 12125       // shared memory key for coefficient array
#define ATEMPKEY 11707      // shared memory key for A temperature array
#define BTEMPKEY 4807       // shared memory key for B temperature array
#define MATKEY 929292       // shared memory key for materials array
#define SEMKEY 1535         // semaphore key
#define LAYERS_PER_SP 1     // max number of layers each subprocess should do calculations for

#define NUM_PARAMETERS 8

#define ACALCB 1311232      // command number to use A to calculate and write to B
#define BCALCA 2311231      // command number to use B to calculate and write to A
#define QUIT 1721920        // command number to use A to calculate and write to B

#endif
