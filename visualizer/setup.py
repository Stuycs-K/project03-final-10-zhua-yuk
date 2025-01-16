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
                    if GSIZE[0] == -1 and UNITS != -1:
                        GSIZE = [int(float(p[1])/UNITS), int(float(p[2])/UNITS), int(float(p[3])/UNITS)]
                        DATA = np.zeros((GSIZE[0], GSIZE[1], GSIZE[2]))
                        MATDATA = np.zeros((GSIZE[0], GSIZE[1], GSIZE[2]))
                    else:
                        exit("Already defined grid, or units not defined!")
                case "MAT":
                    if p[1] not in MATERIALS:
                        MATERIALS[p[1]] = [float(p[2]), MATCOUNT]
                        MATCOUNT += 1
                    else:
                        exit()
                case "TI":
                    TI = float(p[1])
                case "TF":
                    TF = float(p[1])
                case "DT":
                    DT = float(p[1])
                case "UNITS":
                    UNITS = float(p[1])
                case "MESH":
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

                        print(MATERIALS)
                    else:
                        exit()


                    pass
                case _:
                    pass
        file.close()
    outpath = path.split(".")[0] + ".csv"
    with open(outpath, "w") as file:
        
    

        




parse("./test.sim")
