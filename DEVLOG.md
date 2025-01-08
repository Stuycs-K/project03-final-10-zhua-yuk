# Dev Log:

This document must be updated daily by EACH group member.

## Kellen Yu

### 2025-01-06 - Brief description
- Wrote the proposal (15 min)

### 2025-01-07 - project structure, makefile, function headers for all
- added nice project structure (15 min)
- updated headers and documentation for fdmcalc.h (5 min)
- updated headers and documentation for memory.h (5 min)
- updated headers and documentation for subprocess.h (5 min)
- added types.h (3 min)

### 2025-01-08


## Ashley Zhu

### 2025-01-06 - readme, makefile, fdmcalc.c, fdmcalc.h, config.h, memory.h
- Filled out information in readme (5 min)
- Updated makefile (7 min)
- Created header file for fdmcalc.c (5 min)
- Created header file for constants (5 min)
- Created header file for memory.c (4 min)

- Wrote update_cell method to calculate the new temperature of a specific cell after a time step(7 min)
- Wrote update_row method to calculate the new temperatures for a row after a time step (10 min)
- Added descriptions for args and returns of update_cell and update_row methods (10 min)


### 2025-01-07 - update_row, update_layer
- Adjusted update_row method for 3D calculations (12 min)
- Updated h file for changes in update_row and update_layer (7 min)

- Wrote update_layer method (20 min)
- Edited fdmcalc.h so update_row and update_layer directly change newrow and newlayer, respectively and accounted for mode (5 min) 


### 2025-01-08 - fdmcalc adjustments for 2D to 1D arrays
- Edited update_layer so that it takes in 1D arrays now instead of 2D arrays since we intend on storing 1D arrays in the shared memory (15 min) 
- Adjusted fdmcalc.h for updates above (5 min)
- Debugged pointer errors throughout fdmcalc.c methods (10 min)