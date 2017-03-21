//
//  Framework for a raytracer
//  File: plane.h
//
//  Authors:
//    Remco van Buijtenen
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#ifndef PLANE_H_
#define PLANE_H_

#include "object.h"

class Plane : public Object
{
public:
    Plane(Point position, Point normal) : position(position), normal(normal) { }

    virtual Hit intersect(const Ray &ray);
    virtual Color getTextureColor(const Vector N);


    const Point position;
    const Point normal;
};

#endif /* end of include guard: SPHERE_H_115209AE */
