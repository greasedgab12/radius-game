#ifndef MUSIC_H
#define MUSIC_H

#include <inttypes.h>
#include <avr/pgmspace.h>

// a Change has a size of 32 bits
typedef struct __attribute__((packed)) {
    uint16_t delay;
    unsigned track: 2;
    unsigned increment: 14;
} Change;
typedef union {
    Change change;
    uint32_t bits;
} Event;

#define STOP { { .delay = 0xFFFF, .track = 0, .increment = 0 } }

extern const Event song1[117] PROGMEM;

#endif