// Author: Remco van Buijtenen
//
// Implementation of traingle-ray intersection using the Möller–Trumbore intersection algorithm
// 

#include "triangle.h"
#include "plane.h"
#include <stdio.h>
#include <iostream>
#include <math.h>

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
    Vector u, v, n, hit, e1, e2, e3, ph1, ph2, ph3, C;
    double nd, d, t;

    // compute two edges of the triangle
    u = p2 - p1;
    v = p3 - p1;

    // compute normal
    n = u.cross(v);

    // project N on the view direction(negative ray direction)
    nd = n.dot(-ray.D);

    // if the value is near 0, then the line is parallel to the plane of the triangle so they wont intersect
    if(fabs(nd) < 0.000001) {
        return Hit::NO_HIT();
    }

    // distance to one corner of the triangle
    d = n.dot(p1);

    // t = normal projected on the origin + d / nd
    t = (n.dot(ray.O) + d)/nd;
    
    // intersection is behind the eye
    if(t < 0) {
        return Hit::NO_HIT();
    }

    // compute the point of intersection with the plane the triangle lies in
    hit = ray.at(t);
    
    e1 = p2 - p1;
    ph1 = hit - p1;
    C = e1.cross(ph1);

    // ray on the outer side of e1
    if (n.dot(C) < 0) {
        return Hit::NO_HIT();
    }

    e2 = p3 - p2;

    ph2 = hit - p2;
    C = e2.cross(ph2);


    // ray on the outer side of e2
    if (n.dot(C) < 0) {
        return Hit::NO_HIT();
    }

    e3 = p1 - p3;
    ph3 = hit - p3;
    C = e3.cross(ph3);

    // ray on the outer side of e1
    if (n.dot(C) < 0) {
        return Hit::NO_HIT();
    }

    // ray is on the inner side of all 3 edges of the triangle so we have a hit
    return Hit(t, -n.normalized());
}
