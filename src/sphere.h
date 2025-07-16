#ifndef SPHERE_H
#define SPHERE_H

#include "common.h"
#include "hittable.h"

class sphere : public hittable {
    public:

        // Stationary Sphere
        sphere (const point3& center, double radius, shared_ptr<material> mat) : center(center), radius(std::fmax(0, radius)), mat(mat) {
            vec3 rvec = vec3(radius, radius, radius);
            bbox = aabb(center - rvec, center + rvec);
        }

        // Moving Sphere find motion blur in Ch.2 and sphere constructor in 3.5

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            vec3 oc = center - r.origin();
            double a = r.direction().length_squared();
            double h = dot(r.direction(), oc);
            double c = oc.length_squared() - radius*radius;
            double discriminant = h*h - a*c;

            if (discriminant < 0) { return false; }

            double sqrtd = std::sqrt(discriminant);

            // Find the nearest root that lies in the acceptable range
            double root = (h - sqrtd) / a;

            if (!ray_t.surrounds(root)) {
                
                root = (h + sqrtd) / a;
                if (!ray_t.surrounds(root)) { return false; }
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;    // get the normal vector
            rec.set_face_normal(r, outward_normal);             // then use call to set normal direction
            rec.mat = mat;

            return true;
        }

        aabb bounding_box() const override { return bbox; }

    private:
        point3 center;
        double radius;
        shared_ptr<material> mat;
        aabb bbox;
};

#endif