#ifndef TYPES_H
#define TYPES_H 1

typedef unsigned char BYTE;

#ifdef WIN32
typedef __int64 LONGLONG;
#define LONGLONGLITERAL(LLL) LLL ## i64
#else
typedef long long LONGLONG;
#define LONGLONGLITERAL(LLL) LLL ## LL
#endif

typedef int ERROR;

const int EOUTOFDATA = 21075;

#define SUCCEEDED(x) ((x) >= 0)
#define FAILED(x) ((x) < 0)

#endif
