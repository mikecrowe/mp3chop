#ifndef TYPES_H
#define TYPES_H 1

// Get hold of stdint.h if we have it, otherwise fake it up.
#include "types2.h"

typedef uint8_t BYTE;
typedef uint64_t UINT64;

#ifdef _MSC_VER
#define UINT64LITERAL(LLL) LLL ## i64
#else
#define UINT64LITERAL(LLL) LLL ## LL
#endif

typedef int ERROR;

const int EOUTOFDATA = 21075;

#define SUCCEEDED(x) ((x) >= 0)
#define FAILED(x) ((x) < 0)

#endif
