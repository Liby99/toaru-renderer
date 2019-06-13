import bpy
from random import random

def create_vertices(name, verts):
    # Create mesh and object
    me = bpy.data.meshes.new(name + 'Mesh')
    ob = bpy.data.objects.new(name, me)
    ob.show_name = True
    # Link object to scene
    bpy.context.scene.objects.link(ob)
    me.from_pydata(verts, [], [])
    # Update mesh with new data
    me.update()
    return ob

particles = []
for _ in range(100000):
    particles.append((random(), random(), random()))

create_vertices("part", particles)