#include "RayTracer.hh"

RayTracer::RayTracer(QImage *i):
    image(i) {

    // Set up the RayTracer with the current setup and scene from the Controller.
    setup = Controller::getInstance()->getSetUp();
    scene = Controller::getInstance()->getScene();
}

void RayTracer::run() {

    // Initialize the RayTracer.
    init();
    auto camera = setup->getCamera();
    int  width = camera->viewportX;
    int  height = camera->viewportY;

    // Iterate over each pixel in the image, starting from the bottom.
    for (int y = height-1; y >= 0; y--) {
        // Display the progress of the ray tracing.
        std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush;
        for (int x = 0; x < width; x++) {
            vec3 color(0, 0, 0);

            float u = (float(x)) / float(width);
            float v = (float(height -y)) / float(height);

            float i = (float(x+1)) / float(width);
            float o = (float((height-y)-1)) / float(height);

            float final_x;
            float final_y;

            Ray r;

            for (int k = 0; k < setup->getSamples(); k++) {
                final_x = r.random(u, i);
                final_y = r.random(o, v);
                r = camera->getRay(final_x, final_y);
                color += this->RayPixel(r, 0);
            }

            // TODO Phase 2: Gamma correction.
            color = vec3(sqrt(color.x/setup->getSamples()), sqrt(color.y/setup->getSamples()), sqrt(color.z/setup->getSamples()));
            color *= 255;
            setPixel(x, y, color);
        }
    }
    std::cerr << "\nFinished.\n";
}

void RayTracer::setPixel(int x, int y, vec3 color) {

    // Ensure that the color values are within the valid range of 0-255.
    if (color.r < 0) color.r = 0;
    if (color.g < 0) color.g = 0;
    if (color.b < 0) color.b = 0;

    if (color.r > 255) color.r = 255;
    if (color.g > 255) color.g = 255;
    if (color.b > 255) color.b  = 255;

    // Set the pixel color in the image.
    image->setPixelColor(x, y, QColor(color.r, color.g, color.b));
}

// Recursive function that calculates the color.
vec3 RayTracer::RayPixel(Ray &ray, int depth) {
    vec3 color;
    vec3 recursiveRay = normalize(ray.getDirection()); // Get normalized ray direction
    HitInfo info;
    auto s = setup->getShadingStrategy(); // Get shading strategy

    // Check if ray intersects with an object
    if (scene->hit(ray, 0.000001f, numeric_limits<float>::infinity(), info)) {
        // Calculate shading based on material properties and lights
        color = s->shading(scene, info, ray.getOrigin(), setup->getLights(), setup->getGlobalLight()) * (vec3(1.f)-info.mat_ptr->kt);

        Ray scatteredRay;
        vec3 c = vec3(0.0f);

        // Check if max recursion depth has not been reached
        if(depth < setup->getMAXDEPTH()) {
            // Scatter ray and calculate scattered color
            if(info.mat_ptr->scatter(ray, info, c, scatteredRay)) {
                color += RayPixel(scatteredRay,depth+1) * c; // Add scattered color to current color
            }
        } else {
            // Add ambient light based on material properties if max recursion depth has been reached
            color += setup->getGlobalLight() * info.mat_ptr->Ka;
        }
    } else {
        // Calculate background color based on ray direction
        float t = 0.5f * (recursiveRay.y+1);
        color = vec3((1-t) * setup->getDownBackground()+ t * setup->getTopBackground());
    }
    return color; // Return final color
}

// Initialization function
void RayTracer::init() {
    auto s = setup->getShadingStrategy(); // get shading strategy
    auto s_out = ShadingFactory::getInstance().switchShading(s, setup->getShadows()); // switch shading strategy
    if (s_out!=nullptr) setup->setShadingStrategy(s_out); // set new shading strategy if not null
}