#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "gl_canvas2d.h"

#include "def.h"
#include "segment.h"
#include "sphash.h"
#include "rgb.h"
#include "util.h"

static void sphash_render_line  (int n_line  , float y_step, float width );
static void sphash_render_column(int n_column, float x_step, float height);

static void sphash_render_caption(
    int n_line,
    int n_column,
    int cell_width,
    int cell_height,
    int const usage[static DEF_CELLS_MAX]
);

static int sphash_hash_vertical(
    int   x,
    int   one_y,
    int   other_y,
    float cell_height, 
    float cell_width,
    int   n_line,
    int   cells[static DEF_CELLS_PER_SEGMENT_ESTIMATE],
    int   cells_usage[static DEF_CELLS_MAX]
);

static int sphash_hash_linear(
    struct segment segment[static 1],
    float cell_width,
    float cell_height,
    int   n_line,
    int   cells[static DEF_CELLS_PER_SEGMENT_ESTIMATE],
    int   cells_usage[static DEF_CELLS_MAX]
);

static int sphash_hash(
    struct segment segment[static 1],
    float cell_width,
    float cell_height,
    int n_line,
    int cells_per_object[static DEF_CELLS_PER_SEGMENT_ESTIMATE],
    int cells_usage[static DEF_CELLS_MAX]
);

void sphash_update(struct sphash sphash[static 1], struct segments segments[static 1])
{
    assert(sphash   != NULL);
    assert(segments != NULL);

    assert(sphash->n_line >= 1);
    assert(sphash->n_line <= DEF_CELLS_LINE_MAX);
    assert(sphash->n_column >= 1);
    assert(sphash->n_column <= DEF_CELLS_COLUMN_MAX);

    memset(sphash->n_cell_per_object, 0, sizeof (sphash->n_cell_per_object));
    memset(sphash->cells_usage, 0, sizeof (sphash->cells_usage));

    int const n_column = sphash->n_column;
    int const n_line   = sphash->n_line;

    float const cell_width  = DEF_TEST_AREA_WIDTH  / (float)n_column;
    float const cell_height = DEF_TEST_AREA_HEIGHT / (float)n_line;

    for (int segment_i = 0; segment_i < segments->amount; segment_i++)
    {
        sphash->n_cell_per_object[segment_i] = sphash_hash(
            segments->them + segment_i,
            cell_width,
            cell_height,
            n_line,
            sphash->cells_per_object[segment_i],
            sphash->cells_usage
        );
    }
    // Map lines to cells and set usage.

    for (int accum = 0, cell_i = 0; cell_i < n_line * n_column; cell_i++)
    {
        sphash->table[cell_i] = sphash->objects_per_cell + accum;
        accum += sphash->cells_usage[cell_i];
        sphash->cells_final[cell_i] = accum;
    }
    // Setup table and final indices.

    for (int segment_i = 0; segment_i < segments->amount; segment_i++)
    {
        for (int cell_i = 0; cell_i < sphash->n_cell_per_object[segment_i]; cell_i++)
        {
            int const cell = sphash->cells_per_object[segment_i][cell_i];
            sphash->objects_per_cell[--sphash->cells_final[cell]] = segment_i;
        }
    }
    // Map lines to table.
}

void sphash_render(struct sphash sphash[static 1], float scale)
{
    assert(sphash != NULL);

    cv_color(RGB_BLUE);

    int const n_line   = sphash->n_line;
    int const n_column = sphash->n_column;

    float const cell_width  = scale * DEF_TEST_AREA_WIDTH  / sphash->n_column;
    float const cell_height = scale * DEF_TEST_AREA_HEIGHT / sphash->n_line;

    int const *usage = sphash->cells_usage;

    sphash_render_line  (n_line  , cell_height, DEF_TEST_AREA_WIDTH  * scale);
    sphash_render_column(n_column, cell_width , DEF_TEST_AREA_HEIGHT * scale);
    sphash_render_caption(n_line, n_column, cell_width, cell_height, usage);
}

static int sphash_hash(
    struct segment segment[static 1],
    float cell_width,
    float cell_height,
    int n_line,
    int cells[static DEF_CELLS_PER_SEGMENT_ESTIMATE],
    int cells_usage[static DEF_CELLS_MAX]
) {
    assert(segment != NULL);

    if (segment->p1_x == segment->p2_x)
    {
        return sphash_hash_vertical(
            segment->p1_x,
            segment->p1_y,
            segment->p2_y,
            cell_height,
            cell_width,
            n_line,
            cells,
            cells_usage
        );
    }
    else // Using line equation to map segment into cells.
    {
        return sphash_hash_linear(segment, cell_width, cell_height, n_line, cells, cells_usage);
    }
}

