//
//  Framework for a raytracer
//  File: scene.cpp
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
#include "scene.h"
#include "material.h"

 #define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
 #define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
        
int Scene::shadow(const Ray &ray) {
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t > 0 && ray.D.dot(hit.N) > 0) {
            // ray intersects an object, so we should render a shadow
            return i;
        }
    }
    // no objects between hit and light, so there is no shadow
    return -1;
}

Color Scene::traceGooch(const Ray &ray, int num_reflects) {
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    int min_hit_idx = -1;
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
            min_hit_idx = i;
        }
    }

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    double t_offset = 0.000001;
    Material *material = obj->material;            //the hit objects material
    Point hit = ray.at(min_hit.t - t_offset);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;
}

Color Scene::tracePhong(const Ray &ray, int num_reflects)
{
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    int min_hit_idx = -1;
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
            min_hit_idx = i;
        }
    }

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    double t_offset = 0.000001;
    Material *material = obj->material;            //the hit objects material
    Point hit = ray.at(min_hit.t - t_offset);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector



    Color diffuse, specular;
    Vector L,R;

    // we need a small offset to prevent reflecting rays and shadow rays from
    // intersecting again at the point of intersection
    

    // compute ambient part
    Color matcolor = obj->getTextureColor(N);
    Color color = matcolor * material->ka;

    // loop over each light to compute specular and diffuse parts     
    for(unsigned int i = 0; i<lights.size(); i++) {
        L = (lights[i]->position - hit).normalized();
        R = L - (2 * N.dot(L) * N);

        // cast a ray from hit to light. If this ray intersects an object we render a shadow
        Ray shadowRay = Ray(hit, (lights[i]->position-hit).normalized());
        int shadowObjIdx = shadow(shadowRay);
        if(shadowObjIdx >= 0 && shadowObjIdx != min_hit_idx) {
            // ray from intersect point to light intersects an object, so this light
            // does not produce a color value
            continue;
        }   

        // compute diffuse portion of light
        color += matcolor * material->kd * lights[i]->color * MAX(0, N.dot((hit-lights[i]->position).normalized()));

        // compute specular portion of light
        if(R.dot(V) < 0) {
            Color spec = pow(R.dot(V), material->n) * lights[i]->color * material->ks;
            color += spec;
        }
    }

            // recursively reflect rays and add the resulting color to the total color 
        if(material->ks > 0 && num_reflects > 1) {
            R = ray.D - (2 * N * N.dot(ray.D));
            Ray reflectRay = Ray(hit, R.normalized());
            color += tracePhong(reflectRay, num_reflects - 1) * material->ks;
        }
    
    return color;
}

Color Scene::traceZBuffer(const Ray &ray, Point pixel)
{
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
        }
    }

    /* compute distance from the pixel to the intersection point, store at as temorary (r,g,b) values */
    double z = (pixel - ray.at(min_hit.t)).length();

    if(obj && min_hit.t > 0) {       
        return Color(z,z,z);
    } else {
        // no hit, so return black
        return Color(0,0,0);
    }
}

Color Scene::traceNormal(const Ray &ray)
{
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
        }
    }

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Vector N = min_hit.N;                          //the normal at hit point

    // compute color for normal buffer and scale from [-1,1] to [0,1]
    Color color = 0.5 * (Color)N + Color(0.5, 0.5, 0.5);

    return color;
}

