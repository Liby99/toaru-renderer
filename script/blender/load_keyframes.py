import bpy
import json
from mathutils import Vector

file_base = "/Users/liby99/Documents/Chronicle/Senior/Curriculum/CSE 291 Physics Simulation/output/frame_test_0/frame"
file_amount = 5

def add_keyframes(path, amount):

    # Setting up all the basic data
    obj = bpy.context.active_object
    mesh = obj.data

    # Create animation
    action = bpy.data.actions.new("animation")
    mesh.animation_data_create()
    mesh.animation_data.action = action

    # First load the vertices
    data = load_frame(mesh, path, 0)
    load_vertices(mesh, data)
    insert_keyframe(mesh, 0, data)

    # Loop through the frames
    for i in range(1, amount):
        data = load_frame(mesh, path, i)
        insert_keyframe(mesh, i, data)

def load_frame(mesh, path, i):
    filename = "%s_%d.json" % (path, i)
    with open(filename) as f:
        # data should be a List[Tuple[float, float, float]]
        return [(p[0], p[1], p[2]) for p in json.load(f)]

def load_vertices(mesh, verts):

    # Copy the vertex position
    mesh.from_pydata(verts, [], [])
    mesh.update()

    # setup the keyframe track
    data_path = "vertices[%d].co"
    action = mesh.animation_data.action
    for v in mesh.vertices:
        for index in range(3):
            action.fcurves.new(data_path % v.index, index=index)

def insert_keyframe(mesh, i, data):
    data_path = "vertices[%d].co"
    action = mesh.animation_data.action
    for v in mesh.vertices:
        for index in range(3):
            fcu = action.fcurves.find(data_path % v.index, index=index)
            fcu.keyframe_points.insert(i, data[v.index][index], options={'FAST'})

add_keyframes(file_base, file_amount)