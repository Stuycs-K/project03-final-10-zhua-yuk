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

### 2025-01-08 - completed shared_memory files and update makefile
- added ignores for the makefile (15 min)
- completed functions in shared_memory (changed the name of memory) (60 min)

### 2025-01-09
- Creating test input files (10 mins)
- tweaking shared memory functions (20 mins)
- debugging shared memory functions (40 mins)

### 2025-01-10
- Completed testing shared memory functions (45 mins)

### 2025-01-11
- Created remove semaphore methods and debugged remove_shared_mem methods (20 mins)

### 2025-01-11
- Created remove semaphore methods and debugged remove_shared_mem methods (20 mins)
- 

### 2024-01-12
- Researched finite difference method further (30 mins) (r < 1/2)
- Started planning out child and parent communication protocols (20 mins)

### 2025-01-13
- Debugged getindex() method (10 mins)
- debugged fdmcalc functions, rewrote in a cleaner manner (30 mins)

### 2025-01-14
- merged new signal handling together with subprocess (20 mins)
- set constants to be as global variables (20 mins)
- started working with python program for rendering (40 mins)

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

- Tested all fdmcalc methods and debugged errors with all zero rows in update_layer method (1 hour)

### 2025-01-09 - subprocess.h edits, wrote spawn_subprocess
- Specified mode in subprocess.h (3 min)
- Forking, Downed Semaphore, Accessed Shared Memory (25 min)

- Wrote calls to update_layer in spawn_subprocess, attached shared memory to variable (50 min)

### 2025-01-10 - subprocess.c debugging, updates for zero indexing
- Addressed pointer errors when reading and writing to shared memory (15 min)
- Edits for starting i, j, k at 0 in spawn_subprocess and update_layer methods (5 min)

### 2025-01-11 - testing subprocess.c (incomplete)
- Created main file to test subprocess.c (15 min)
- Adding print statements and config.h variables to spawn_subprocess while debugging (40 min)

### 2025-01-13 - testing, sighandler
- Tested spawn_subprocesses, made main to test (35 min)

- Divided spawn_subprocess into spawn_subprocess (for forking and attaching signals) and calculate_once, started sighandler (25 min)
- Updated subprocess.h for changes (5 min)
