#ifndef __STATE_H__
#define __STATE_H__

#include "expand.h"

#define STATE_XS(X)                                             \
X(STATE_IDLE       , "Press D to distribute line segments"    ), \
X(STATE_DISTRIBUTED, "Press E to execute, D to redistribute"  ), \
X(STATE_INTERSECTED, "Press E to reexecute, D to redistribute"),

enum state { STATE_XS(EXPAND_AS_FIRST_FROM_TWO) };

// TODO Systematize input and rendering.

#endif
