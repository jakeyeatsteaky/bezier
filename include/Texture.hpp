#pragma once
#include <SDL.h>
#include <SDL_render.h>
#include <memory>
#include <iostream>

struct TextureConfig {
    SDL_PixelFormatEnum pixelFormat_;
    SDL_TextureAccess access_;
    int width_;
    int height_;
};

struct TextureDeleter {
    void operator()(SDL_Texture* texture) {
        if (texture) { 
            SDL_DestroyTexture(texture);
            texture = nullptr;
            std::cout << "Destroying Texture" << std::endl;
        }
    }
};

class Texture {
    std::unique_ptr<SDL_Texture, TextureDeleter> texture_;
    SDL_Renderer* appRenderer_; // do not delete, non-owning
    TextureConfig config_;

    static TextureConfig GetDefaultTextureConfig();
public:
    explicit Texture(SDL_Renderer* renderer);
    Texture() = delete;
    ~Texture() = default;
    Texture(const Texture& copy);
    Texture(Texture&& move) noexcept;
    Texture& operator=(const Texture& copy);
    Texture& operator=(Texture&& move) noexcept;

    void setTextureConfig(TextureConfig&& config);
    SDL_Texture* getTexture() const;
};