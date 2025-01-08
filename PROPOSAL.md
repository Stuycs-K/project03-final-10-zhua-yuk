# Final Project Proposal

## Group Members:

Ashley Zhu, Kellen Yu
       
# Intentions:

A parallel iterative FDM solver to the heat equation.

![Alt text](https://raw.githubusercontent.com/Stuycs-K/project03-final-10-zhua-yuk/refs/heads/main/images/heatequation.png)
    
# Intended usage:

The user will input data into a `.csv` file that contains data about the grid dimensions, time step settings, units, heat coefficients of each cell and the initial conditions of each cell. 

The top row of the csv file will contain the following:
```
size in x, size in y, size in z, start of time interval, end of time interval, time step, units per cell, number of distinct materials.
```
The following rows will contain a list of materials and their corresponding id:
```
0, 0.1, vacuum
1, 0.2, air
2, 0.3, copper
.
.
.
```
Then the following rows will contain data to represent each layer of the grid. the x coordinate is the columns, the y coordinate is the rows, and the z coordinate is the "grouping" of rows and columns. 

each entry will contain the thermal diffusivity coefficient followed by the initial temperature and then the material id. 
Here is a 2x2x2 grid example:
```
0.1, 10, 1, 0.1, 10, 1
0.2, 0, 2, 0.2, 0, 2
0.2, 0, 2, 0.2, 0, 2
0.2, 0, 2, 0.2, 0, 2
```
Then they will run our program, supplying the input file as an argument:
```
$ solver file.csv
```
The program will then write the states of the space at each time interval into an output `.csv` file either specified by the user (via an optional argument) or a file that will be created by the program automatically. 

The user can set up the inital `.csv` file using a seperate program and also view the data with a visualizer program.

# Technical Details:
Using the Finite Difference Method, we can simulate how heat dissipates througn a space over a time interval. In order to achieve a high level of accuracy, the resolution of the grid must be very high and the individual time steps must be very small. This is a very computationally expensive process, as many calculations need to be made, one we hope to expedite through parallel processing.

We will use what we learned about manipulating and reading from files to read data of the initial conditions from a file and then write to an output file with the results. 

We need to split up the calculations needed across various processes, which involves process management functions such as forking. We also will have all of the child processes work with a block of shared memory, (so that we don't spend alot of time reading/writing/copying data, this is a huge time and memory saver for huge arrays (which we intend this project to be used for)) 

To find out when all calculations are completed and we are able to move onto the next time step, we will use semaphores, and each child will down the semaphore when it is spawned. When the semaphore value equals the amount of children spawned, then the calculations are complete. We don't need semaphores for writing to shared resources because each child will be assigned a seperate section of the array to work on. 

If possible, we plan to create a python visualizer that uses matplotlib or similar graphics libraries to visually represent the data. We also hope to create a tool that makes the process of setting up initial conditions easier, possibly letting 3D models to be taken in, or simpler geometries. 

# Distribution of work
## MVP
- Kellen: File I/O functions and handlers
- Ashley: Shared memory management + Semaphores
- Kellen: Sub-process creation and management functions
- Ashley: FDM Calculation functions

## Extra
- Kellen: Visualization tool
- Ashley: Initial setup tools



# Intended pacing:
- [x] 1/6 : Initial Proposal
- [ ] 1/8: FDM Calculation functions and File I/O functions complete
- [ ] 1/11: Shared memory management and Sub-process management complete
- [ ] 1/12: Testing examples completed
- [ ] 1/13: MVP completed
- [ ] 1/15: Visualization tool complete and set up tool complete
- [ ] 1/18: Visualization animation tool. 
- [ ] 1/20: Video completed 

