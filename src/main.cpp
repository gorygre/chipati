#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <memory>
#include <cstring>

#include "chip.hpp"

struct AppState {
  SDL_Window* window;
  SDL_Texture* texture;
  SDL_Renderer* renderer;
  Chip* chip;
};

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
  if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
    SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Startup error initializing SDL subsystems: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  auto state = static_cast<AppState*>(SDL_malloc(sizeof(AppState)));
  if (!state) {
    return SDL_APP_FAILURE;
  }

  state->chip = static_cast<Chip*>(SDL_malloc(sizeof(Chip)));
  if (!state->chip) {
    return SDL_APP_FAILURE;
  }

  // zero out chip's pixels
  for (std::uint64_t& row : state->chip->pixels) {
    row = 0x0;
  }


  state->window = SDL_CreateWindow("Smiley", 640, 480, SDL_WINDOW_RESIZABLE);
  if (!state->window) {
    SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // let SDL choose the rendering backend
  state->renderer = SDL_CreateRenderer(state->window, nullptr);
  if (!state->renderer) {
    SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Could not select rendering backend: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // crude smiley in binary
  state->chip->pixels[0] = 0b00100100;
  state->chip->pixels[1] = 0b00100100;
  state->chip->pixels[2] = 0b10000001;
  state->chip->pixels[3] = 0b01100110;
  state->chip->pixels[4] = 0b00011000;

  SDL_Surface* surface = SDL_CreateSurface(64, 32, SDL_PIXELFORMAT_INDEX1LSB);
  if (!surface) {
    SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Could not create drawing surface: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_Palette* palette = SDL_CreateSurfacePalette(surface);
  palette->colors[0] = SDL_Color{0x00, 0x00, 0x00, 0x00};
  palette->colors[1] = SDL_Color{0xff, 0xff, 0xff, 0xff};

  // load chip's pixels into the surface memory
  std::memcpy(surface->pixels, state->chip->pixels.data(), 8 * 32);

  state->texture = SDL_CreateTextureFromSurface(state->renderer, surface);
  if (!state->texture) {
    SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to upload surface texture: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_DestroySurface(surface);
  surface = nullptr;

  SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Initialization complete");
  *appstate = state;
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  auto state = static_cast<AppState*>(appstate);

  SDL_RenderClear(state->renderer);
  SDL_RenderTexture(state->renderer, state->texture, nullptr, nullptr);
  SDL_RenderPresent(state->renderer);

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
    return SDL_APP_SUCCESS;
  }

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  auto state = static_cast<AppState*>(appstate);

  SDL_DestroyWindow(state->window);
  SDL_DestroyTexture(state->texture);
  SDL_DestroyRenderer(state->renderer);

  SDL_free(state->chip);
  SDL_free(state);
}