void Scene::render(Image &img) {
    Point eye, pixel, center;
    Vector dir, up, gaze, right, view, H_FOV, V_FOV, displ;
    Color col;
    double pixel_size, h_ratio, w_ratio, x_displ, y_displ;
    int w, h, i, j, nsamples;


    Color max_col = Color(0,0,0);

    // retreive the relevant parameters from camera
    center = camera.getCenter();
    eye = camera.getEye();
    up = camera.getUp();

    // compute the pixel size
    pixel_size = up.length();

    // gaze, right and view vectors from eye (camera) to projection plane
    view = (center-eye).normalized();
    right = view.cross(up).normalized();

    //view = right.cross(gaze);
    //printf("%f %f %f\n", gaze.x, gaze.y, gaze.z);
    

    H_FOV = right * ((camera.getViewWidth() * pixel_size) / 2);
    V_FOV = up.normalized() * ((camera.getViewHeight() * pixel_size) / 2);
    printf("%f %f %f\n", H_FOV.x, H_FOV.y, H_FOV.z);
    printf("%f %f %f\n", V_FOV.x, V_FOV.y, V_FOV.z);
    // compute the number of rays we trace per pixel
    nsamples = antiAlias * antiAlias;

    // loop over every pixel
    w = img.width();
    h = img.height();
    for (int y = 0; y < h; y++) {\
        for (int x = 0; x < w; x++) {
            Color color(0.0, 0.0, 0.0);

            // compute the scalars for FOV vectors
            w_ratio = ((2.0 * x) / w) - 1;
            h_ratio = ((2.0 * (h-1-y)) / h) - 1;

            Vector H_F = H_FOV * w_ratio;
            Vector V_F = V_FOV * h_ratio;
            // translate the pixel from center point to pixel position in world space
            Point pixel =  (center + Vector(0.5, 0.5, 0.0));
            
            // create antiAlias * antiAlias rays for each pixel
            for(i = 1; i <= antiAlias; i++) {
                for(j = 1; j <= antiAlias; j++) {
                    // compute x and y displacement for the additional anti-aliassing rays
                    x_displ = (i - 0.5)/antiAlias;
                    y_displ = (j - 0.5)/antiAlias;

                    // multiply this displacement by pixel size
                    x_displ *= pixel_size;
                    y_displ *= pixel_size;

                    // store displacements in a vector. Note that we take the negative y displacement 
                    // since we render starting from the top-right to bottom-left of the image
                    displ = Vector(x_displ, -y_displ, 0.0);

                    // compute the view direction for the adjusted supersampled rays
                    Point pix = pixel + displ;
                    pix = pix + H_F + V_F;
                    dir = ((pix + displ) - eye).normalized();
                    
                    // create the ray and trace it using the appropriate algorithm. 
                    // Divide the result by number of samples (antiAlias^2) to average the color.
                    const Ray ray(eye, dir);
                    switch(renderMode) {
                        case 0: color += tracePhong(ray, recursionDepth)/nsamples;
                            break;
                        case 1: color +=  traceZBuffer(ray, pixel + displ)/nsamples;
                            break;
                        case 2: color += traceNormal(ray)/nsamples;
                            break;
                        case 3: color += traceGooch(ray, recursionDepth)/nsamples;
                    }
                }
            }
            if(renderMode == 1) {
                // if we're computing the z buffer we need to remember the maximum color.
                if(color.r > max_col.r) {
                    max_col = color;
                }
            } else {
                color.clamp();
            }


            // store the color in the image
            img(x,y) = color;
        }
    }

    if(renderMode == 1) {
        // We still need to normalize the color values to the range [0,1] for z-buffering 
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                Point pixel(x+0.5, h-1-y+0.5, 0);
                double max_l = (pixel - eye).length();
                printf("%f, %f\n", max_col.r, max_l);
                // once again, r = g = b, sp we just use max_col.r
                img(x,y) = img(x,y) * (1/(max_l-max_col.r));
            }
        }
    }
}

void Scene::addObject(Object *o)
{
    objects.push_back(o);
}

void Scene::addLight(Light *l)
{
    lights.push_back(l);
}

void Scene::setEye(Triple e)
{
    eye = e;
}

void Scene::setRenderMode(int m) {
    renderMode = m;
}

void Scene::setShadows(bool s) {
    shadows = s;
}

void Scene::setAntiAlias(int n) {
    antiAlias = n;
}

void Scene::setRecursionDepth(int n) {
    recursionDepth = n;
}

void Scene::setCamera(Camera c) {
    camera = c;
}

Camera Scene::getCamera() {
    return camera;
}
