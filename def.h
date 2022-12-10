#ifndef __DEF_H__
#define __DEF_H__

#define DEF_TEST_AREA_WIDTH  800
#define DEF_TEST_AREA_HEIGHT 600
#define DEF_TEST_AREA_RATIO  (DEF_TEST_AREA_WIDTH / (float)DEF_TEST_AREA_HEIGHT)

#define DEF_SEGMENT_LENGTH_MIN  1
#define DEF_SEGMENT_LENGTH_MAX 50

_Static_assert(DEF_SEGMENT_LENGTH_MAX >= DEF_SEGMENT_LENGTH_MIN, "Stupid!");

#define DEF_SEGMENT_MAX      5000000
#define DEF_SEGMENT_AMOUNT     25000
#define DEF_SEGMENT_VIEW_MAX ((3840 * 2160) / 5000)

#define DEF_INTERSECTION_VIEW_MAX ((DEF_SEGMENT_VIEW_MAX - 1) * DEF_SEGMENT_VIEW_MAX / 2)

_Static_assert(DEF_SEGMENT_MAX >= 2, "Two or more segments needed");

#define DEF_STATUS_BAR_LENGHT 1000
#define DEF_STATUS_BAR_HEIGHT   15

#define DEF_WINDOW_WIDTH  800
#define DEF_WINDOW_HEIGHT 600

#define DEF_CELLS_PER_SEGMENT_ESTIMATE   40
#define DEF_CELLS_LINE_MAX              128
#define DEF_CELLS_COLUMN_MAX            128
#define DEF_CELLS_MAX (DEF_CELLS_LINE_MAX * DEF_CELLS_COLUMN_MAX)

#define DEF_MAX_RAM_USAGE_ESTIMATE 6000000000

#define DEF_ZOOM_FACTOR  4
#define DEF_ZOOM_DOUBLES 6
#define DEF_ZOOM_MAX     DEF_ZOOM_FACTOR * DEF_ZOOM_DOUBLES

#define DEF_MOUSE_CLAMP_MARGIN 0.05f

#endif
