import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
import math
import os
import sys

def visualize_voxels(data, matdata, units, gsize, time, elev, azim, output_file, maxT, minT):
    fig = plt.figure(figsize=(6, 6))
    ax = fig.add_subplot(111, projection='3d')
    plt.get_current_fig_manager().canvas.setWindowTitle("preview")

    active_voxels = np.where(matdata != 0)
    temperatures = data[active_voxels]

    norm = plt.Normalize(vmin=minT, vmax=maxT)  # Explicitly set vmin and vmax
    cmap = cm.plasma

    ax.scatter(active_voxels[0] * units, active_voxels[1] * units, active_voxels[2] * units,
               c=temperatures, cmap=cmap, marker='s', s=100, norm=norm)

    ax.set_xlabel('X (m)')
    ax.set_ylabel('Y (m)')
    ax.set_zlabel('Z (m)')
    ax.set_title(f"T={time}s")

    ax.set_xlim(0, gsize[0] * units)
    ax.set_ylim(0, gsize[1] * units)
    ax.set_zlim(0, gsize[2] * units)

    cbar = plt.colorbar(cm.ScalarMappable(norm=norm, cmap=cmap), ax=ax)
    cbar.set_label('Temperature (C)')

    ax.view_init(elev=elev, azim=azim)

    plt.savefig(output_file, dpi=300)
    plt.close()

def read_file(path, outdir):
    if not os.path.exists(outdir):
        os.mkdir(outdir)
    minT = 0
    maxT = 0
    elev = 0
    azim = 0
    DATA = None
    MATDATA = None
    SIZE = [0, 0, 0]
    TI = float()
    TF = float()
    DT = float()
    NT = int()
    UNITS = float()
    MATERIALS = list()

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
            MATERIALS[int(m[0])] = [float(m[1]), m[2], False]
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
        
        elev = float(input("Enter camera elevation: "))
        azim = float(input("Enter camera azimuth: "))

        NT = math.ceil(TF/DT)
        print(f"Reading {NT} time frames...")
        #allocate data array (cursed quadruple nested loop)
        DATA = np.zeros((SIZE[0], SIZE[1], SIZE[2]))
        #read in data
        for t in range(NT):
            print(f"Reading in frame {t+1} of {NT}")
            #read in by layer
            for k in range(SIZE[2]):
                #read in layers by rows
                for j in range(SIZE[1]):
                    index = 1 + n_mats + SIZE[1]*SIZE[2] + t*SIZE[2]*SIZE[1] + k*SIZE[2] + j
                    d = data[index].split(",")
                    for i in range(SIZE[0]):
                        DATA[i][j][k] = float(d[i])
            if (t == 0):
                print(t)
                minT = np.min(DATA)
                maxT = np.max(DATA)

            visualize_voxels(DATA, MATDATA, UNITS, SIZE, t*DT, elev, azim, f"{outdir}/{t}.png", maxT, minT)
            

        print("Sucessfully read in data!")
        file.close()


if __name__ == "__main__":
    read_file(sys.argv[1], sys.argv[2])
        