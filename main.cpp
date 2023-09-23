#include <iostream>
#include <vector>
#include "SDL2/SDL.h"
#include <thread>
#define SLEEP(x) std::this_thread::sleep_for(std::chrono::seconds(x))

int main() {
    Game* myGame = (Game*)malloc(sizeof(Game));
    if (myGame->Initialize()) {
        myGame->RunLoop();
    }
    delete myGame;
    return 0;
}

