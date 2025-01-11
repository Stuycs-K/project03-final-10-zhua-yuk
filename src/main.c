#include "config.h"
#include "shared_memory.h"
#include <stdlib.h>
#include <stdio.h>
#include "fdmcalc.h"


int main() {
    grid_dimen dimen = read_fdata("test.csv", "out.csv");
    write_data("out.csv", dimen.size, 0);
    write_data("out.csv", dimen.size, 0);
    write_data("out.csv", dimen.size, 0);
    remove_shared_mem();
}
