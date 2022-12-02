#ifndef __SPHASH_H__
#define __SPHASH_H__

#include "def.h"
#include "segment.h"

struct sphash
{
    int n_cell_per_object[DEF_SEGMENT_MAX];
    int cells_per_object[DEF_SEGMENT_MAX][DEF_CELLS_PER_SEGMENT_ESTIMATE];
    int objects_per_cell[DEF_SEGMENT_MAX *DEF_CELLS_PER_SEGMENT_ESTIMATE];

    int  cells_usage[DEF_CELLS_MAX];
    int  cells_final[DEF_CELLS_MAX];
    int *table      [DEF_CELLS_MAX];

    int n_line;
    int n_column;
};

void sphash_update(struct sphash sphash[static 1], struct segments segments[static 1]);

void sphash_render(struct sphash sphash[static 1], float max_y, float max_x, float scale);

#endif
