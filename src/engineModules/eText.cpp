#include "eText.hpp"
#include "JamEngine.hpp"
#define DEFAULT_FONT 0
#include <stdio.h>

eFont *eText::font = nullptr;
SDL_Renderer *eText::Renderer = nullptr;

void eText::setFont(eFont *font) { eText::font = font; }

eText::eText(int x, int y) {
  setPosition(x, y);
  texture = nullptr;
}

void eText::setPosition(int x, int y) {
  posX = x;
  posY = y;
}

void eText::setString(std::string str) {

  SDL_Color black = {0, 0, 0, 0};
  SDL_Color yellow = {0, 255, 255, 0};

  SDL_Surface *surfaceMessage =
      TTF_RenderText_Solid(font->getSDLFont(), str.c_str(), black);
  if (surfaceMessage != nullptr) {
    texture = SDL_CreateTextureFromSurface(
        Renderer, surfaceMessage); // now you can convert it into a texture
    if (texture == nullptr) {
      printf("ERROR CREATING texture ! SDL Error: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(surfaceMessage);
  } else {
    printf("ERROR CREATING text SURFACE ! SDL Error: %s\n", SDL_GetError());
  }
}

void eText::Draw() {
  if (texture != nullptr) {
    SDL_Rect Message_rect;
    Message_rect.x = posX;
    Message_rect.y = posY;
    Message_rect.h = 60;
    Message_rect.w = 60;

    SDL_QueryTexture(texture, NULL, NULL, &Message_rect.w, &Message_rect.h);

    JamEngine::Instance()->drawTexture(texture, NULL, &Message_rect, 0);
  }
}

// free texture
eText::~eText() {
  if (texture != nullptr) {
    // liberamos lo anterior
    SDL_DestroyTexture(texture);
    texture = nullptr;
  }
}