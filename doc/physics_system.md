# Physics System

![Cube](https://cubit.sandia.gov/public/13.2/help_manual/WebHelp/mesh_generation/meshing_schemes/traditional/images/tetmesh_respect1.gif)

``` c++
class PhysicsSystem {
  std::vector<PhysicsObject *> objects;
  std::vector<Point *> points;
  std::vector<Tetrahedron *> tetras;

  int addPoint(Vector3f pos) {
    points.push_back(new Point(pos));
    return points.size() - 1;
  }

  int addTetrahedron(int obj, int p1, int p2, int p3, int p4) {
    PhysicsObject *objp = objects[obj];
    Point *p1p = points[p1];
    Point *p2p = points[p2];
    Point *p3p = points[p3];
    Point *p4p = points[p4];
    tetra = new Tetrahedron(objp->KMat, objp->DMat, obj->density, p1p, p2p, p3p, p4p);
    tetras.push_back(tetra);

    Face *f1 = getFace(tetra, p1, p2, p3, opposite);
    Face *f2 = getFace(p2, p3, p4);
    Face *f3 = getFace(p3, p4, p1);
    Face *f4 = getFace();

    return tetras.size() - 1;
  }

  Matrix3f stress; //3x3

  Vector3f faceNormal; // isInternal == false, trivial; otherwise, which side
  if (tetra.t1 == thisTetra) {
    face.normal;
  } else {
    -face.normal;
  }

  void getFace(tetra, p1, p2, p3, oppositePoint) {
    if (existed(face)) {
      face.t2 = tetra;
      face.isInternal = true;
      face.p2 = oppositePoint;
    } else {
      faces.push_back(new Face({p1, p2, p3}, oppositePoint)); // isInternal = false
      face.t1 = tetra;
    }
  }

  int createCube(PhysMat mat, Vector3f center, Vector3f extent) {
    return createRectangle(obj, center, extent, Vector3f(1, 1, 1));
  }

  int createRectangle(PhysMat kmat, Vector3f center, Vector3f extent, Vector3u subdivision) {
    std::vector<std::vector<std::vector<int>>> points;
    for (0..n-1) {
      for () {
        for () {
          points.push_back();
        }
      }
    } 

    for (i = 0:n-2:2) {
      for (j = 0:m - 2:2) {
        for (k = ) {
          p1 = points[i][j][k]
          p2 = points[i + 1][j][k]
          p3
          p4
          ..
          p16

          pcenter = addPoint();
          addTetra(p1, p2, p3, p16);

        }
      }
    }
  }

  int createSphere(Vector3f center, float radius) {

  }
};
```

``` c++
// Test File
class MyPhysicsSystem : public PhysicsSystem {
  std::vector<std::unique_ptr<Obj>> objs;

  virtual void init() {
    std::shared_ptr<PhysicsMaterial> mat;
    mat.set();
    int obj0 = createRectange(mat, ...);
    int obj1 = createSphere(mat, ...);

    addVelocityToObject(obj1, Vector3f(1, 1, 1));
    fixObject(obj2);
  }
}
```