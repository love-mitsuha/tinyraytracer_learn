#ifndef __MODEL_H__
#define __MODEL_H__
#include <vector>
#include <string>
#include "geometry.h"
#include "Material.h"

class Model {
private:
    std::vector<Vec3f> verts;
    std::vector<Vec3i> faces;
    Vec3f min, max;
    Material material;

public:
    Model(const char *filename,const Material& material);

    int nverts() const;                          // number of vertices
    int nfaces() const;                          // number of triangles

    bool ray_triangle_intersect(const int &fi, const Vec3f &orig, const Vec3f &dir, float &tnear);
    bool bbox_intersect(const Vec3f& orig, const Vec3f& dir);
    friend void caculate_models(std::vector<std::unique_ptr<Model>>& models, float& min_distance, const Vec3f& origin,
        const Vec3f& direction, Vec3f& hit, Vec3f& normal, Material& material);

    Vec3f get_face_normal(int i);
    const Vec3f &point(int i) const;                   // coordinates of the vertex i
    Vec3f &point(int i);                   // coordinates of the vertex i
    int vert(int fi, int li) const;              // index of the vertex for the triangle fi and local index li
    void get_bbox(Vec3f &min, Vec3f &max); // bounding box for all the vertices, including isolated ones
};

std::ostream& operator<<(std::ostream& out, Model &m);

#endif //__MODEL_H__

