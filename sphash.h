#ifndef __SPHASH_H__
#define __SPHASH_H__

#include "cell.h"
#include "def.h"
#include "segment.h"

struct sphash
{
    int n_cell_per_object[DEF_SEGMENT_MAX];
    int cells_per_object[DEF_SEGMENT_MAX][DEF_CELLS_PER_SEGMENT_ESTIMATE];
    int objects_per_cell[DEF_SEGMENT_MAX *DEF_CELLS_PER_SEGMENT_ESTIMATE];

    int *table[DEF_CELLS_MAX];

    struct cells cells;
};

void sphash_update(struct sphash sphash[static 1], struct segments segments[static 1]);

#endif
