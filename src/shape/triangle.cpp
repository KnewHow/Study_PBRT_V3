#include "triangle.h"

namespace pbrt {

TriangleMesh::TriangleMesh(int nTriangles, int nVertices,
                           const std::vector<int> &vIndices, 
                           const std::vector<Point3f> &ps, 
                           const std::vector<Normal3f> &ns) 
                         : nTriangles(nTriangles), nVertices(nVertices), vertexIndices(vIndices) {
    p.reset(new Point3f[nVertices]);
    for(int i = 0; i < nVertices; ++i) p[i] = ps[i];
    n.reset(new Normal3f[nVertices]);
    for(int i = 0; i < nVertices; ++i) n[i] = ns[i];
}

bool Triangle::Intersection(const Ray &ray, Float &tHit, SurfaceInteraction &isect) const {
   const Point3f &p0 = mesh->p[v[0]];
   const Point3f &p1 = mesh->p[v[1]];
   const Point3f &p2 = mesh->p[v[2]];
   const Vector3f e1 = p1 - p0;
   const Vector3f e2 = p2 - p0;
   Vector3f S = ray.o - p0;
   Vector3f S1 = Cross(ray.d, e2);
   Vector3f S2 = Cross(S, e1);
   Vector3f v = Vector3f(Dot(S2, e2), Dot(S1, S), Dot(S2, ray.d));
   Vector3f r = 1.0f / Dot(S1, e1) * v;
   if(r.x < ray.tMax && r.x > 0 && r.y > 0 && r.z > 0 && (1 - r.y - r.z) > 0) {
      float t = r.x;
      Point3f hitPoint = ray.o + t * ray.d;
      tHit = t;
      Normal3f n = Normal3f(Normalize(Cross(e1, e2)));
      isect = SurfaceInteraction(this, hitPoint, n);
      isect.shape = this;
      return true;
   }
   return false;
}


bool Triangle::IntersectionP(const Ray &ray) const {
   const Point3f &p0 = mesh->p[v[0]];
   const Point3f &p1 = mesh->p[v[1]];
   const Point3f &p2 = mesh->p[v[2]];
   const Vector3f e1 = p1 - p0;
   const Vector3f e2 = p2 - p0;
   Vector3f S = ray.o - p0;
   Vector3f S1 = Cross(ray.d, e2);
   Vector3f S2 = Cross(S, e1);
   Vector3f v = Vector3f(Dot(S2, e2), Dot(S1, S), Dot(S2, ray.d));
   Vector3f r = 1.0f / Dot(S1, e1) * v;
   if(r.x < ray.tMax && r.x > 0 && r.y > 0 && r.z > 0 && (1 - r.y - r.z) > 0) return true;
   else return false;
}

Float Triangle::Area() const {
   const Point3f &p0 = mesh->p[v[0]];
   const Point3f &p1 = mesh->p[v[1]];
   const Point3f &p2 = mesh->p[v[2]];
   return 0.5 * Cross(p2 - p0, p1 - p0).Length();
}

Bounds3f Triangle::WorldBound() const {
   const Point3f &p0 = mesh->p[v[0]];
   const Point3f &p1 = mesh->p[v[1]];
   const Point3f &p2 = mesh->p[v[2]];
   return Union(Bounds3f(p0, p1), p2);
}

Interaction Triangle::Sample(Float& pdf) const {
   Float x = std::sqrt(get_random_Float()), y = get_random_Float();
   const Point3f &p0 = mesh->p[v[0]];
   const Point3f &p1 = mesh->p[v[1]];
   const Point3f &p2 = mesh->p[v[2]];
   const Vector3f e1 = p1 - p0;
   const Vector3f e2 = p2 - p0;
   Normal3f n = Normal3f(Normalize(Cross(e1, e2)));
   // random uniform triangle, reference: https://stackoverflow.com/questions/19654251/random-point-inside-triangle-inside-java
   Point3f p =  p0 * (1.0f - x) + p1 * (x * (1.0f - y)) + p2 * (x * y);
   pdf = 1 / Area();
   return Interaction(p, n);
}

} // namespace pbrt