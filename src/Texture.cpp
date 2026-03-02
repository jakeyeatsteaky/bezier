#include "Texture.hpp"
#include <SDL_pixels.h>
#include <SDL_render.h>

constexpr int DefaultTextureWidth = 200;
constexpr int DefaultTextureHeight = 150;

static TextureConfig DefaultConfig = {.pixelFormat_ = SDL_PIXELFORMAT_RGBA8888,
                                      .access_ = SDL_TEXTUREACCESS_TARGET,
                                      .width_ = DefaultTextureWidth,
                                      .height_ = DefaultTextureHeight};

TextureConfig Texture::GetDefaultTextureConfig() { return DefaultConfig; }

Texture::Texture(SDL_Renderer *renderer)
    : appRenderer_(renderer), config_(GetDefaultTextureConfig()) {

  auto pTexture =
      SDL_CreateTexture(renderer, config_.pixelFormat_, config_.access_,
                        config_.width_, config_.height_);
  if (!pTexture) {
    std::cerr << "SDL_Texture error: " << SDL_GetError() << std::endl;
    return;
  }

  texture_.reset(pTexture);
}

Texture::Texture(TextureConfig&& config, SDL_Renderer* renderer) : appRenderer_(renderer), config_(config) {
  auto pTexture =
      SDL_CreateTexture(renderer, config_.pixelFormat_, config_.access_,
                        config_.width_, config_.height_);
  if (!pTexture) {
    std::cerr << "SDL_Texture error: " << SDL_GetError() << std::endl;
    return;
  }

  texture_.reset(pTexture);
}

SDL_Texture* Texture::getTexture() const {
    return texture_.get();
}

