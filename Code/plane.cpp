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
