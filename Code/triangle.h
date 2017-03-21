// Author: Remco van Buijtenen & Jurgen Nijland
//
// class defining a triangle based on its 3 corner locations.
//

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "object.h"

class Triangle : public Object
{
public:
    Triangle(Point P_1, Point P_2, Point P_3) : p1(P_1), p2(P_2), p3(P_3) { }

    virtual Hit intersect(const Ray &ray);
    virtual Color getTextureColor(const Vector N);

    const Point p1, p2, p3;
};

#endif /* end of include guard: TRIANGLE_H */
