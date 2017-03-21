//
//  Framework for a raytracer
//  File: scene.h
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

#ifndef SCENE_H_KNBLQLP6
#define SCENE_H_KNBLQLP6

#include <vector>
#include "triple.h"
#include "light.h"
#include "object.h"
#include "image.h"
#include "camera.h"

class Scene
{
private:
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Triple eye;
    int renderMode;
    bool shadows;
    int antiAlias;
    Camera camera;
    int recursionDepth;
    double b,y,alpha,beta;
public:
    int shadow(const Ray &ray);
    Color traceAntiAlias(Point eye, Point pixel);
    Color traceGooch(const Ray &ray, int num_reflects);
    Color tracePhong(const Ray &ray, int num_reflects);
    Color traceZBuffer(const Ray &ray, Point pixel);
    Color traceNormal(const Ray &ray);
    Color reflect(int num_reflects, const Ray &ray);
    Color trace(const Ray &ray);
    Camera getCamera();
    void render(Image &img);
    void addObject(Object *o);
    void addLight(Light *l);
    void setEye(Triple e);
    void setRenderMode(int m);
    void setShadows(bool s);
    void setAntiAlias(int n);
    void setCamera(Camera c);
    void setRecursionDepth(int n);
    void setGoochParams(double bp, double yp, double alphap, double betap);
    int getRenderMode();
    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
