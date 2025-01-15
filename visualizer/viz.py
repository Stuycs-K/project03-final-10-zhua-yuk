import matplotlib.pyplot as plt
import numpy as np
import math
import fileinput

#0 - i, 1 - j, 2 - k

DATA = None
MATDATA = None
SIZE = [0, 0, 0]
TI = float()
TF = float()
DT = float()
NT = int()
UNITS = float()
MATERIALS = list()


def read_file(path):
    with open(path, "r") as file:
        print("Reading in file...")
        data = file.read().split("\n")
        grid_dimens = data[0].split(",")
        #set grid dimensions
        SIZE[0] = int(grid_dimens[0])
        SIZE[1] = int(grid_dimens[1])
        SIZE[2] = int(grid_dimens[2])
        print(f"[Size]: ({SIZE[0]}, {SIZE[1]}, {SIZE[2]})")
        #read in time interval
        TI = float(grid_dimens[3])
        TF = float(grid_dimens[4])
        DT = float(grid_dimens[5])
        print(f"[Time interval]: {TI}s to {TF}s. [DT]: {DT}s")
        # read in units
        UNITS = float(grid_dimens[6])
        r = DT/pow(UNITS, 2)
        print(f"[Units per cell]: {UNITS}m^3")
        print(f"[r value]: {r}")
        #read in materials
        n_mats = int(grid_dimens[7])
        print(f"[Number of materials]: {n_mats}")
        MATERIALS = [0] * n_mats
        for i in range(n_mats):
            m = data[1+i].split(",")
            MATERIALS[int(m[0])] = [float(m[1]), m[2]]
            print(f"\t[{m[0]}]: {m[2]} : {m[1]}")

        #allocate MAT array
        MATDATA = np.zeros((SIZE[0], SIZE[1], SIZE[2]), dtype="int")
        #read in materials
        for k in range(SIZE[2]):
            for j in range(SIZE[1]):
                index = 1 + n_mats + k*SIZE[1] + j
                d = data[index].split(",")
                for i in range(SIZE[0]):
                    MATDATA[i][j][k] = int(d[i])


        NT = math.ceil(TF/DT)
        print(f"Reading {NT} time frames...")
        #allocate data array (cursed quadruple nested loop)
        DATA = np.zeros((NT, SIZE[0], SIZE[1], SIZE[2]))
        #read in data
        for t in range(NT):
            #read in by layer
            for k in range(SIZE[2]):
                #read in layers by rows
                for j in range(SIZE[1]):
                    index = 1 + n_mats + SIZE[1]*SIZE[2] + t*SIZE[2]*SIZE[1] + k*SIZE[1] + j
                    d = data[index].split(",")
                    for i in range(SIZE[0]):
                        DATA[t][i][j][k] = float(d[i])

        print("Sucessfully read in data!")
        file.close()


if __name__ == "__main__":
    read_file("../out.csv")