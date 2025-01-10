

#include "config.h"
#include "shared_memory.h"
#include <stdlib.h>
#include <stdio.h>
#include "fdmcalc.h"


int main() {
	grid_dimen out = read_fdata("test.csv", "out.csv");
	printf("Size: %d, %d, %d\n", out.size.i, out.size.j, out.size.k);
	printf("Time: %lf - %lf\n", out.ti, out.tf);
	printf("Time interval: %lf\n", out.dt);
	printf("Units: %lf m/cell\n", out.units);
	printf("MATERIALS:\n");
	for (int i=0; i<out.num_materials; i++) {
		printf("\t%d: %lf\n", i, out.matcoeffs[i]);
	}
	remove_shared_mem();
	return 0;
}
