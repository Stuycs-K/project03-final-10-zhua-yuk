[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Vh67aNdh)
# Heat Equation Solver

## Zhu Cool For Yu [41]

Ashley Zhu and Kellen Yu
       
## Project Description:

A parallelized numerical solver for the heat equation. Space and time are discretized using the finite difference method (FDM), allowing us to solve the heat equation iteratively. Updating every cell in a 3D space can get quite expensive (computationally), so to expedite this we have opted into parallelizing the calculations needed. Different portions of the space are split among many processes, which all perform the necessary calculations. All of the source files ```.c
``` can be found in ```src/``` and the header files ```.h``` can be found in ```include/```.
###[Demo Video](https://drive.google.com/file/d/1ZxU0X1dKx90iceQIdppKsIjghlIx8yXa/view?usp=sharing)
## Instructions:
### To compile:
```$ make```

### To run:
```$ ./solver input.csv output.csv```

## Using the description language:
### python venv (python3 3.10 used)
#### set up the virtual environment
```$ python3 -m venv .env```
#### Install dependencies:
```$ pip3 install -r visualizer/requirements.txt```
#### Activate the environment
```$ source .env/bin/activate```

### Syntax
Statements are seperated by semicolons
```
SIZE x y z (m); 
//define the size of the space (must be first line of program)

MAT name thermal-coeff (m^2/s);
 //define a material

TI t (s); 
//define time_initial

TF t (s); 
//define time_final

DT t (s); 
//define time step

UNITS t (m); 
//define meters-per-cell

//The above paramters must be defined before anything else!

SETTEMP temp (C) x0 y0 z0 x1 y1 z1 (m); 
//sets a box defined by opposing corners (x0, y0, z0) and (x1, y1, z1) to a certain temperature

BOX material temp (C) x0 y0 z0 x1 y1 z1 (m); 
//creates a box of some material and some initial temperature

SPHERE material temp (C) cx cy cz radius (m); 
//Creates a sphere of some material and some initial temperature with center (cx, cy, cz) and radius (m)

MESH file.stl x-len material temp (C) x y z (m); 
//voxelizes a .stl file and places the lower-left-corner of it's bounding box at x y z. The file is the path from the cwd, the x-len is a scaling factor (sets how long the bounding box is along the x-axis).

```
Note that DT/(UNITS^2) < 0.5 MUST BE TRUE! try to minimize this value.

### Example program
```
UNITS 0.025;
TI 0.0;
TF 0.01;
DT 0.00001;
SIZE 1 1 1;
MAT m1 0.5;
SETTEMP 32 0 0 0 1 1 1;
BOX m1 32 0 0 0 1 1 0.1;
SETTEMP 1000 0.2 0.2 0 0.8 0.8 0.1;
```

### Converting description language to .csv input file
```$ ./setup input.sim output.csv```
This will display a preview of the environment in a numpy window (lets you get the proper elevation and azimuth angles)

### Visualizing data
```$ ./render data.csv dirname```
Numbered images will be written to the dirname.

## Media
![1](https://raw.githubusercontent.com/Stuycs-K/project03-final-10-zhua-yuk/refs/heads/main/images/platform.gif)
![2](https://raw.githubusercontent.com/Stuycs-K/project03-final-10-zhua-yuk/refs/heads/main/images/rabbit.gif)

