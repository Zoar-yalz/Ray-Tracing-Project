#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"


#include"scenes.h"



color ray_color(const ray& r, const color& background, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0, 0, 0);

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
}


int main() {
    stbi_flip_vertically_on_write(true);//ÆôÓÃ·­×ª

    // Image
     auto aspect_ratio = 16.0 / 9.0;
     int image_width = 400;
     int image_height = static_cast<int>(image_width / aspect_ratio);
    const char image_name[] = "output.png";
    int samples_per_pixel = 400;
    const int max_depth = 50;
    color background(0, 0, 0);


    // World
    hittable_list world;


    // Camera

    point3 lookfrom;
    point3 lookat;

    auto vfov = 40.0;
    auto aperture = 0.0;

    switch (0) {
    case 1:
        world = random_scene();
        background = color(0.70, 0.80, 1.00);
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        vfov = 20.0;
        aperture = 0.1;
        break;

    case 2:
        world = two_spheres();
        background = color(0.70, 0.80, 1.00);
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        vfov = 20.0;
        break;
    case 3:
        world = two_perlin_spheres();
        background = color(0.70, 0.80, 1.00);
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        vfov = 20.0;
        break;
    case 4:
        world = earth();
        background = color(0.70, 0.80, 1.00);
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        vfov = 20.0;
        break;
    case 5:
        world = simple_light();
        samples_per_pixel = 400;
        background = color(0, 0, 0);
        lookfrom = point3(26, 3, 6);
        lookat = point3(0, 2, 0);
        vfov = 20.0;
        break;
    case 6:
        world = cornell_box();


        aspect_ratio = 1.0;
        image_width = 600;

        samples_per_pixel = 200;
        background = color(0, 0, 0);
        lookfrom = point3(278, 278, -800);
        lookat = point3(278, 278, 0);
        vfov = 40.0;
        break;
    case 7:
        world = cornell_smoke();
        aspect_ratio = 1.0;
        image_width = 600;
        samples_per_pixel = 200;
        lookfrom = point3(278, 278, -800);
        lookat = point3(278, 278, 0);
        vfov = 40.0;
        break;
    default:
    case 8:
        world = final_scene();
        aspect_ratio = 1.0;
        image_width = 800;
        samples_per_pixel = 1000;
        background = color(0, 0, 0);
        lookfrom = point3(478, 278, -600);
        lookat = point3(278, 278, 0);
        vfov = 40.0;
        break;
    }

    // Camera

    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    image_height = static_cast<int>(image_width / aspect_ratio);
    char* image_data = new char[image_height * image_width * 3];

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    // Render
    std::cout << "Processing image:" << image_width << "*" << image_height << "\n";
    std::cout << "samples_per_pixel:" << samples_per_pixel<<"\n";
    time_t start_time=time(NULL);
    time_t now_time = time(NULL);
    time_t estimate_time;
    int remain = image_height;
    for (int j = image_height - 1; j >= 0; --j) {
        #pragma omp parallel for
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, background, world, max_depth);
            }
            write_color(image_data,image_width,image_height,j,i,pixel_color, samples_per_pixel);

        }
        now_time = time(NULL);
        estimate_time = (double)image_height / ((double)image_height - (double)j) * (double)(now_time - start_time);
        std::cerr << "\rScanlines remaining: " << j  << "\testimate time remaining : " << estimate_time << 's' << std::flush;

    }
    stbi_write_png(image_name, image_width, image_height, 3, image_data, 0);
    std::cerr << "\nDone.\n";
}