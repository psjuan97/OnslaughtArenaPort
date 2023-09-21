# OnslaughtArenaPort
An C++ (SDL + Duktape) port of the original game [Onslaught! Arena](https://github.com/lostdecade/onslaught_arena)



# Build for Psvita
```
mkdir build
cmake ..
make -j
```

# Controller
* Use Left Joystic or DPAD to move the hero.
* Fire in direction using Cross, Square, Circle, Triangle.

# Bugs - Problem
* No Sound
* Menu no fully working
* No Text rendering

# Technical Stack 
The main idea of this port is not to recreate line by a line the code in JS from to original game to C++. 

The motivation for this, is to use a javascript interpreter execute the code of the game the minimun posible  and open the door to migrate more games in a fast way. TThe selected interpreter is [Duktape](https://duktape.org/), which together with SDL has managed to execute the game in a fluid and playable way. 

Modifications to the original source code have been necessary to simplify game logic and thus increase performance or discard functionalities that do not come from outside the browser.
