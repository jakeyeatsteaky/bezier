#pragma once
#include <cstddef>
#include <cstdint>

typedef enum AppStartupFlags {
    APP_STARTUP_FLAGS_SUCCESS = 0x0000,
    APP_STARTUP_FLAGS_BEZIER = 0x0001,
    APP_STARTUP_FLAGS_DEBUG = 0x1000
} AppStartupFlags;

typedef uint32_t _AppStartupFlags_;

constexpr AppStartupFlags APP_STARTUP_FLAGS_MIN = APP_STARTUP_FLAGS_SUCCESS;
constexpr AppStartupFlags APP_STARTUP_FLAGS_MAX = APP_STARTUP_FLAGS_DEBUG;

class App;
uint32_t initialize_debug_window(const App& app);
uint32_t initialize_bezier_window(const App& app);

constexpr float PI = 3.141592;
constexpr size_t WINDOW_WIDTH = 1600;
constexpr size_t WINDOW_HEIGHT = 900;
constexpr size_t TEXTURE_WIDTH = 200;
constexpr size_t TEXTURE_HEIGHT = 150;

void render_circle(const App& app, int x, int y, int radius);

