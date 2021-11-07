#pragma once
#include "Ray.h"
#include "Material.h"

class Geometry
{
public:
    Geometry(std::shared_ptr<Material> material) : material{ material } {};
    Geometry() = default;
    virtual ~Geometry() = default;
    virtual bool Hit(const ray_t& r, float tMin, float tMax, raycastHit_t& hit) = 0;

public:
    std::shared_ptr<Material> material;
};