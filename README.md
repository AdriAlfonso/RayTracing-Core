# RayTracing core implementation example
This is the core of a ray tracing application that generates images by simulating the behavior of light. It uses the C++ programming language and the Qt framework.

## Description
The application takes as input a scene that contains information about objects and their materials, lighting sources, and camera setup. It then calculates the interaction of light rays with the objects in the scene to generate an image. The application supports different shading strategies, such as Phong shading, and can handle shadows and reflections (no shading or reflection code provided, this is just the core).

This code is an example extracted from a university project from a Graphics and Data Visualization course. The full project includes a graphical user interface for setting up and rendering scenes, as well as additional features such as shadings, virtual scenes, usage of different materials, etc.

## Functions
* RayTracer::RayTracer(QImage *i): The constructor of the RayTracer class that initializes the setup and scene from the Controller, and stores the provided QImage for later use.
* void RayTracer::run(): The main function that runs the ray tracing algorithm. It iterates over each pixel in the image, calculates the color for that pixel, and sets the pixel color in the QImage.
* void RayTracer::setPixel(int x, int y, vec3 color): A helper function that ensures the color values are within the valid range of 0-255, and sets the pixel color in the QImage.
* vec3 RayTracer::RayPixel(Ray &ray, int depth): A recursive function that calculates the color for a given ray. It first checks if the ray intersects with an object in the scene, and calculates shading based on the material properties and lights. If the maximum recursion depth has not been reached, it also calculates the scattered color by generating a new ray and calling the function recursively. If the maximum recursion depth has been reached, it adds ambient light based on the material properties. If the ray does not intersect with any object in the scene, it calculates the background color based on the ray direction.
* void RayTracer::init(): An initialization function that switches the shading strategy if necessary by calling the switchShading() function of the ShadingFactory.

## Credits
The full project was developed by [Noah Márquez, Alejandro Guzmán, Oscar de Caralt and Adrià Alfonso].
