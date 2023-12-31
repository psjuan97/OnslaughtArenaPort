#include "engineModules/AssetManager.hpp"
#include "engineModules/JamEngine.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "engineModules/Tilemap.hpp"

#include "engineModules/EventManager.hpp"
#include "engineModules/audio.h" // Pruebas audio
#include "engineModules/eClock.hpp"

#include "engineModules/AnimatedSprite.hpp"
#include "sGame.hpp"
#include "sJS.hpp"
#include "sMenu.hpp"

#include "ASSETS_IDs.hpp"

#include "engineModules/StateMachine.hpp"

#ifdef PSP

#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspkernel.h>

#define VERS 1 // Talk about this
#define REVS 0

PSP_MODULE_INFO("SDL IS HERE", 0, VERS, REVS);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1 * 1024);

#endif

#define BUTTON_DOWN SDL_JOYBUTTONDOWN
#define BUTTON_UP 1540

int main() {

  JamEngine *JAM = JamEngine::Instance();
  JAM->Init();

  AssetManager *Assets = AssetManager::Instance();

  //Assets->loadFont(DEFAULT_FONT, "assets/font.ttf");

  eText::setFont(Assets->getFont(DEFAULT_FONT));

  //////////////////////////
  // Prueba musica
  //////////////////////////
  //eMusic *musica = Assets->loadMusic(SAMPLE_MUSIC, "assets/theme.wav");
  //musica->playAsSound();

  StateMachine *mashin = StateMachine::Instance();
  mashin->setState(new sJS);

  while (JAM->isOpen()) {
    JAM->Clear();

    EventManager::Instance()->launch();

    mashin->Update();

    JAM->Draw();
    // SDL_Delay(5);
  }

  // JAM->~JamEngine();

#ifdef PSP
  sceKernelExitGame();
#endif

  return 0;
}