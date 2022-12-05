#ifndef __SEGMENT_H__
#define __SEGMENT_H__

#include <stdbool.h>

#include "def.h"

struct segment
{
    float p1_x;
    float p1_y;
    float p2_x;
    float p2_y;
};

struct segments
{
    int amount;
    struct segment them[DEF_SEGMENT_MAX];
};

void segments_generate(struct segments segments[static 1]);
void segments_render  (struct segments segments[static 1], float scale);

#endif
