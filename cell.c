#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "gl_canvas2d.h"

#include "cell.h"
#include "rgb.h"

static void cells_render_grid   (struct cells cells[static 1], float scale);
static void cells_render_caption(struct cells cells[static 1], float scale);

void cells_render(struct cells cells[static 1], float scale)
{
    assert(cells != NULL);

    cv_color(RGB_BLUE);

    cells_render_grid   (cells, scale);
    cells_render_caption(cells, scale);
}

static void cells_render_grid(struct cells cells[static 1], float scale)
{
    int   const n_line      = cells->n_line;
    int   const n_column    = cells->n_column;
    float const cell_width  = cells->cell_width  * scale;
    float const cell_height = cells->cell_height * scale;
    float const width       = cell_width  * n_column;  
    float const height      = cell_height * n_line;  

    for (int line_i = 0; line_i < n_line - 1; line_i++)
    {
        float const y = cell_height * (line_i + 1);
        cv_line(0.0f, y, width, y);
    }

    for (int column_i = 0; column_i < n_column - 1; column_i++)
    {
        float const x = cell_width * (column_i + 1);
        cv_line(x, 0, x, height);
    }
}

static void cells_render_caption(struct cells cells[static 1], float scale)
{
    assert(cells != NULL);

    int   const  n_line      = cells->n_line;
    int   const  n_column    = cells->n_column;
    float const  cell_width  = cells->cell_width  * scale;
    float const  cell_height = cells->cell_height * scale;
    int   const *usage       = cells->usage;

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
