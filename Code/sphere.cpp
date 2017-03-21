//
//  Framework for a raytracer
//  File: sphere.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "sphere.h"
#include <stdio.h>
#include <iostream>
#include "material.h"
#include "triple.h"
#include <math.h>

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray)
{
    double b, c, d, xp, xn, t;

    // compute b & c values for ABC formula according to formulas in slide lecture 1
    // we don't define a since always is equal to 1
    b = 2 * (ray.O - position).dot(ray.D);
    c = (ray.O - position).dot(ray.O - position) - r*r;

    // compute determinant of ABC formula
    d = b*b - 4*c;
    Vector N;
    if (d > 0) {
        // 2 roots, select the min value
        xp = (-b + sqrt(d))/2;
        xn = (-b - sqrt(d))/2;
        t = xp < xn ? xp : xn;
    } else if (d == 0) {
        // 1 root
        t = -b/2;
    } else {
        // no intersection
        return Hit::NO_HIT();
    }
 
    if(t < 0) {
        return Hit::NO_HIT();
    }
    // Compute surface normal, which when normalized is equal to the radius vector to the point of intersection
    N = (position-ray.at(t)).normalized();
    return Hit(t,N);
}

Color Sphere::getTextureColor(const Vector N)
{
    Color color;
    if (material->texture != NULL) {
        double a2 = a * M_PI / 180.0;
        Triple c2 = r * c;
        c2.normalize();
        Vector d = N;
        double x = d.x*cos(a2) + (1-cos(a2)) * (c2.x*c2.x*d.x + c2.x*c2.y*d.y + c2.x*c2.z*d.z) + (c2.y*d.z - c2.z*d.y) * sin(a2);
        double y = d.y*cos(a2) + (1-cos(a2)) * (c2.y*c2.x*d.x + c2.y*c2.y*d.y + c2.y*c2.z*d.z) + (c2.z*d.x - c2.x*d.z) * sin(a2);
        double z = d.z*cos(a2) + (1-cos(a2)) * (c2.z*c2.x*d.x + c2.z*c2.y*d.y + c2.z*c2.z*d.z) + (c2.x*d.y - c2.y*d.x) * sin(a2);
        d = Triple(x, y, z);
        
        double u = 0.5 + atan2(d.y, d.x) / (2*M_PI);
        double v = 1 - acos(d.z) / M_PI;
        color = material->texture->colorAt(u, v);
    } else {
        color = material->color;
    }
    return color;
}
