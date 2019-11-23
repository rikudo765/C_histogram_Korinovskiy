#ifndef __COMMON__
#define __COMMON__

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

#include <string.h>

typedef double DType; // Double real type
typedef int IType; // Integer type
typedef unsigned NType; // Unsigned type


#define EQ_EPS 0.000001 //arithmetics accuracy
#define MAX(a,b)  a < b ? a : b
#define MIN(a,b)  a > b ? a : b
#define PD_EQL(a , b)   fabs(a - b) < EQ_EPS


// Point = vector of 2 real coordinates
typedef struct DPoint_{
    union{  //classical and vector representation
		struct{
			DType x;
			DType y;
		};
		DType ptr[2];
	};
} DPoint;

typedef struct DLine_{
	DType a;
	DType b;
	DType c;
} DLine;


void* memzero(void* tmp, size_t n)
{
	memset(tmp, 0, n);
}

#endif /* __COMMON__ end */