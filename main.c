/* ManySnakes by Danielle Raine
 * Start Date: June 8th, 2024
 */

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>


int main(void)
{
	// print game description, github url
	printf("Game Descripton: %s\n", ManySnakes_DESCRIPTION);
	printf("Homepage Url: %s\n", ManySnakes_HOMEPAGE_URL);
	
	// print game version
	printf("Game Version: "); 
	
	switch (ManySnakes_VERSION_PHASE)
	{
		case 0:
			printf("Release %d.%d.%d\n", ManySnakes_VERSION_MAJOR, ManySnakes_VERSION_MINOR, ManySnakes_VERSION_PATCH);
			break;
		case 1:
			printf("Release %d.%d.0-preview.%d\n", ManySnakes_VERSION_MAJOR, ManySnakes_VERSION_MINOR, ManySnakes_VERSION_PATCH);
			break;
		case 2:
			printf("pre-alpha.%d\n", ManySnakes_VERSION_PATCH);
			break;
		case 3:
			printf("alpha-%d.%d\n", ManySnakes_VERSION_MINOR, ManySnakes_VERSION_PATCH);
			break;
		case 4:
			printf("beta-%d.%d.%d\n", ManySnakes_VERSION_MAJOR, ManySnakes_VERSION_MINOR, ManySnakes_VERSION_PATCH);
			break;
		default:
			printf("Unknown???\n");
	}

	//printf("Game Version: %d.%d.%d", ManySnakes_VERSION_MAJOR, ManySnakes_VERSION_MINOR, ManySnakes_VERSION_PATCH);
	printf("C Version: %d\n", __STDC_VERSION__ );
	// sdl version structs
	SDL_version compiled;
	SDL_version linked;
	// print compiled, linked sdl version
	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	SDL_Log("Compiled against SDL version %u.%u.%u\n", compiled.major, compiled.minor, compiled.patch);
	SDL_Log("Linking against SDL version %u.%u.%u\n", linked.major, linked.minor, linked.patch);

	// register sdl shutdown on program closure
	atexit(SDL_Quit);

	// initialize sdl. if error, return 
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_EVENTS) != 0)
	{
		printf("%s", SDL_GetError());
		return 1;
	}

	// print sdl subsystem initialization flags
	printf("%X\n\n", SDL_WasInit(SDL_INIT_EVERYTHING));

	return 0;
}
