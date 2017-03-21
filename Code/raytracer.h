//
//  Framework for a raytracer
//  File: raytracer.h
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

#ifndef RAYTRACER_H_6GQO67WK
#define RAYTRACER_H_6GQO67WK

#include <iostream>
#include <string>
#include "triple.h"
#include "light.h"
#include "scene.h"
#include "camera.h"
#include "yaml/yaml.h"

class Raytracer {
private:
    Scene *scene;

    // Couple of private functions for parsing YAML nodes
    Material* parseMaterial(const YAML::Node& node);
    Object* parseObject(const YAML::Node& node);
    Light* parseLight(const YAML::Node& node);
    void parseGoochParams(const YAML::Node& node);
    int parseRenderMode(const YAML::Node* node);
    int parseAntiAlias(const YAML::Node& node);
    int parseRecursionDepth(const YAML::Node& node);
    bool parseShadows(const YAML::Node* node);
    void parseMesh(std::string fname);
    Camera parseCamera(const YAML::Node& node);
    Camera parseEye(const YAML::Node& node);

public:
    Raytracer() { }

    bool readScene(const std::string& inputFilename);
    void renderToFile(const std::string& outputFilename);
};

#endif /* end of include guard: RAYTRACER_H_6GQO67WK */
