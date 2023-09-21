#include "JamEngine.hpp"
#include <cstddef>
#include <random>
#include <stdio.h>
static int exitRequest = 0;

#ifdef PSP

#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspkernel.h>

////////////////////////////////////////////
////                                    ////
////           PSP ROUTINES             ////
////////////////////////////////////////////

int isRunning() { return !exitRequest; }
int exitCallback(int arg1, int arg2, void *common) {
  exitRequest = 1;
  return 0;
}

int callbackThread(SceSize args, void *argp) {
  int callbackID;
  callbackID = sceKernelCreateCallback("Exit Callback", exitCallback, NULL);
  sceKernelRegisterExitCallback(callbackID);
  sceKernelSleepThreadCB();
  return 0;
}
int setupExitCallback() {
  int threadID = 0;
  threadID = sceKernelCreateThread("Callback Update Thread", callbackThread,
                                   0x11, 0xFA0, THREAD_ATTR_USER, 0);
  if (threadID >= 0) {
    sceKernelStartThread(threadID, 0, 0);
  }
  return threadID;
}

#endif
/////////////////////////////////////////////////
////////////////////////////////////////////////

std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(1, 3);

JamEngine::JamEngine()
    : Renderer(nullptr), GameController(nullptr), Window(nullptr),
      camera(SCREEN_WIDTH, SCREEN_HEIGHT) {
  buzz = 0;
}

JamEngine::~JamEngine() {
  // Clear del motor de audio
  endAudio();

  SDL_DestroyWindow(Window);
  TTF_Quit();
  // Mix_CloseAudio();
  SDL_Quit();
}

bool JamEngine::Init() {

#ifdef PSP
  setupExitCallback();
#endif

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO |
               SDL_INIT_JOYSTICK) < 0)
    return false;

  Window = SDL_CreateWindow("JamEngine", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (!Window)
    return false;

  Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);

  // Check for joysticks
  if (SDL_NumJoysticks() < 1) {
    // printf( "Warning: No joysticks connected!\n" );
  } else {
    // Load joystick
    GameController = SDL_JoystickOpen(0);
    if (GameController == nullptr) {
      // printf( "Warning: Unable to open game controller! SDL Error: %s\n",
      // SDL_GetError() );
    }
  }

  // Check if first joystick is game controller interface compatible
  if (!SDL_IsGameController(0)) {
    printf("Warning: Joystick is not game controller interface compatible! SDL "
           "Error: %s\n",
           SDL_GetError());
  } else {
    // Open game controller and check if it supports rumble
    gGameController = SDL_GameControllerOpen(0);
    if (!SDL_GameControllerHasRumble(gGameController)) {
      printf("Warning: Game controller does not have rumble! SDL Error: %s\n",
             SDL_GetError());
    }
  }

  if (TTF_Init() == -1) {
    //  printf("TTF_Init: %s\n", TTF_GetError());
    exit(2);
  }

  // Init del motor de audio
  // initAudio();

  // el render se ha creado?
  if (!Renderer)
    return false;

  Sprite::setRenderer(Renderer);
  Tilemap::setRenderer(Renderer);
  eText::setRenderer(Renderer);

  context_data ctx;
  ctx.transform_x = 0;
  ctx.transform_y = 0;
  ctx.angle = 0;
  ctx.color_r = 0;
  ctx.color_g = 0;
  ctx.color_b = 0;

  this->ctx_stack.push(ctx);

  return true;
}

void JamEngine::drawTexture(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dst,
                            float rot) {

  // center on screen
  dst->x += 160;

  SDL_RenderCopyEx(Renderer, texture, src, dst, rot, NULL, SDL_FLIP_NONE);
}

void JamEngine::drawRect(SDL_Rect *dst, int color_r, int color_g, int color_b) {
  SDL_SetRenderDrawColor(Renderer, color_r, color_g, color_b, 255);

  // center on screen
  dst->x += 160;

  SDL_RenderFillRect(Renderer, dst);
}

void JamEngine::drawText(std::string text, SDL_Rect *dst, int color_r,
                         int color_g, int color_b) {

  eText txt = eText(dst->x, dst->y);

  txt.setString(text);
  txt.Draw();
}

void JamEngine::Clear() {
  SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
  SDL_RenderClear(Renderer);
}

