#include <iostream>
#include <vector>
#include "SDL2/SDL.h"
#include <thread>
#define SLEEP(x) std::this_thread::sleep_for(std::chrono::seconds(x))

int main() {
    
    std::unique_ptr<Game> myGame = std::make_unique<Game>();
    
    if (myGame->Initialize()) {
        myGame->RunLoop();
    }
    
    return 0;
}

