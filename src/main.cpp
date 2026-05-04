#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>

struct AppState {
  SDL_Window* window;
  SDL_Texture* texture;
  SDL_Renderer* renderer;
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

  state->window = SDL_CreateWindow("Smiley", 640, 480, SDL_WINDOW_RESIZABLE);
  if (!state->window) {
    SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // let SDL choose the rendering backend
  state->renderer = SDL_CreateRenderer(state->window, nullptr);
  if (!state->renderer) {
    SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Error selecting rendering backend: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_Surface* smiley = IMG_Load("dist/smiley.png");
  state->texture = SDL_CreateTextureFromSurface(state->renderer, smiley);
  SDL_DestroySurface(smiley);
  smiley = nullptr;

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

  SDL_free(state);
}
