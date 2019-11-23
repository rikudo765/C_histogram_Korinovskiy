#ifndef __HIST_H__
#define _CRT_SECURE_NO_WARNINGS
#define __HIST_H__
#include "common.h"
#include "stdlib.h"
#define true 1
#define false 0

// Each "bucket" is being described as left order and width
// Each element finds its "bucket" by: left_order <= elem <= right order, where right_order = left_order + width
struct hist {
	DType minHist;
	DType maxHist;
	DType bWidth;
	NType M;
	NType *frequency;
};

NType num(struct hist *h);
NType numHist(struct hist *h, NType i);
DType mean(struct hist *h);
DType median(struct hist *h);
DType dev(struct hist *h);
DType var(struct hist *h);

void init_struct(DType minHist, DType maxHist, NType M, struct hist *res);

void set_max(DType m, struct hist *h);

void set_min(DType m, struct hist *h);

void set_m(NType n, struct hist *h);

int add_number(DType x, struct hist *h);

int add_batch(DType data[], size_t dataSize, struct hist *h);

int add_batch_from_file(char *filename, char mode, struct hist *h);

int _batch_helper_text(char *filename, DType *data, size_t size, long *fpos);

int _batch_helper_bin(char *filename, DType *data, size_t size, long *fpos);

#endif