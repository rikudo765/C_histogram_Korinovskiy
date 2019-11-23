#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "../headers/custom.h"
#include "../src/Korinovskiy_test.c"

extern int test_file_text();
extern int test_file_bin();
extern int test_hist();

int main()
{
    test_hist();
	return 0;
}
