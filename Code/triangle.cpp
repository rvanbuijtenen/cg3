// Author: Remco van Buijtenen
//
// Implementation of traingle-ray intersection using the Möller–Trumbore intersection algorithm
// 

#include "triangle.h"
#include "plane.h"
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
    
    /*
    Vector A = p1 - ray.O;
    double t2 = A.dot(ray.D);
    //printf("%f %f %f\n", N.x, N.y, N.z);
    printf("%f\n", t2);

    P = e2.cross(ray.D);

    det = e1.dot(P);



    if(det > -EPSILON && det < EPSILON) {
        return Hit::NO_HIT();
    }

    inv_det = 1.0 / det;

    T = ray.O - p1;

    u = T.dot(P) * inv_det;

    if(u < 0.0 || u > 1) {
        return Hit::NO_HIT();
    }

    Q = T.cross(e1);

    v = Q.dot(ray.D) * inv_det;

    if(v < 0 || u + v > 1) {
        return Hit::NO_HIT();
    }

    double t = e2.dot(Q) * inv_det;

    if(t < EPSILON) {
        return Hit::NO_HIT();
    }
    //printf("%f\n", t);
    return Hit(t, N.normalized());


*/

    /*
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
    return Hit(t, -n.normalized());*/
}
