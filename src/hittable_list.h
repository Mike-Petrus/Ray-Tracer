#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "common.h"
#include "hittable.h"
#include <vector>

// the hittable list is itself something the ray can intersect
// it is a composite object

class hittable_list : public hittable {
    public:
        // Stores a list of hittable objects
        // Each object is a shared_ptr to a hittable which lets us store diff types of objects (tr, sphere, etc.)
        // We can safely shre ownership off all these objects

        std::vector<shared_ptr<hittable>> objects; 

        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) {
            objects.push_back(object);
        }

        // For a given ray, find if it hits any object in our list
        // If it it hits multiple, return the closest one
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            hit_record temp_rec;
            bool hit_anything = false;
            double closest_so_far = ray_t.max;

            // For each object, call hit() and test ray
            for (const auto& object :objects) {

                if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {

                    // If we get a hit closer than record, update closest dist, update hit record
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }
            
            return hit_anything;
        }
};

#endif