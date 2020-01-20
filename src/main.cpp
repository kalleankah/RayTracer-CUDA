#include <iostream>
#include <fstream>
#include <limits>
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "random.h"
#include "material.h"

#define MAXFLOAT std::numeric_limits<float>::max()

vec3 color(const ray& r, hittable *world, int depth) {
  hit_record rec;
  if (world->hit(r, 0.001, MAXFLOAT, rec)) {
    ray scattered;
    vec3 attenuation;
    if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return attenuation*color(scattered, world, depth+1);
    }
    else {
      return vec3(0,0,0);
    }
  }
  else {
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
  }
}

//Main render loop
int main() {
  //Define image size
  int nx = 800;
  int ny = 400;
  int ns = 100;

  //Create file
  std::ofstream file("img.ppm");
  //Add PPM header info
  file << "P3\n" << nx << " " << ny << "\n255\n";

  //Define pixel grid corners
  vec3 lower_left_corner(-2.0, -1.0, -1.0);
  vec3 horizontal(4.0, 0.0, 0.0);
  vec3 vertical(0.0, 2.0, 0.0);
  vec3 origin(0.0, 0.0, 0.0);

  //Create list of world objets "hittable"
  hittable *list[4];
  list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
  list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
  list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
  list[3] = new sphere(vec3(-1,0,-1), 0.5, new metal(vec3(0.8, 0.8, 0.8), 1.0));
  hittable *world = new hittable_list(list,4);

  //Create camera
  camera cam;

  //Loop through pixels
  for (int j = ny-1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      vec3 col(0, 0, 0);
      for (int s = 0; s < ns; s++) {
        float u = float(i + random_double()) / float(nx);
        float v = float(j + random_double()) / float(ny);
        //Send a ray in a random direction inside the current pixel in the grid
        ray r = cam.get_ray(u, v);
        col += color(r, world, 0);
      }
      col /= float(ns);
      //Square root to correct for gamma 2.0
      col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );

      //Write color data to file
      file << (int)(255.99*col[0]) << " " << (int)(255.99*col[1]) << " " << (int)(255.99*col[2]) << "\n";
    }
  }
  file.close();
}
