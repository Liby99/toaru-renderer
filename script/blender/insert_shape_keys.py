import bpy
import json
from mathutils import Vector

file_base = "/Users/liby99/Documents/Chronicle/Senior/Curriculum/CSE 291 Physics Simulation/output/hand_made_0/frame"
file_amount = 5

def add_keyframes(path, amount):

    # Setting up all the basic data
    obj = bpy.context.active_object
    mesh = obj.data

    # First load the vertices
    data = load_frame(mesh, path, 0)
    load_vertices(mesh, data)

    bpy.context.scene.frame_set(0)
    obj.shape_key_add(from_mix=False)

    # Loop through the frames
    for i in range(1, amount):
        bpy.context.scene.frame_set(i)
        key = obj.shape_key_add(from_mix=False)

        data = load_frame(mesh, path, i)
        update_vertices(mesh, data)

        # setup keyframes before and after each frame
        key.keyframe_insert("value", frame=i - 1)
        key.value = 1
        key.keyframe_insert("value", frame=i)
        key.value = 0
        key.keyframe_insert("value", frame=i + 1)

        bpy.ops.object.mode_set(mode='EDIT')
        bpy.ops.object.mode_set(mode='OBJECT')

def load_frame(mesh, path, i):
    filename = "%s_%d.json" % (path, i)
    with open(filename) as f:
        # data should be a List[Tuple[float, float, float]]
        return [(p[0], p[1], p[2]) for p in json.load(f)]

def load_vertices(mesh, verts):
    mesh.from_pydata(verts, [], [])
    mesh.update()

def update_vertices(mesh, verts):
    for i in range(len(mesh.vertices)):
        mesh.vertices[i].co = verts[i]

add_keyframes(file_base, file_amount)