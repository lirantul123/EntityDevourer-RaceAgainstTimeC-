#include <iostream>
#include <vector>
#include "SDL2/SDL.h"
#include <thread>
#define SLEEP(x) std::this_thread::sleep_for(std::chrono::seconds(x))

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

struct Vector2 {
    float x;
    float y;
    float z;
};

class Player {
public:
    Vector2 position;
    SDL_Rect rect;
    Player(int x, int y) {
        position.x = x;
        position.y = y;
        rect.w = 50;
        rect.h = 50;
    }

    void Move(int x, int y) {
        position.x += x;
        position.y += y;
        rect.x = static_cast<int>(position.x);
        rect.y = static_cast<int>(position.y);
    }
};

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

// Create Obstacles
void Game::CreateObstacle() {
    for (int i = 0; i < 3; ++i) {
        int x = rand() % SCREEN_WIDTH;
        int y = rand() % SCREEN_HEIGHT;
        SDL_Rect obstacle = { x, y, 30, 80 };
        mObstacles.push_back(obstacle);
    }
}


Game::Game()
        : mWindow(nullptr),
          mRenderer(nullptr),
          mPlayer(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
          mIsRunning(false),
          mTimeLimit(0),
          mPlayerSpeed(0),
          mTimeLeft(0) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

Game::~Game() {
    Shutdown();
}

bool Game::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    mWindow = SDL_CreateWindow("Entity Devourer: Race Against Time", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!mWindow) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
        return false;
    }

    mPlayer = Player(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    CreateObstacle();
    // Create initial objects (Rectangles)
//    for (int i = 0; i < 1; ++i) {// one for now
//        int x = rand() % SCREEN_WIDTH;
//        int y = rand() % SCREEN_HEIGHT;
//        SDL_Rect objectRect = { x, y, 20, 20 };
//        mObjects.push_back(objectRect);
//    }

    mIsRunning = true;
    return true;
}

void Game::Shutdown() {
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}


void Game::DisplayTitleScreen() {
    std::cout << "-----------------------------------------------------" << std::endl;
    std::cout << "        Entity Devourer: Race Against Time" << std::endl;
    std::cout << "-----------------------------------------------------" << std::endl;
    std::cout << "Goal: Eat as many figures as you can within the time limit." << std::endl;
    std::cout << "Figures will appear randomly every second." << std::endl;
    std::cout << "Be quick and precise!" << std::endl;
    std::cout << "-----------------------------------------------------" << std::endl;
    std::cout << "Press ENTER to start..." << std::endl;

    std::cin.ignore(); // Wait for user input (ENTER key)
}

void Game::StartScreen() {
    DisplayTitleScreen();
    std::cout << "Enter the time limit (in seconds): ";
    std::cin >> mTimeLimit;
    
    std::cout << "Enter the player speed: ";
    std::cin >> mPlayerSpeed;

    //mGameStartTime2 = SDL_GetTicks();
    
    mTimeLeft = mTimeLimit;

    mIsRunning = true;
}

void Game::GameOver(bool win) {
    if (win) {
        std::cout << "Congratulations! You won!" << std::endl;
    } else {
        std::cout << "You have only eaten " << eatenFigues << "/" << mTimeLimit << "." << std::endl;
        std::cout << "Game Over! You lost!\n" << std::endl;
    }

    mIsRunning = false;
}

void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            mIsRunning = false;
            std::cout << "Closed by 'X' button." << std::endl;
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    mIsRunning = false;
                    std::cout << "Closed by 'Escape' button." << std::endl;
                    break;
                case SDLK_UP:
                case SDLK_w:
                    mPlayer.Move(0, -mPlayerSpeed);
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    mPlayer.Move(0, mPlayerSpeed);
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    mPlayer.Move(-mPlayerSpeed, 0);
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    mPlayer.Move(mPlayerSpeed, 0);
                    break;
                default:
                    break;
            }
        }
    }
}

