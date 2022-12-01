#ifndef __UTIL_H__
#define __UTIL_H__

#define MIN(  A, B   ) ((A) < (B) ? (A) : (B))
#define MAX(  A, B   ) ((A) > (B) ? (A) : (B))
#define CLAMP(A, B, C) MIN(MAX((A), (B)), (C))

#endif
