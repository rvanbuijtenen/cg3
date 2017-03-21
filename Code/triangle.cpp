// Author: Remco van Buijtenen & Jurgen Nijland
//
// Implementation of triangle-ray intersection using the Möller–Trumbore intersection algorithm
// 

#include "triangle.h"
#include "plane.h"
#include "material.h"
#include <stdio.h>
#include <iostream>
#include <math.h>

Hit Triangle::intersect(const Ray &ray)
{
    Vector e1, e2;
    Vector P, Q, T;
    double v, w, t;

    e1 = p2 - p1;
    e2 = p3 - p2;

    // normal of triangle is the cross product of two edges
    Vector N = e2.cross(e1).normalized();


    // compute the distance from the viewpoint to the point that we know lies on the plane of triangle
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

    // compute point of intersection
    Point h = ray.at(t);

    Vector A1, B1, C1, A2, B2, C2, A3, B3, C3;


    // for each edge: compute if the edge lies to left or right of this edge
    A1 = (p2 - p1);
    A2 = (p2 - h);
    A3 = A1.cross(A2);

    if(A3.dot(N) < 0) {
        return Hit::NO_HIT();
    }

    B1 = (p3 - p2);
    B2 = (p3 - h);
    B3 = B1.cross(B2);

    if(B3.dot(N) < 0) {
        return Hit::NO_HIT();
    } 

    C1 = (p1 - p3);
    C2 = (p1 - h);
    C3 = C1.cross(C2);

    if(C3.dot(N) < 0) {
        return Hit::NO_HIT();
    }

    // point lies within the triangle so we return a hit
    return Hit(t, N);
}

Color Triangle::getTextureColor(const Vector N)
{
    return material->color;
}
