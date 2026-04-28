#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>

SDL_Window* window;
SDL_Texture* texture;
SDL_Renderer* renderer;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
  window = SDL_CreateWindow("Smiley", 640, 480, SDL_WINDOW_RESIZABLE);

  SDL_Surface* smiley = IMG_Load("dist/smiley.png");

  // using software renderer because my x220 doesn't support Vulkan
  renderer = SDL_CreateRenderer(window, nullptr);

  texture = SDL_CreateTextureFromSurface(renderer, smiley);

  SDL_DestroySurface(smiley);
  smiley = nullptr;

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  SDL_RenderClear(renderer);
  SDL_RenderTexture(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
    return SDL_APP_SUCCESS;
  }

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  SDL_DestroyWindow(window);
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
}
