import numpy as np

GSIZE = [-1, -1, -1]
DATA = None
MATDATA = None
TI = 0.0
TF = 5.0
DT = 0.001
UNITS = 0.1
MATERIALS = {
    "v" : 0.0
    }

def parse(path):
    with open(path, "r") as file:
        data = file.read().replace("\n", "").split(";")
        for d in data:
            p = d.split(" ")
            match p[0]:
                case "SIZE":
                    if GSIZE[0] != -1:
                        GSIZE = [int(p[1]), int(p[2]), int(p[3])]
                        DATA = np.zeros((GSIZE[0], GSIZE[1], GSIZE[2]))
                        MATDATA = np.zeros((GSIZE[0], GSIZE[1], GSIZE[2]))
                    else:
                        exit()
                case "MAT":
                    if p[0] not in MATERIALS:
                        MATERIALS[p[0]] = float(p[1])
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
                
                case "SPHERE":
                    pass
                case "HSPHERE":
                    pass
                case "HBOX":
                    pass
                case "BOX":
                    pass
                case "MESH":
                    pass
                case _:
                    print("none")

        




parse("../test.sim")
print(UNITS)
print(DT)
print(SIZE)