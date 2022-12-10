#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cell.h"
#include "def.h"
#include "fps.h"
#include "gl_canvas2d.h"
#include "input.h"
#include "intersect.h"
#include "rgb.h"
#include "segment.h"
#include "sphash.h"
#include "state.h"
#include "ui.h"
#include "util.h"

static struct fps           fps           = {0};
static struct intersections intersections = {0};
static struct segments      segments      = {0};
static struct ui            ui            = {0};
static struct sphash        sphash        = {0};

#define MAIN_CONSUMPTION (sizeof fps      + sizeof intersections + \
                          sizeof segments + sizeof ui            + \
                          sizeof sphash                          )

#define MAIN_WINDOW_WIDTH  800
#define MAIN_WINDOW_HEIGHT 600
#define MAIN_MAX_RAM_USAGE_ESTIMATE 6000000000
               
_Static_assert(
    MAIN_CONSUMPTION < MAIN_MAX_RAM_USAGE_ESTIMATE,
    "Use less memory!"
);

static enum state state = STATE_IDLE;

static bool is_paused = false;

static const int grids[] = {8, 16, 32};
static const int n_grid = sizeof grids / sizeof *grids;

static const int amounts[] = {5000, 10000, 20000};
static const int n_amount  = sizeof amounts / sizeof *amounts;

static const int n_run = n_grid * n_amount;

static inline int which_grid(int run)
{
    return run % n_grid;
}

static inline int which_amount(int run)
{
    return run / n_grid;
}

static int run = 0;

static int time_hash         = -1;
static int time_intersection = -1;

void update(void)
{
    fps_update(&fps);

    cv_translate(ui.offset_x, ui.offset_y);

    if (!is_paused)
    {
        int     *time_which = NULL;
        clock_t  start      = clock();

        switch (state)
        {
            case STATE_IDLE:
                segments_generate(&segments, amounts[which_amount(run)]);
                state = STATE_DISTRIBUTED;
            break;

            case STATE_DISTRIBUTED:
            {
                int const grid = grids[which_grid(run)];

                sphash.cells.n_line      = grid;
                sphash.cells.n_column    = grid;
                sphash.cells.cell_width  = DEF_TEST_AREA_WIDTH  / (float)grid;
                sphash.cells.cell_height = DEF_TEST_AREA_HEIGHT / (float)grid;

                sphash_update(&sphash, &segments);
                time_which = &time_hash;
                state = STATE_HASHED;
            }
            break;

            case STATE_HASHED:
                intersections_test(&intersections, &segments, &sphash);
                time_which = &time_intersection;
                state = STATE_INTERSECTED;
            break;

            case STATE_INTERSECTED:
                printf(
                    "Took %3d ms to hash and %3d ms to intersect with"
                    " %5d segments and grid %2dx%2d\n",
                    time_hash, time_intersection,
                    amounts[which_amount(run)],
                    grids[which_grid(run)],
                    grids[which_grid(run)]
                );
                run = (run + 1) % n_run;
                state = STATE_IDLE;
            break;

            default:
                assert(!"Untreated state!");
            break;
        }

        clock_t end = clock();

        if (time_which != NULL)
        {
            *time_which = MAX(0, (end - start) * 1000 / CLOCKS_PER_SEC);
        }
    }

    if (STATE_DISTRIBUTED == state || STATE_INTERSECTED == state || STATE_HASHED == state)
    {
        segments_render(&segments, ui.scale);
    }

    if (STATE_INTERSECTED == state)
    {
        intersections_render(&intersections, ui.scale);
    }

    if (STATE_HASHED == state || STATE_INTERSECTED == state)
    {
	cells_render(&sphash.cells, ui.scale);
    }

    ui_render(&ui, fps.fps);
}

void keyboard(int key)
{
    switch (toupper(key))
    {
        case INPUT_KEY_ESC: // Fall through;
        case 'Q': exit(EXIT_SUCCESS); break;

        case 'P': is_paused = !is_paused; break;

        default: /* Do nothing. */ break;
    }
}

void keyboardUp(int key)
{
    (void)key;

    // Do nothing.
}

void mouse(int button, int _state, int wheel, int direction, int x, int y)
{
    (void)button;
    (void)_state;

    ui.mouse_x = x;
    ui.mouse_y = y;

    if (INPUT_SCROLL_YES == wheel)
    {
        if (INPUT_UPWARDS == direction)
        {
            ui_zoom_in(&ui);
        }
        else if (INPUT_DOWNWARDS == direction)
        {
            ui_zoom_out(&ui);
        }
    }

    ui_scale(&ui);
}

void resize(int width, int height)
{
    ui.screen_width  = width;
    ui.screen_height = height;

    ui_scale(&ui);
}

int main(void)
{
    fps_init(&fps);
    srand(time(NULL));

    cv_init(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, "Intersections and spatial hash!");
    cv_run();
}
