#ifndef TYPES_H
#define TYPES_H 1

typedef unsigned char BYTE;

typedef int ERROR;

const int EOUTOFDATA = 21075;

#define SUCCEEDED(x) ((x) >= 0)
#define FAILED(x) ((x) < 0)

#endif
