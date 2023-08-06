#include "Game.h"


const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
:mWindow(nullptr),
mRenderer(nullptr),
mIsRunning(true),
mTicksCount(0),
mPaddleDir(0),
mPaddleDir2(0)
{

}

bool Game::Initialize()
{
	unsigned int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Undable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(
		"Game Dev Project 1",				// title of window
		100,								// x position
		100,								// y position
		1024,								// width
		768,								// length
		0);									// flags

	if (!mWindow)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(
		mWindow,												// window object
		-1,														// let SDL choose the window	works for when we only have one window
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	// flags for SDL RENDERER Accelerated allows prog to use graphic hardware
																// PRESENTVSYNC enables Vsync

	if (!mRenderer)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f / 2.0f;
	mPaddlePos2.x = 1024.0f - 10.0f;
	mPaddlePos2.y = 768.0f / 2.0f;
	Ball addBall;							// object of ball struct 
	for (int x = 0; x < 3; x++)				// add different balls to balls vector list
	{
		addBall.mBallPos.x = 1024.0f / 2.0f;
		addBall.mBallPos.y = 768.0f / 2.0f;
		addBall.mBallVel.x = -200.0f+(x*30);
		addBall.mBallVel.y = 235.0f;

		balls.push_back(addBall);
	}
	

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;

	// While therer are still events in the queue
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			//Handle different events
		case SDL_QUIT:
			mIsRunning = false;
			break;

		}

		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_ESCAPE])
		{
			mIsRunning = false;
		}
		mPaddleDir = 0;
		mPaddleDir2 = 0;
		if (state[SDL_SCANCODE_W])
		{
			mPaddleDir -= 1;
		}
		if (state[SDL_SCANCODE_S])
		{
			mPaddleDir += 1;
		}
		// player 2 paddle
		if (state[SDL_SCANCODE_UP])
		{
			mPaddleDir2 -= 1;
		}
		if (state[SDL_SCANCODE_DOWN])
		{
			mPaddleDir2 += 1;
		}
	}
}

void Game::UpdateGame()
{
	// ensures atleast 16 ms has passed between frames
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;
	// Delta Time is the difference in ticks from the last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();

	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// TODO Update objects in game world as function of Detla time
	// paddle 1 collsion
	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
		// Make surte paddle doesn't move off screen!
		if (mPaddlePos.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos.y = 768 - paddleH / 2.0f - thickness;
		}
	}
	// paddle 2 collsion
	if (mPaddleDir2 != 0)
	{
		mPaddlePos2.y += mPaddleDir2 * 300.0f * deltaTime;
		// Make surte paddle doesn't move off screen!
		if (mPaddlePos2.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos2.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos2.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos2.y = 768 - paddleH / 2.0f - thickness;
		}
	}

	for (int x = 0; x < balls.size(); x++)
	{
		balls[x].mBallPos.x += balls[x].mBallVel.x * deltaTime;
		balls[x].mBallPos.y += balls[x].mBallVel.y * deltaTime;

		float diff = mPaddlePos.y - balls[x].mBallPos.y;
		float diff2 = mPaddlePos2.y - balls[x].mBallPos.y;
		// Take absolute value of difference
		diff = (diff > 0.0f) ? diff : -diff;
		diff2 = (diff2 > 0.0f) ? diff2 : -diff2;

		// collision checks
		if (balls[x].mBallPos.y <= thickness && balls[x].mBallVel.y < 0.0f)
		{
			balls[x].mBallVel.y *= -1;
		}
		if (balls[x].mBallPos.y >= 768 - thickness && balls[x].mBallVel.y > 0.0f)
		{
			balls[x].mBallVel.y *= -1;
		}
		// right wall collision
		/*
		if (mBallPos.x >= 1024.0f - thickness && mBallVel.x > 0.0f)
		{
			mBallVel.x *= -1;
		}
		*/

		if (
			// Our y-differnece is small enough
			diff <= paddleH / 2.0f &&
			// Ball is at the correct x-position
			balls[x].mBallPos.x <= 25.0f && balls[x].mBallPos.x >= 20.0f &&
			// The ball is moving to the left
			balls[x].mBallVel.x < 0.0f)
		{
			balls[x].mBallVel.x *= -1.0f;
		}
		if (
			// our y-differnece is small enough
			diff2 <= paddleH / 2.0f &&
			// Ball is at the correct x-position
			balls[x].mBallPos.x >= 1004.0f && balls[x].mBallPos.x >= 99.0f &&
			// The ball is movinf to the right
			balls[x].mBallVel.x > 0.0f)
		{
			balls[x].mBallVel.x *= -1.0f;
		}
		//	if ball makes it passed paddle
		if (balls[x].mBallPos.x < 0 || balls[x].mBallPos.x > 1024)
		{
			Shutdown();
		}
	}


}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(		// set color
		mRenderer,				// renderer object
		0,						// R
		0,						//	G
		255,					// B
		255);					// alpha

	SDL_RenderClear(mRenderer);	// clear back buffer to the currernt draw color
	
	// Drawing 
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255); 
	SDL_Rect wall{
		0,				// top right
		0,				//Top left
		1024,			// width
		thickness };	// height

	SDL_RenderFillRect(mRenderer, &wall);

	// draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// draw right wall
	/*
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);
	*/


	SDL_SetRenderDrawColor(mRenderer,255,0,255,255);		// player 1 paddle color

	// The paddle
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};

	SDL_RenderFillRect(mRenderer, &paddle);

	SDL_SetRenderDrawColor(mRenderer, 0, 255, 200, 255);	// player 2 paddle color

	paddle.x = static_cast<int>(mPaddlePos2.x- thickness);
	paddle.y = static_cast<int>(mPaddlePos2.y - paddleH / 2);

	SDL_RenderFillRect(mRenderer, &paddle);

	SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);		// ball color
	for (int x = 0; x < balls.size(); x++)
	{
		// The ball
		SDL_Rect ball{
			static_cast<int>(balls[x].mBallPos.x - thickness / 2),
			static_cast<int>(balls[x].mBallPos.y - thickness / 2),
			thickness,
			thickness };

		SDL_RenderFillRect(mRenderer, &ball);
	}


	SDL_RenderPresent(mRenderer);	// swap front and back bufferes 
}

void Game::Shutdown()
{
	SDL_DestroyWindow(mWindow);				// destroy window
	SDL_DestroyRenderer(mRenderer);			// destroy renderer
	SDL_Quit();								// exit program
}