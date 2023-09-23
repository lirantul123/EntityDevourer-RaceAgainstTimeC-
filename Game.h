#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <memory>
#include <vector>
#include "SDL2/SDL.h"
#include "holder.h"
#include "Player.h"

class Game {
public:
    Game();
    ~Game();

    bool Initialize();
    void RunLoop();
    void Shutdown();

private:
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    Player mPlayer;
    std::vector<SDL_Rect> mObjects; // Rectangles for objects
    bool mIsRunning;
    int eatenFigues = 0;
    int mTimeLimit;
    int mPlayerSpeed;
    int mTimeLeft;
    Uint32 mGameStartTime;

    // For the figures spawning
    Uint32 mGameStartTime2;
    Uint32 mCurrentTime;
    Uint32 mElapsedTime;
    Uint32 mLastObjectSpawnTime;

    std::vector<SDL_Rect> mObstacles; // Obstacles in the game
    //void UpdateObstacles();
    void CreateObstacle();

    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void StartScreen();
    void GameOver(bool win);
    void DisplayTitleScreen();
};

#endif // GAME_H
