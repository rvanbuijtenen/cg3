//
//  Framework for a raytracer
//  File: raytracer.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Author: Maarten Everts
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//
#include "plane.h"
#include "triangle.h"
#include "raytracer.h"
#include "object.h"
#include "sphere.h"
#include "material.h"
#include "light.h"
#include "image.h"
#include "yaml/yaml.h"
#include <ctype.h>
#include <fstream>
#include <assert.h>


// Functions to ease reading from YAML input
void operator >> (const YAML::Node& node, Triple& t);
Triple parseTriple(const YAML::Node& node);

void operator >> (const YAML::Node& node, Triple& t)
{
    assert(node.size()==3);
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;
}

Triple parseTriple(const YAML::Node& node)
{
    Triple t;
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;	
    return t;
}

int Raytracer::parseRenderMode(const YAML::Node* node)
{
    if(!node || node[0] == "phong") {
        return 0;
    } else if (node[0] == "zbuffer") {
        return 1;
    } else if (node[0] == "normal") {
        return 2;
    }

    return 0;
}

bool Raytracer::parseShadows(const YAML::Node* node)
{
    if(node && node[0] == "true") {
        printf("shadows=true\n");
        return true;
    } else {
        printf("shadows=false\n");
        return false;
    }
}

int Raytracer::parseAntiAlias(const YAML::Node& node) {
    int factor = 1;
    if(node.FindValue("factor")) {
        node["factor"] >> factor;
    }
    return factor;
}

Camera Raytracer::parseCamera(const YAML::Node &node) {
    Point eye = parseTriple(node["eye"]);
    Point center = parseTriple(node["center"]);
    double width = node["viewSize"][0];
    double height = node["viewSize"][1];
    Vector up = parseTriple(node["up"]);


    Camera cam = Camera();
    cam.setEye(eye);
    cam.setCenter(center);
    cam.setViewWidth(width);
    cam.setViewHeight(height);
    cam.setUp(up);

    return cam;
}

Camera Raytracer::parseEye(const YAML::Node &node) {
    Point eye = parseTriple(node);

    Camera cam = Camera();
    cam.setEye(eye);
    cam.setCenter(Point(200, 200, 0));
    cam.setViewHeight(400);
    cam.setViewWidth(400);
    cam.setUp(Vector(0,1,0));
    return cam;
}

Material* Raytracer::parseMaterial(const YAML::Node& node)
{
    Material *m = new Material();
    node["color"] >> m->color;	
    node["ka"] >> m->ka;
    node["kd"] >> m->kd;
    node["ks"] >> m->ks;
    node["n"] >> m->n;
    return m;
}

Object* Raytracer::parseObject(const YAML::Node& node)
{
    Object *returnObject = NULL;
    std::string objectType;
    node["type"] >> objectType;

    if (objectType == "sphere") {
        Point pos;
        node["position"] >> pos;
        double r;
        node["radius"] >> r;
        Sphere *sphere = new Sphere(pos,r);		
        returnObject = sphere;
    } else if(objectType == "plane") {
        Point pos, N;
        node["position"] >> pos;
        node["normal"] >> N;
        Plane *plane = new Plane(pos, N.normalized());
        returnObject = plane;
    } else if(objectType == "triangle") {
        Point p1, p2, p3;
        node["p1"] >> p1;
        node["p2"] >> p2;
        node["p3"] >> p3;
        Triangle *triangle = new Triangle(p1,p2,p3);
        returnObject = triangle;
    }

    if (returnObject) {
        // read the material and attach to object
        returnObject->material = parseMaterial(node["material"]);
    }

    return returnObject;
}

Light* Raytracer::parseLight(const YAML::Node& node)
{
    Point position;
    node["position"] >> position;
    Color color;
    node["color"] >> color;
    return new Light(position,color);
}

int Raytracer::parseRecursionDepth(const YAML::Node& node) {
    int n;
    node >> n;
    return n;
}
/*
* Read a scene from file
*/

bool Raytracer::readScene(const std::string& inputFilename)
{
    // Initialize a new scene
    scene = new Scene();

    // Open file stream for reading and have the YAML module parse it
    std::ifstream fin(inputFilename.c_str());
    if (!fin) {
        cerr << "Error: unable to open " << inputFilename << " for reading." << endl;;
        return false;
    }
    try {
        YAML::Parser parser(fin);
        if (parser) {
            YAML::Node doc;
            parser.GetNextDocument(doc);

            // Read rendermode, defaults to "normal" if not set
            scene->setRenderMode(parseRenderMode(doc.FindValue("RenderMode")));
            scene->setShadows(parseShadows(doc.FindValue("Shadows")));

            if(doc.FindValue("SuperSampling")) {
                scene->setAntiAlias(parseAntiAlias(doc["SuperSampling"]));
            } else {
                scene->setAntiAlias(1);
            }

            if(doc.FindValue("MaxRecursionDepth")) {
                scene->setRecursionDepth(parseRecursionDepth(doc["MaxRecursionDepth"]));
            }
            
            
            if(doc.FindValue("Camera")) {
                scene->setCamera(parseCamera(doc["Camera"]));
            } else {
                scene->setCamera(parseEye(doc["Eye"]));
            }
            // Read scene configuration options
            //scene->setEye(parseTriple(doc["Eye"]));

            // Read and parse the scene objects
            const YAML::Node& sceneObjects = doc["Objects"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of objects." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneObjects.begin();it!=sceneObjects.end();++it) {
                Object *obj = parseObject(*it);
                // Only add object if it is recognized
                if (obj) {
                    scene->addObject(obj);
                } else {
                    cerr << "Warning: found object of unknown type, ignored." << endl;
                }
            }

            // Read and parse light definitions
            const YAML::Node& sceneLights = doc["Lights"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of lights." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneLights.begin();it!=sceneLights.end();++it) {
                printf("adding light \n");
                scene->addLight(parseLight(*it));
            }
        }
        if (parser) {
            cerr << "Warning: unexpected YAML document, ignored." << endl;
        }
    } catch(YAML::ParserException& e) {
        std::cerr << "Error at line " << e.mark.line + 1 << ", col " << e.mark.column + 1 << ": " << e.msg << std::endl;
        return false;
    }

    cout << "YAML parsing results: " << scene->getNumObjects() << " objects read." << endl;
    return true;
}

void Raytracer::renderToFile(const std::string& outputFilename)
{   
    Camera cam = scene->getCamera();
    Image img(cam.getViewWidth(), cam.getViewHeight());
    cout << "Tracing..." << endl;
    scene->render(img);
    cout << "Writing image to " << outputFilename << "..." << endl;
    img.write_png(outputFilename.c_str());
    cout << "Done." << endl;
}