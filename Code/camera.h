//
//  Framework for a raytracer
//  File: sphere.h
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

#ifndef CAMERA_H
#define CAMERA_H

#include "triple.h"

class Camera
{
private:
	Point eye;
    Point center;
    Vector up;
    double view_width;
    double view_height;

public:
//    Camera(Point eye, Point center, double view_width, double view_height, Vector up, double pixel_size) : eye(eye), center(center), view_width(view_width), view_height(view_height), up(up), pixel_size(pixel_size) { }
	Camera() {}

	void setEye(Point e);
	void setCenter(Point c);
	void setUp(Vector u);
	void setViewWidth(double w);
	void setViewHeight(double h);
	void setPixelSize(double ps);

	Point getEye();
	Point getCenter();
	Vector getUp();
	double getViewWidth();
	double getViewHeight();
};

#endif /* end of include guard: SPHERE_H_115209AE */
