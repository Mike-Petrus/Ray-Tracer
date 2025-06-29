#ifndef SPHERE_H
#define SPHERE_H

#include "common.h"
#include "hittable.h"

class sphere : public hittable {
    public:
        sphere (const point3& center, double radius) : center(center), radius(std::fmax(0, radius)) {}

        bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
            vec3 oc = center - r.origin();
            double a = r.direction().length_squared();
            double h = dot(r.direction(), oc);
            double c = oc.length_squared() - radius*radius;
            double discriminant = h*h - a*c;

            if (discriminant < 0) { return false; }

            double sqrtd = std::sqrt(discriminant);

            // Find the nearest root that lies in the acceptable range
            double root = (h - sqrtd) / a;

            if (root <= ray_tmin || root >= ray_tmax) {
                
                root = (h + sqrtd) / a;
                if (root <= ray_tmin || root >= ray_tmax) { return false; }
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;    // get the normal vector
            rec.set_face_normal(r, outward_normal);             // then use call to set normal direction

            return true;
        }

    private:
        point3 center;
        double radius;
};

#endif