Modifications Raytracer 2:

Scene.cpp
	- refactored Render() method 
	- extended Render() method to implement extended camera model
	- added shadow computation to tracePhong()
	- added superSampling computation in render()
	- adapted the calls for normal buffer and z-buffer to also apply super sampling
	- made recursive call to tracePhong() to compute reflections

Raytracer.cpp
	- added parsing options for Camera, MaxRecursiveDepth, Shadows and SuperSampling
	- added variable resolution for image (resolution is taken from camera)

Camera.cpp/h
	- added Camera definitions
	- defined getters and setters for camera attributes





Modifications Raytracer 1:

Sphere.cpp
	- added intersection and normal calculation
Plane.cpp/ && Plane.h
	- added plane definition and plane-ray intersection
Triangle.cpp && triangle.h
	- added triangle difintion and triangle-ray intersection
Scene.h
	- added renderMode variable (int)
Scene.cpp
	- added setRenderMode function
	- split trace(ray) function into:
		tracePhong(ray)
		traceNormal(ray)
		traceZbuffer(ray)
	- added switch case to select render mode in Scene::render()
	- added loop to normalize colors if z-buffer mode is selected

Raytracer.cpp
	- added "triangle" and "plane" parsing to Raytracer::parseObjects()
	- added function Raytracer::parseRenderMode()



Usage:
	make				// compiles the program
	ray <dir/filename.yaml>		// traces the scene described in file 

For an example of a Plane and Triangle definition, see "scene01.yaml" in Code/Raytracer. Syntax for camera, recursive depth, supersampling and shadows are as in the provided example "scene01-camera-ss-reflect-lights-shadows.yaml". This file is included within the Code directory.
