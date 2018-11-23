#include <iostream>
#include "vec3.hpp"

struct imagedata {
  int xs, ys;
};


void print_imagedata(imagedata& img) {
  std::cout << "P3\n" << img.xs << ' ' << img.ys << "\n255\n";

  for(int j = img.ys; j >= 0; --j) {
    std::cout << "# Line " << img.ys-j << '\n';

    for(int i = 0; i < img.xs; ++i) {
      vec3 colour(float(i) / float(img.xs), float(j) / float(img.ys), 0.2);

      int ir = int(255.99 * colour.r());
      int ig = int(255.99 * colour.g());
      int ib = int(255.99 * colour.b());

      std::cout << ir << ' ' << ig << ' ' << ib << '\n';
    }
  }
}

int main() {
  imagedata image = { 800, 600 };
  print_imagedata(image);
}
