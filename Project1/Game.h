#pragma once

#include "SDL2/SDL.h"
#include <vector>

struct Vector2
{
	float x;
	float y;
};

struct Ball
{
	Vector2 mBallPos;
	Vector2 mBallVel;
};

class Game
{
public:
	Game();

	bool Initialize();		// Intialize game
	void RunLoop();			// Runs game loop until game is over
	void Shutdown();		// shutdown game

private:
	// Helper functions for game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	std::vector<Ball> balls;

	// window created by SDL
	SDL_Window* mWindow;

	// renderer created by SDL
	SDL_Renderer* mRenderer;

	Uint32 mTicksCount;

	int mPaddleDir;		// player1 paddle direction
	int mPaddleDir2;	// player2 paddle direction

	// Game should continue to run
	bool mIsRunning;

	// player 1
	Vector2 mPaddlePos;
	// player 2
	Vector2 mPaddlePos2;

	Vector2 mBallPos;
	
	Vector2 mBallVel;

};
