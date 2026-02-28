#pragma once
#include <cstddef>
#include "App.hpp"

constexpr float PI = 3.141592;
constexpr size_t WINDOW_WIDTH = 1024;
constexpr size_t WINDOW_HEIGHT = 768;
constexpr size_t TEXTURE_WIDTH = 200;
constexpr size_t TEXTURE_HEIGHT = 150;

void render_circle(const App& app, int x, int y, int radius);