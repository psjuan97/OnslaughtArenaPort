#include "Sprite.hpp"

SDL_Renderer *Sprite::Renderer = nullptr;

Sprite::Sprite() : Texture(nullptr) {
  Size_Pos.h = 0;
  Size_Pos.w = 0;
  Size_Pos.x = 0;
  Size_Pos.y = 0;

  slice = new SDL_Rect;

  //slice = nullptr;
}

Sprite::Sprite(const Sprite &orig) {
  Texture = orig.Texture;
  Size_Pos = orig.Size_Pos;
}

void Sprite::operator=(const Sprite &orig) {
  Texture = orig.Texture;
  Size_Pos = orig.Size_Pos;
}

Sprite::~Sprite() {}

void Sprite::setTexturePosition(float x, float y) {
    slice->x = x;
    slice->y = y;

}
void Sprite::setTextureSize(float w, float h) {
    slice->w = w;
    slice->h = h;
}

void Sprite::setPosition(float x, float y) {
  Size_Pos.x = x;
  Size_Pos.y = y;
}

void Sprite::setX(float x) { Size_Pos.x = x; }

void Sprite::setY(float y) { Size_Pos.y = y; }

void Sprite::setSize(float w, float h) {
  Size_Pos.w = w;
  Size_Pos.h = h;
}

void Sprite::Draw() {

  JamEngine::Instance()->drawTexture(Texture, slice, &Size_Pos,0);
}