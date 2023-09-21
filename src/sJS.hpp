#ifndef SMENU_H
#define SMENU_H

#include "engineModules/State.hpp"
#include "engineModules/eText.hpp"
#include "engineModules/Sprite.hpp"

class sJS : public State {
    public: 
        sJS();
        sJS(const sJS& orig);
        ~sJS();

        virtual void Init();
        virtual void Update();
        virtual void Exit();

    private:
        //eText PRESS_STARTO;
        Sprite MenuStartSprite;
        Sprite SplashSprite;
};

#endif