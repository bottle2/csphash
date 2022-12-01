#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "gl_canvas2d.h"

#include "def.h"
#include "segment.h"
#include "sphash.h"
#include "rgb.h"
#include "util.h"

void sphash_update(struct sphash sphash[static 1], struct segments segments[static 1])
{
    assert(sphash   != NULL);
    assert(segments != NULL);

    assert(sphash->n_line >= 1);
    assert(sphash->n_line <= DEF_CELLS_LINE_MAX);
    assert(sphash->n_column >= 1);
    assert(sphash->n_column <= DEF_CELLS_COLUMN_MAX);

    memset(sphash->cells_per_object, -1, sizeof (sphash->cells_per_object));

    float const cell_height = segments->max_y / (float)sphash->n_line;
    float const cell_width  = segments->max_x / (float)sphash->n_column;

    for (int segment_i = 0; segment_i < segments->amount; segment_i++)
    {
        struct segment const *segment = segments->them + segment_i;

        if (segment->p1_x == segment->p2_x)
        {
            assert(!"Not implemented");
#if 0
            float const min_y = MIN(segment->p1_y, segment->p2_y);
            float const max_y = MAX(segment->p1_y, segment->p2_y);

            int const cell_x = segment->p1_x / cell_width;

            //int const cell_y // XXX Finish!

            for (int cell_y = min_y / cell_height; cell_y < max_y / cell_height; cell_y++)
            {
                sphash->cells_per_object[segment_i][cell_i] = cell_y;
            }
#endif
        }
        else // Using line equation to map segment into cells.
        {
            float const a = 0.0f / 0.0f;
            float const b = 0.0f / 0.0f;
        }

#if 0
        for (int cell_i = 0; cell_i < DEF_CELLS_PER_SEGMENT_ESTIMATE; cell_i++)
        {
            if (-1 == cells_per_object[segment_i])
            {
                break;
            }
        }
#endif
    }
    // Map lines to cells and set usage.

    for (int segment_i = 0; segment_i < segments->amount; segment_i++)
    {
    }
    // Setup table and final indices.

    for (int segment_i = 0; segment_i < segments->amount; segment_i++)
    {
    }
    // Map lines to table.
}

void sphash_render(struct sphash sphash[static 1], float max_y, float max_x, float scale)
{
    assert(sphash != NULL);

    cv_color(RGB_BLUE);

    float const cell_height = max_y / (float)sphash->n_line;
    float const cell_width  = max_x / (float)sphash->n_column;

    for (int line_i = 0; line_i < sphash->n_line - 1; line_i++)
    {
        float const y = cell_height * (line_i + 1) * scale;
        cv_line(0.0f, y, max_x * scale, y);
    }
    for (int column_i = 0; column_i < sphash->n_column - 1; column_i++)
    {
        float const x = cell_width * (column_i + 1) * scale;
        cv_line(x, 0, x, max_y * scale);
    }

    for (int cell_i = 0; cell_i < sphash->n_line * sphash->n_column; cell_i++)
    {
        int const column_i = cell_i / sphash->n_line;
        int const row_i    = cell_i % sphash->n_line;

        float const text_pos_x = scale * column_i * cell_width  + 2.0f;
        float const text_pos_y = scale * row_i    * cell_height + 2.0f;

        char buffer[100] = {0};
        sprintf(buffer, "%2d(%1d,%1d)", cell_i + 1, row_i + 1, column_i + 1);
        cv_text(text_pos_x, text_pos_y, buffer);
        sprintf(buffer, "%d", sphash->cells_usage[cell_i]);
        cv_text(text_pos_x, text_pos_y + 11.0f, buffer);
    }
}