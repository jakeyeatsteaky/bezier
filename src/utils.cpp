#include "utils.hpp"

#include <iostream>

void function() {
    std::cout << "Calling the test function" << std::endl;
}

void render_circle(const App& app, int x, int y, int r) {
  Uint8 red = 0xFF;
  Uint8 green = 0x0F;
  Uint8 blue = 0x5F;
  SDL_SetRenderDrawColor(app.getRenderer(), red, green, blue, 0xFF);

  std::vector<SDL_Point> pointBuffer{};
  
  //bounds
  int upperBound = y + r;
  int lowerBound = y - r;

  // TODO -- improve using symmetry
  for (int i = lowerBound; i < upperBound; i++) {
    int xLeft = x - sqrt(r*r - (std::pow(i - y,2)));
    int xRight = x + sqrt(r*r - (std::pow(i - y,2)));

    while (xLeft < xRight) {
      int xValue = xLeft;
      pointBuffer.push_back(SDL_Point{xValue,i});
      xLeft++;
    }
  }

  SDL_RenderDrawPoints(app.getRenderer(), pointBuffer.data(), pointBuffer.size());
}