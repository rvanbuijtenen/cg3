// Author: Remco van Buijtenen
//
// Implementation of traingle-ray intersection using the Möller–Trumbore intersection algorithm
// 

#include "triangle.h"
#include "plane.h"
#include "material.h"
#include <stdio.h>
#include <iostream>
#include <math.h>

#define EPSILON 0.000001
/************************** Sphere **********************************/

Hit Triangle::intersect(const Ray &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, r
    * Sought: intersects? if true: *t
    * 
    * Insert calculation of ray/sphere intersection here. 
    *
    * You have the sphere's center (C) and radius (r) as well as
    * the ray's origin (ray.O) and direction (ray.D).
    *
    * If the ray does not intersect the sphere, return false.
    * Otherwise, return true and place the distance of the
    * intersection point from the ray origin in *t (see example).
    ****************************************************/

    Vector e1, e2;
    Vector P, Q, T;

    e1 = p2 - p1;
    e2 = p3 - p2;

    Vector N = e2.cross(e1).normalized();

    double v, w, t;

    // compute the distance from the viewpoint to the point that we know lies on the plane
    v = (p1-ray.O).dot(N);
    // project the direction vector onto normal
    w = ray.D.dot(N);
    
    if(w == 0) {
        // ray is parallel to plane, so they never intersect
        return Hit::NO_HIT();
    }

    t = v/w;
    if(t < 0) {
        // intersection with plane is behind eye
        return Hit::NO_HIT();
    }

    Point h = ray.at(t);

    Vector A1, B1, C1, A2, B2, C2;
    A1 = (p2 - p1);
    B1 = (p3 - p2);
    C1 = (p1 - p3);

    A2 = (p2 - h);
    B2 = (p3 - h);
    C2 = (p1 - h);

    Vector d1,d2,d3;

    d1 = A1.cross(A2);
    d2 = B1.cross(B2);
    d3 = C1.cross(C2);


    if(d1.dot(N) > 0 && d2.dot(N) > 0 && d3.dot(N) > 0) {
        return Hit(t, N);
    } else {
        return Hit::NO_HIT();
    }
}

Color Triangle::getTextureColor(const Vector N)
{
    return material->color;
}
