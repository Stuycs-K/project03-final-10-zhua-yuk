import open3d as o3d
import numpy as np

# Load the STL file
mesh = o3d.io.read_triangle_mesh("vaca.stl")

# Ensure the mesh has vertex normals (required for some Open3D operations)
mesh.compute_vertex_normals()

# Define the voxel size
voxel_size = 2 # Adjust this based on your desired resolution

# Convert the mesh to a voxel grid
voxel_grid = o3d.geometry.VoxelGrid.create_from_triangle_mesh(mesh, voxel_size=voxel_size)

# Extract the voxel grid to a NumPy array
# Each voxel's center is stored as a point in the array
voxels = np.array([voxel.grid_index for voxel in voxel_grid.get_voxels()])

# Print or process the voxel array
print("Voxel Grid Array Shape:", voxels.shape)
print(voxels)
o3d.visualization.draw_geometries([voxel_grid])