#pragma once 
#include <SDL.h>
#include <SDL_quit.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <memory>
#include <iostream>
#include <vector>
#include <utility>

namespace AppDeleter {
    struct WindowDeleter {
        void operator()(SDL_Window* window) {
            if (window) {
                SDL_DestroyWindow(window);
                std::cout << "Deleting SDL Window" << std::endl;
                SDL_Quit();
                std::cout << "Deleting SDL Context" << std::endl;


            }
        }
    };

    struct RendererDeleter {
        void operator()(SDL_Renderer* renderer) {
            if (renderer) {
                SDL_DestroyRenderer(renderer);
                std::cout << "Deleting SDL Renderer" << std::endl;
            }
        }
    };
}

class App {
    typedef int SDL_Context;
    App(App&&) = delete;
    App(const App&) = delete;
    App& operator=(App&&) = delete;
    App& operator=(const App&) = delete;

    bool register_imgui();
    
    void input();
    void update();
    void render();
    std::vector<SDL_Texture*> textures_;
    std::unique_ptr<SDL_Window, AppDeleter::WindowDeleter> window_;
    std::unique_ptr<SDL_Renderer, AppDeleter::RendererDeleter> renderer_;
public:
    bool initialized_ = false;

    App() = default;
    ~App() = default;
    void init();
    void run();

   void addTexture(SDL_Texture* texture);
   SDL_Texture* getTexture(size_t idx) const; 

    SDL_Window* getWindow() const;
    SDL_Renderer* getRenderer() const;

};