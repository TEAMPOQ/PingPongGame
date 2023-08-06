// Project:     GameDevProject1.cpp
// Author:      Carlos Rodriguez
// Date:        9/10/2022
// Purpose:     Intro To Game Dev


#include "Game.h"

#define main SDL_main    // Original line. Renames main(){} to SDL_main(){}. 
#define main main        // Added line. Undo the renaming.

int main(int argc, char** argv)
{
	Game game;					// create object game
	bool success = game.Initialize();
	if (success)
	{
		game.RunLoop();
	}
	game.Shutdown();
	return 0;
}
