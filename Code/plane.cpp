// Author: Remco van Buijtenen & Jurgen Nijland
//
// Implementation of plane-ray intersection using the Möller–Trumbore intersection algorithm
// 

#include <stdio.h>
#include <iostream>
#include <math.h>
#include "triple.h"
#include "material.h"

#include "plane.h"

/************************** Plane **********************************/

Hit Plane::intersect(const Ray &ray)
{
    double v, w, t;

    // compute the distance from the viewpoint to the point that we know lies on the plane
    v = (position-ray.O).dot(normal);
    // project the direction vector onto normal
    w = ray.D.dot(normal);
    
    if(w == 0) {
        // ray is parallel to plane, so they never intersect
        return Hit::NO_HIT();
    }

    t = v/w;
    if(t < 0) {
        // intersection with plane is behind eye
        return Hit::NO_HIT();
    }

    // return the time of intersection and the normal used to define the plane 
    return Hit(t,normal);
}

Color Plane::getTextureColor(const Vector N)
{
    return material->color;
}
