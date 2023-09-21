#include <iostream>
#include <stdio.h>

#include "EventManager.hpp"
#include "JamEngine.hpp"

#define BUTTON_DOWN SDL_JOYBUTTONDOWN
#define BUTTON_UP 1540

#define JOYSTICK_DEAD_ZONE 8000

EventManager::EventManager() {

  for (uint8_t i = 0; i < PspCtrlButtons::PSPKEYCOUNT; ++i)
    PSP_BUTTONS_STATE[i] = false;
}

EventManager::~EventManager() {}

void EventManager::launch() {
  SDL_Event event;

  SDL_GameController *gGameController =
      JamEngine::Instance()->getGameController();

  Uint8 cross = SDL_GameControllerGetButton(
      gGameController, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A);
  Uint8 circle = SDL_GameControllerGetButton(
      gGameController, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B);
  Uint8 square = SDL_GameControllerGetButton(
      gGameController, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X);
  Uint8 triangle = SDL_GameControllerGetButton(
      gGameController, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y);

  Uint8 up = SDL_GameControllerGetButton(
      gGameController, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP);
  Uint8 down = SDL_GameControllerGetButton(
      gGameController,
      SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN);
  Uint8 left = SDL_GameControllerGetButton(
      gGameController,
      SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT);
  Uint8 right = SDL_GameControllerGetButton(
      gGameController,
      SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

  if (!(right or left)) {
    int16_t xVal =
        SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_LEFTX);
    if (xVal < -JOYSTICK_DEAD_ZONE) {
      PSP_BUTTONS_STATE[PspCtrlButtons::LEFT] = true;
    }
    // Right of dead zone
    else if (xVal > JOYSTICK_DEAD_ZONE) {
      PSP_BUTTONS_STATE[PspCtrlButtons::RIGHT] = true;
    } else {

      PSP_BUTTONS_STATE[PspCtrlButtons::LEFT] = false;
      PSP_BUTTONS_STATE[PspCtrlButtons::RIGHT] = false;
    }

  } else {
    PSP_BUTTONS_STATE[PspCtrlButtons::LEFT] = left;
    PSP_BUTTONS_STATE[PspCtrlButtons::RIGHT] = right;
  }

  if (!(up or down)) {
    int16_t yVal =
        SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_LEFTY);

    if (yVal < -JOYSTICK_DEAD_ZONE) {
      PSP_BUTTONS_STATE[PspCtrlButtons::UP] = true;
    }
    // Above of dead zone
    else if (yVal > JOYSTICK_DEAD_ZONE) {
      PSP_BUTTONS_STATE[PspCtrlButtons::DOWN] = true;
    } else {
      PSP_BUTTONS_STATE[PspCtrlButtons::DOWN] = false;
      PSP_BUTTONS_STATE[PspCtrlButtons::UP] = false;
    }
  } else {
    PSP_BUTTONS_STATE[PspCtrlButtons::UP] = up;
    PSP_BUTTONS_STATE[PspCtrlButtons::DOWN] = down;
  }

  PSP_BUTTONS_STATE[PspCtrlButtons::CROSS] = cross;
  PSP_BUTTONS_STATE[PspCtrlButtons::CIRCLE] = circle;
  PSP_BUTTONS_STATE[PspCtrlButtons::SQUARE] = square;
  PSP_BUTTONS_STATE[PspCtrlButtons::TRIANGLE] = triangle;

  while (SDL_PollEvent(&event)) { // algun evento?

    if (event.type == SDL_QUIT) {
      std::cout << "EVENTO DE SALIDA!!!" << std::endl;

      JamEngine::Instance()->close();
    }

    /*if (event.type == SDL_JOYAXISMOTION) {
      std::cout << event.jaxis.value << std::endl;

      // X axis motion
      if (event.jaxis.axis == 0) {
        // Left of dead zone
        if (event.jaxis.value < -JOYSTICK_DEAD_ZONE) {
          PSP_BUTTONS_STATE[PspCtrlButtons::LEFT] = true;
        }
        // Right of dead zone
        else if (event.jaxis.value > JOYSTICK_DEAD_ZONE) {
          PSP_BUTTONS_STATE[PspCtrlButtons::RIGHT] = true;
        } else {

          PSP_BUTTONS_STATE[PspCtrlButtons::LEFT] = false;
          PSP_BUTTONS_STATE[PspCtrlButtons::RIGHT] = false;
        }
      }
      // Y axis motion
      else if (event.jaxis.axis == 1) {
        // Below of dead zone
        if (event.jaxis.value < -JOYSTICK_DEAD_ZONE) {
          PSP_BUTTONS_STATE[PspCtrlButtons::UP] = true;
        }
        // Above of dead zone
        else if (event.jaxis.value > JOYSTICK_DEAD_ZONE) {
          PSP_BUTTONS_STATE[PspCtrlButtons::DOWN] = true;
        } else {
          PSP_BUTTONS_STATE[PspCtrlButtons::DOWN] = false;
          PSP_BUTTONS_STATE[PspCtrlButtons::UP] = false;
        }
      }
    }*/
  }
}

bool EventManager::isAnyKeyPressed() {
  for (uint8_t i = 0; i < PspCtrlButtons::PSPKEYCOUNT; ++i) {
    if (PSP_BUTTONS_STATE[i]) {
      PSP_BUTTONS_STATE[i] = false;
      return true;
    }
  }

  return false;
}