static void sphash_render_line(int n_line, float y_step, float width)
{
    for (int line_i = 0; line_i < n_line - 1; line_i++)
    {
        float const y = y_step * (line_i + 1);
        cv_line(0.0f, y, width, y);
    }
}

static void sphash_render_column(int n_column, float x_step, float height)
{
    for (int column_i = 0; column_i < n_column - 1; column_i++)
    {
        float const x = x_step * (column_i + 1);
        cv_line(x, 0, x, height);
    }
}

static void sphash_render_caption(
    int n_line,
    int n_column,
    int cell_width,
    int cell_height,
    int const usage[static DEF_CELLS_MAX]
) {
    for (int cell_i = 0; cell_i < n_line * n_column; cell_i++)
    {
        int const column_i = cell_i / n_line;
        int const row_i    = cell_i % n_line;

        float const text_pos_x = column_i * cell_width  + 5.0f;
        float       text_pos_y = row_i    * cell_height + 5.0f;

        char buffer[100] = {0};

        sprintf(buffer, "%d(%d,%d)", cell_i + 1, column_i + 1, row_i + 1);
	if (strlen(buffer) * (CV_CHAR_WIDTH + 1) < cell_width - 10.0f)
	{
            cv_text(text_pos_x, text_pos_y, buffer);
            text_pos_y += CV_CHAR_HEIGHT;
	}

        sprintf(buffer, "%d", usage[cell_i]);
	if (strlen(buffer) * (CV_CHAR_WIDTH + 1) < cell_width - 10.0f)
	{
            cv_text(text_pos_x, text_pos_y, buffer);
	}
    }
}

static int sphash_hash_vertical(
    int   x,
    int   one_y,
    int   other_y,
    float cell_height, 
    float cell_width,
    int   n_line,
    int   cells[static DEF_CELLS_PER_SEGMENT_ESTIMATE],
    int   cells_usage[static DEF_CELLS_MAX]
) {
    float const min_y = MIN(one_y, other_y);
    float const max_y = MAX(one_y, other_y);

    int const column_i = x / cell_width;

    int const row_from = min_y / cell_height;
    int const row_to   = max_y / cell_height;

    for (int row_i = row_from; row_i <= row_to; row_i++)
    {
        int const cell_i = column_i * n_line + row_i;
        cells[row_i - row_from] = cell_i;
        cells_usage[cell_i]++;
    }

    return row_to - row_from + 1;
}

static int sphash_hash_linear(
    struct segment segment[static 1],
    float cell_width,
    float cell_height,
    int   n_line,
    int   cells[static DEF_CELLS_PER_SEGMENT_ESTIMATE],
    int   cells_usage[static DEF_CELLS_MAX]
) {
    float const a = (segment->p2_y - segment->p1_y) / (segment->p2_x - segment->p1_x);
    float const b = (segment->p2_x * segment->p1_y - segment->p1_x * segment->p2_y) / (segment->p2_x - segment->p1_x);
    // a*x + b = y.

    float const min_x = MIN(segment->p1_x, segment->p2_x);
    float const max_x = MAX(segment->p1_x, segment->p2_x);

    int const column_from = min_x / cell_width;
    int const column_to   = max_x / cell_width;

    int accum = 0;

    for (int column_i = column_from; column_i <= column_to; column_i++)
    {
        float const x_one   = column_from == column_i ? min_x : column_i * cell_width;
        float const x_other = column_to   == column_i ? max_x : nextafterf((column_i + 1) * cell_width, -HUGE_VAL);

        float const y_one   = (a * x_one   + b);
        float const y_other = (a * x_other + b);

        float const min_y = MIN(y_one, y_other);
        float const max_y = MAX(y_one, y_other);

        int const row_from = min_y / cell_height;
        int const row_to   = max_y / cell_height;

        for (int row_i = row_from; row_i <= row_to; row_i++)
        {
            assert(accum < DEF_CELLS_PER_SEGMENT_ESTIMATE);

            int cell_i = column_i * n_line + row_i;

            cells[accum++] = cell_i;
            cells_usage[cell_i]++;
        }
    }

    return accum;
}
