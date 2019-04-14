# Scene JSON Format

## File Root

The root of the file should be an JSON Object, containing two fields:

- `"name"`: String, The registered name of the scene
- `"root"`: Node, The root object of the scene

## Object Node

Any node of the file should be an JSON Object. The only required field is `type`, which type is `string` and could have the following values:

- `"empty"`: Saying that this node contains nothing and is just an empty node
- `"assimp_object"`: Saying that this node is an assimp object. If this is the case, you must also specify `file` in this node.
- `"sphere"`: A primitive sphere
- `"plane"`: A primitive plane
- `"cube"`: A primitive cube

You can also specify the optional basic transform properties of an object:

- `"position"`: `[<x>, <y>, <z>]`, this should be an array containing three coordinates. If not specified, will be `[0, 0, 0]` by default.
- `"rotation"`: `[<x-rot>, <y-rot>, <z-rot>]`, this is an array containing three number specifying rotation around each axis. Default to be `[0, 0, 0]`.
- `"scale"`: `[<x-scale>, <y-scale>, <z-scale>]`: This is a scale vector containing scale on each axis

Additionally, since this is a scene graph, you also need the children field if you want something to be a child of the current node:

- `"children"`: `[<node>]`. The `children` should be a list of nodes.

Since you will definitely have a single `root` in a scene, a usual approach is to give `root` an `"type": "empty"` and attach multiple other objects inside the `"children"` field of the `root`.