void JamEngine::Render() {
  AssetManager *Assets = AssetManager::Instance();

  a = SDL_GetTicks();
  int delta = a - b;

  if (delta > 1000 / 60.0) {
    b = a;

    // pintamos las imagenes
    for (int image = 0; image < 6; image++) {
      for (int i = 0; i < draw_pool_size[image]; i++) {
        draw_data d = draw_pool[image][i];
        drawTexture(Assets->getTexture(image), &d.s, &d.d, d.angle);
      }
      draw_pool_size[image] = 0;
    }

    // pintamos rectanguos
    for (int i = 0; i < rect_pool_size; i++) {
      draw_data d = rect_pool[i];
      drawRect(&d.d, d.color_r, d.color_g, d.color_b);
    }
    rect_pool_size = 0;

    // pintamos los textos
    for (int i = 0; i < text_pool_size; i++) {
      text_data d = text_pool[i];
      drawText(d.text, &d.d, d.color_r, d.color_g, d.color_b);
    }

    text_pool_size = 0;

    SDL_RenderPresent(Renderer);
  } else {
    SDL_Delay((1000 / 60) - delta);
  }
}

void JamEngine::Draw() {
  uint16_t i = 0;
  while (i < Z_INDEX_LEVELS * DRAWABLES_PER_LEVEL) {
    Drawable *Current = DRAWABLEPOOL[i];
    if (Current && Current->Visibility)
      Current->Draw();
    ++i;
  }
}

void JamEngine::DrawUno(uint16_t n, uint16_t p) {
  Drawable *Current = DRAWABLEPOOL[n * DRAWABLES_PER_LEVEL + p];
  Current->Draw();
  // SDL_RenderPresent(Renderer);
}

void JamEngine::Update() {
  // update inputs?
  // update physics?

  if (buzzTimer-- == 0) {
    isBuzz = false;
  }
}

bool JamEngine::isOpen() { return !exitRequest; }
void JamEngine::close() { exitRequest = true; }

// DRAWABLEPOOL
void JamEngine::setDrawable_ZIndex(Drawable *D, uint8_t Z_Index) {
  uint8_t Position = CurrentMaxZIndexID[Z_Index]++;
  if (Position > DRAWABLES_PER_LEVEL - 1) {
    CurrentMaxZIndexID[Z_Index] = 0;
    Position = 0;
  }
  setDrawable_ZIndex(D, Z_Index, Position);
}

void JamEngine::setView(eView v) { camera = v; }

void JamEngine::moveView(int x, int y) { camera.move(x, y); }

int JamEngine::getTicks() { return (int)SDL_GetTicks(); }

void JamEngine::setDrawable_ZIndex(Drawable *D, uint8_t Z_Index,
                                   uint8_t Position) {
  DRAWABLEPOOL[Z_Index * DRAWABLES_PER_LEVEL + Position] = D;
}

void JamEngine::clearAllDrawables() {

  for (uint16_t i = 0; i < Z_INDEX_LEVELS * DRAWABLES_PER_LEVEL; ++i)
    DRAWABLEPOOL[i] = nullptr;

  for (uint16_t i = 0; i < Z_INDEX_LEVELS; ++i)
    CurrentMaxZIndexID[i] = 0;
}

void JamEngine::clearZLevel(uint8_t Z_Level) {
  uint16_t i = 0;
  uint16_t LEVEL = Z_Level * DRAWABLES_PER_LEVEL;
  while (i < DRAWABLES_PER_LEVEL) {
    DRAWABLEPOOL[LEVEL + i] = nullptr;
    ++i;
  }
  CurrentMaxZIndexID[Z_Level] = 0;
}

void JamEngine::saveContext() {
  context_data t = ctx_stack.top();
  context_data copy(t);
  ctx_stack.push(copy);
}

void JamEngine::restoreContext() {

  context_data t = ctx_stack.top();
  ctx_stack.pop();
  t = ctx_stack.top();
}

void JamEngine::ctx_translate(float x, float y) {
  context_data &t = ctx_stack.top();
  t.transform_x = x;
  t.transform_y = y;
}

void JamEngine::ctx_change_color(int r, int g, int b) {
  context_data &t = ctx_stack.top();
  t.color_r = r;
  t.color_g = g;
  t.color_b = b;
}

void JamEngine::ctx_add_rotation(float rot) {
  context_data &t = ctx_stack.top();
  t.angle += rot;
}
// Debug
/*
#include <iostream>

void JamEngine::printDrawablepool(){
    std::cout << "POOL: " << Z_INDEX_LEVELS << " x " << DRAWABLES_PER_LEVEL <<
'\n'; for(uint8_t i = 0; i < Z_INDEX_LEVELS; ++i){ for(uint8_t j = 0; j <
DRAWABLES_PER_LEVEL; ++j){ std::cout << DRAWABLEPOOL[i*DRAWABLES_PER_LEVEL + j]
<< " ";
        }
        std::cout << '\n';
    }
    std::cout << "POOL PRINTED ===========" << '\n' << '\n';
}
*/
