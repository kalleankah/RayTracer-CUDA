#include <iostream>
#include <fstream>
#include <vector>
#include "glm/vec3.hpp"

//Main render loop
int main() {
  int nx = 512;
  int ny = 512;
  //Create file
  std::ofstream file("img.ppm");
  //Add PPM header info
  file << "P3\n" << nx << " " << ny << "\n255\n";
  for (int j = ny-1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      float r = float(i) / float(nx);
      float g = float(j) / float(ny);
      float b = 0.2;
      file << (int)(255.99*r) << " " << (int)(255.99*g) << " " << (int)(255.99*b) << "\n";
    }
  }
  file.close();
}
