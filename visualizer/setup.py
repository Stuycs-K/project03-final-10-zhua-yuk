import numpy as np
import open3d as o3d


def parse(path):
    GSIZE = [-1, -1, -1]
    DATA = None
    MATDATA = None
    TI = 0.0
    TF = 5.0
    DT = 0.001
    UNITS = -1
    MATCOUNT = 1
    MATERIALS = {
        "v" : [0.0, 0]
        }
    with open(path, "r") as file:
        data = file.read().replace("\n", "").split(";")
        for d in data:
            p = d.split(" ")
            match p[0]:
                case "SIZE":
                #SIZE x y z (m)
                    if GSIZE[0] == -1 and UNITS != -1:
                        GSIZE = [int(float(p[1])/UNITS), int(float(p[2])/UNITS), int(float(p[3])/UNITS)]
                        DATA = np.zeros((GSIZE[0], GSIZE[1], GSIZE[2]))
                        MATDATA = np.zeros((GSIZE[0], GSIZE[1], GSIZE[2]))
                    else:
                        exit("Already defined grid, or units not defined!")
                case "MAT":
                #MAT name thermal-diff-coeff
                    if p[1] not in MATERIALS:
                        MATERIALS[p[1]] = [float(p[2]), MATCOUNT]
                        MATCOUNT += 1
                    else:
                        exit()
                case "TI":
                #TI [s]
                    TI = float(p[1])
                case "TF":
                #TF [s]
                    TF = float(p[1])
                case "DT":
                #DT [s]
                    DT = float(p[1])
                case "UNITS":
                #UNITS [m per cell len]
                    UNITS = float(p[1])
                case "SETTEMP":
                #SETTEMP temp x0 y0 z0 x1 y1 z1
                    if UNITS != -1 and GSIZE[0] != -1:
                        x0 = int(float(p[2])/UNITS)
                        y0 = int(float(p[3])/UNITS)
                        z0 = int(float(p[4])/UNITS)
                        x1 = int(float(p[5])/UNITS)
                        y1 = int(float(p[6])/UNITS)
                        z1 = int(float(p[7])/UNITS)
                        temp = float(p[1])
                        for i in range(x0, x1+1):
                            for j in range(y0, y1+1):
                                for k in range(z0, z1+1):
                                    try:
                                        DATA[i][j][k] = temp
                                    except:
                                        pass
                case "BOX":
                #BOX material temp x0 y0 z0 x1 y1 z1 
                    if UNITS != -1 and GSIZE[0] != -1:
                        x0 = int(float(p[3])/UNITS)
                        y0 = int(float(p[4])/UNITS)
                        z0 = int(float(p[5])/UNITS)
                        x1 = int(float(p[6])/UNITS)
                        y1 = int(float(p[7])/UNITS)
                        z1 = int(float(p[8])/UNITS)
                        temp = float(p[2])
                        mat = p[1]
                        for i in range(x0, x1+1):
                            for j in range(y0, y1+1):
                                for k in range(z0, z1+1):
                                    try:
                                        DATA[i][j][k] = temp
                                        MATDATA[i][j][k] = MATERIALS[mat][1]
                                    except:
                                        pass

                case "SPHERE":
                    # SPHERE material temp cx cy cz radius
                    if UNITS != -1 and GSIZE[0] != -1:
                        cx = int(float(p[3]) / UNITS) 
                        cy = int(float(p[4]) / UNITS)
                        cz = int(float(p[5]) / UNITS)  
                        radius = int(float(p[6]) / UNITS)
                        temp = float(p[2])
                        mat = p[1]
                        for i in range(cx - radius, cx + radius + 1):
                            for j in range(cy - radius, cy + radius + 1):
                                for k in range(cz - radius, cz + radius + 1):
                                    if (i-cx)**2 + (j-cy)**2 + (k-cz)**2 <= radius**2:
                                        try:
                                            DATA[i][j][k] = temp
                                            MATDATA[i][j][k] = MATERIALS[mat][1]
                                        except:
                                            pass

                case "MESH":
                #MESH file.stl x-len material init_temp x y z (bottom corner)
                    if UNITS != -1 and GSIZE[0] != -1:
                        #load mesh
                        meshpath = p[1]
                        mesh = o3d.io.read_triangle_mesh(meshpath)
                        #get size
                        bbox = mesh.get_axis_aligned_bounding_box()
                        bbox_extent = bbox.get_extent()
                        #scale and generate voxels
                        mesh.compute_vertex_normals()
                        mesh.scale(float(p[2])/bbox_extent[0], center=mesh.get_center())
                        voxel_grid = o3d.geometry.VoxelGrid.create_from_triangle_mesh(mesh, voxel_size=UNITS)
                        voxels = np.array([voxel.grid_index for voxel in voxel_grid.get_voxels()])

                        x_i = float(p[5])
                        y_i = float(p[6])
                        z_i = float(p[7])

                        #write to grid
                        for v in voxels:
                            ix = int(x_i/UNITS + v[0])
                            iy = int(y_i/UNITS + v[1])
                            iz = int(z_i/UNITS + v[2])
                            try:
                                MATDATA[ix][iy][iz] = MATERIALS[p[3]][1]
                                DATA[ix][iy][iz] = float(p[4])
                            except:
                                pass
                    else:
                        exit("Units and size not set!")
                case _:
                    pass
        file.close()

    outpath = path.split(".")[0] + ".csv"
    print(f"R Value: {DT/(UNITS**2)}")
    print(MATERIALS)
    
    with open(outpath, "w") as file:
        outdata = f"{GSIZE[0]},{GSIZE[1]},{GSIZE[2]},{TI},{TF},{DT},{UNITS},{len(MATERIALS.keys())}\n"
        for mat in MATERIALS.keys():
            outdata += f"{MATERIALS[mat][1]},{MATERIALS[mat][0]},{mat}\n"
        #print materials
        for k in range(GSIZE[2]):
            for j in range(GSIZE[1]):
                for i in range(GSIZE[0]):
                    outdata += f"{int(MATDATA[i][j][k])}"
                    if (i < GSIZE[0]-1):
                        outdata += ","
                outdata += "\n"
        #print initial temps
        for k in range(GSIZE[2]):
            for j in range(GSIZE[1]):
                for i in range(GSIZE[0]):
                    outdata += f"{float(DATA[i][j][k])}"
                    if (i < GSIZE[0]-1):
                        outdata += ","
                outdata += "\n"
        file.write(outdata)
        file.close()
            
    

    

parse("test.sim")
