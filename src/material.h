#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material {
    public:
        virtual ~material() = default;

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
            return false;
        }
};

class lambertian : public material {
    public:
        lambertian(const color& albedo) : albedo(albedo) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 scatter_direction = rec.normal + random_unit_vector();

            // Catch degenerate scatter direction e.g. scatter is opposite normal
            if (scatter_direction.near_zero()) { scatter_direction = rec.normal; }

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    private:
        color albedo;
};

class metal : public material {
    public:
        metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(r_in.direction(), rec.normal);

            // Fuzz logic, scatters ray in random direction scaled by fuzz
            reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
            scattered = ray(rec.p, reflected);
            attenuation = albedo;

            // Discards ray if scattered ray is below surface (scatter (dot) normal < 0)
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    private:
        color albedo;
        double fuzz;
};

class dielectric : public material { 
    public:
        dielectric(double refraction_index) : refraction_index(refraction_index) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            attenuation = color(1.0, 1.0, 1.0);

            // If the ray is coming through air (RI 1.0) then we must convert the ray's direction from
            // the context of air to the material. If the ray is exiting, it uses the material's RI
            double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

            vec3 unit_direction = unit_vector(r_in.direction());
            vec3 refracted = refract(unit_direction, rec.normal, ri);

            scattered = ray(rec.p, refracted);
            return true;
        }

    private:
        // Refractive index in vacuum/air or the ratio of the material's refractive index
        // over the refractive index of the enclosing material
        double refraction_index;
};

#endif