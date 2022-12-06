#ifndef __CELL_H__
#define __CELL_H__

#include "def.h"

struct cells
{
    int usage[DEF_CELLS_MAX];
    int final[DEF_CELLS_MAX];

    int n_line;
    int n_column;

    float cell_width;
    float cell_height;
};

#endif