void Game::UpdateGame() {
    // Calculate elapsed time in seconds
    Uint32 currentTime = SDL_GetTicks();
    float elapsedTime = (currentTime - mGameStartTime) / 1000.0f;

    // Check collisions with the Figures- FIX IT
    auto it = mObjects.begin();
    while (it != mObjects.end()) {
        SDL_Rect& objectRect = *it;
        float dx = mPlayer.position.x - (objectRect.x + objectRect.w / 2);
        float dy = mPlayer.position.y - (objectRect.y + objectRect.h / 2);
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance < mPlayer.rect.w / 2 + objectRect.w / 2) {
            // Collision with figure
            it = mObjects.erase(it); // Remove the figure from the vector
            eatenFigues++;
        } else {
            ++it; // Move to the next figure
        }
    }

    // Check collisions with the Obstacles
    for (auto& obstacle : mObstacles) {
        // Calculate the half-widths and half-heights of the player and obstacle
        float playerHalfWidth = mPlayer.rect.w / 2.0f;
        float playerHalfHeight = mPlayer.rect.h / 2.0f;
        float obstacleHalfWidth = obstacle.w / 2.0f;
        float obstacleHalfHeight = obstacle.h / 2.0f;

        // Calculate the centers of the player and obstacle
        float playerCenterX = mPlayer.position.x + playerHalfWidth;
        float playerCenterY = mPlayer.position.y + playerHalfHeight;
        float obstacleCenterX = obstacle.x + obstacleHalfWidth;
        float obstacleCenterY = obstacle.y + obstacleHalfHeight;

        // Calculate the distances between the centers of the player and obstacle
        float deltaX = playerCenterX - obstacleCenterX;
        float deltaY = playerCenterY - obstacleCenterY;

        // Calculate the minimum distances for no overlap
        float minDistanceX = playerHalfWidth + obstacleHalfWidth;
        float minDistanceY = playerHalfHeight + obstacleHalfHeight;

        // If the player is closer to the obstacle in both dimensions, there's a collision
        if (std::abs(deltaX) < minDistanceX && std::abs(deltaY) < minDistanceY) {
            // Calculate the overlap in both dimensions
            float overlapX = minDistanceX - std::abs(deltaX);
            float overlapY = minDistanceY - std::abs(deltaY);

            // Push the player away from the obstacle based on the overlap
            if (overlapX < overlapY) {
                // X-axis collision, resolve horizontally
                if (deltaX < 0) {
                    mPlayer.position.x -= overlapX;
                } else {
                    mPlayer.position.x += overlapX;
                }
            } else {
                // Y-axis collision, resolve vertically
                if (deltaY < 0) {
                    mPlayer.position.y -= overlapY;
                } else {
                    mPlayer.position.y += overlapY;
                }
            }

            // Update the player's rect to match the new position
            mPlayer.rect.x = static_cast<int>(mPlayer.position.x);
            mPlayer.rect.y = static_cast<int>(mPlayer.position.y);
        }
    }

    //UpdateObstacles(); // Add this line to update obstacles

    // Update the time left
    mTimeLeft = std::max(0, mTimeLimit - static_cast<int>(elapsedTime));

    // Check for game over conditions
    if (mTimeLeft <= 0) {
        GameOver(false); // Game over due to time limit
    }
    // Or If he has eaten all before time limit

    
    // Calculate elapsed time in seconds
    mCurrentTime = SDL_GetTicks();
    mElapsedTime = mCurrentTime - mGameStartTime;

    // Check if it's time to spawn a new object (e.g., every 1 second)
    Uint32 timeSinceLastSpawn = mCurrentTime - mLastObjectSpawnTime;
    if (timeSinceLastSpawn >= 1000) { // Spawn a new object every 1000 milliseconds (1 second)
        int x = rand() % SCREEN_WIDTH;
        int y = rand() % SCREEN_HEIGHT;
        SDL_Rect objectRect = {x, y, 20, 20};
        
        // Check if the new figure overlaps with any obstacle
        bool overlapsWithObstacle = false;
        for (const auto& obstacle : mObstacles) {
            if (SDL_HasIntersection(&objectRect, &obstacle)) {
                overlapsWithObstacle = true;
                break; // No need to check further, we know it overlaps
            }
        }
        if (!overlapsWithObstacle) {
            // No overlap with obstacles, so add it to mObjects
            mObjects.push_back(objectRect);
        }

        // Update the last spawn time
        mLastObjectSpawnTime = mCurrentTime;
    }
}

void Game::GenerateOutput() {
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255); // Clear the screen to black
    SDL_RenderClear(mRenderer);

    // Render player
    SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255); // Red color - for player
    SDL_RenderFillRect(mRenderer, &mPlayer.rect);

    // Render objects
    SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255); // Green color - for figures
    for (const auto& objectRect : mObjects) {
        SDL_RenderFillRect(mRenderer, &objectRect);
    }
    
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255); // White color - for obstacles
    for (const auto& obstacle : mObstacles) {
        SDL_RenderFillRect(mRenderer, &obstacle);
    }

    SDL_RenderPresent(mRenderer);
}

void Game::RunLoop() {
    StartScreen();

    mGameStartTime = SDL_GetTicks();
    mGameStartTime2 = SDL_GetTicks();

    // Game loop
    Uint32 lastFrameTime = SDL_GetTicks();
    Uint32 deltaTime = 16; // 60 FPS initially
    while (mIsRunning) {
        ProcessInput();

        Uint32 currentFrameTime = SDL_GetTicks();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
        
        // Now the mTimeLimit- represents the figures to eat per second
        if (eatenFigues >= mTimeLimit) {
            GameOver(true); // Player won
        } else if (mTimeLeft <= 0) {
            GameOver(false); // Player lost due to time limit
        }

        UpdateGame();
        GenerateOutput();
        SDL_Delay(16); // Cap frame rate (approximately 60 FPS)
    }
}

int main() {
    Game* myGame = new Game();
    if (myGame->Initialize()) {
        myGame->RunLoop();
    }
    delete myGame;
    return 0;
}

