#ifndef JAMENGINE_H
#define JAMENGINE_H

#include "AssetManager.hpp"
#include "Sprite.hpp"
#include "Tilemap.hpp"
#include "eText.hpp"
#include "eView.hpp"

#include "Drawable.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "audio.h" // Audio engine

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 544

#include <sstream>
#include <stack>

class text_data {
public:
  std::string text;
  SDL_Rect d;
  int color_r;
  int color_g;
  int color_b;
};

class draw_data {
public:
  SDL_Rect d;
  SDL_Rect s;
  float angle;
  int color_r;
  int color_g;
  int color_b;
};

class context_data {
public:
  float transform_x;
  float transform_y;
  float angle;
  int color_r;
  int color_g;
  int color_b;
};

template <typename T> std::string to_string(const T &value) {
  std::ostringstream oss;
  oss << value;
  return oss.str();
}
#define Z_INDEX_LEVELS 11
#define DRAWABLES_PER_LEVEL 35

class JamEngine {
  friend class AssetManager;

  // METHODS
public:
  static JamEngine *Instance() {
    static JamEngine instance;
    return &instance;
  }

  ~JamEngine();

  // 2d context
  void saveContext();
  void restoreContext();
  void ctx_translate(float x, float y);
  void ctx_add_rotation(float rot);
  void ctx_change_color(int r, int g, int b);

  bool Init();
  bool isOpen();
  void close();
  void Clear();
  void Draw();
  void Update();
  void Render();
  int getTicks();

  void drawTexture(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dst,
                   float rot);
  void drawRect(SDL_Rect *dst, int color_r, int color_g, int color_b);
  
  void drawText(std::string text, SDL_Rect *dst, int color_r, int color_g, int color_b);



  // CAMARA
  void setView(eView v);
  void moveView(int x, int y);
  void setDrawable_ZIndex(Drawable *D, uint8_t Z_Index);
  void setDrawable_ZIndex(Drawable *D, uint8_t Z_Index, uint8_t Position);
  void clearAllDrawables();
  void clearZLevel(uint8_t Z_Level);
  void setBuzz(bool buzz) {
    if (buzz) {
      buzzTimer = 10;
    }

    isBuzz = buzz;
  };
  // void printDrawablepool();
  void DrawUno(uint16_t n, uint16_t p);
  SDL_GameController *getGameController() { return gGameController; }

  void insertDrawPool(int type, draw_data &data) {
    int pos = draw_pool_size[type];
    draw_pool[type][pos] = data;
    draw_pool_size[type]++;
    if (pos >= DRAWABLES_PER_LEVEL) {
      std::cout << "nos quedmaos sin espacio" << std::endl;
    }
  }

  void insertDrawRectPool(draw_data &data) {
    int pos = rect_pool_size;
    rect_pool[pos] = data;
    rect_pool_size++;
    if (pos >= DRAWABLES_PER_LEVEL) {
      std::cout << "nos quedmaos sin espacio para rectangulos" << std::endl;
    }
  }


  void insertDrawTextPool(text_data &data) {
    int pos = text_pool_size;
    text_pool[pos] = data;
    text_pool_size++;
    if (pos >= DRAWABLES_PER_LEVEL) {
      std::cout << "nos quedmaos sin espacio para texto" << std::endl;
    }
  }
  std::stack<context_data> ctx_stack;

private:
  JamEngine();
  JamEngine(const JamEngine &orig) = delete;
  void operator=(const JamEngine &orig) = delete;

private:
  Drawable *DRAWABLEPOOL[Z_INDEX_LEVELS * DRAWABLES_PER_LEVEL];
  // por cada imagen sera necesario un pool, con 10 nos sobra
  draw_data draw_pool[6][DRAWABLES_PER_LEVEL];
  int draw_pool_size[6] = {0, 0, 0, 0, 0, 0};

  draw_data rect_pool[DRAWABLES_PER_LEVEL];
  int rect_pool_size = 0;

  text_data text_pool[DRAWABLES_PER_LEVEL];
  int text_pool_size = 0;





  uint8_t CurrentMaxZIndexID[Z_INDEX_LEVELS];

  SDL_Renderer *Renderer;
  SDL_Window *Window;
  SDL_Joystick *GameController;
  SDL_GameController *gGameController = NULL;

  eView camera;

  int buzz;
  bool isBuzz;
  int buzzTimer;

  int a = 0;
  int b = 0;
};

#endif