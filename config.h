#ifndef __CONFIG__H__
#define __CONFIG__H__

#include "line.h"
#include "state.h"

struct config
{
    float area_width;
    float area_height;

    float segment_length_max;

    enum state state;
};

#endif
