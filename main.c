/* ManySnakes by Danielle Raine
 * Start Date: June 8th, 2024
 */

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>


int main(void)
{
	// print game description, github url
	SDL_Log("%s\n", ManySnakes_DESCRIPTION);
	SDL_Log("Homepage Url %s\n", ManySnakes_HOMEPAGE_URL);
	
	// print game version
	SDL_Log("Game Version ");
	switch (ManySnakes_VERSION_PHASE)
	{
		case 0: // is release version
			printf("Release %d.%d.%d\n", ManySnakes_VERSION_MAJOR, ManySnakes_VERSION_MINOR, ManySnakes_VERSION_PATCH);
			break;
		case 1: // is release preview version
			printf("Release %d.%d.0-preview.%d\n", ManySnakes_VERSION_MAJOR, ManySnakes_VERSION_MINOR, ManySnakes_VERSION_PATCH);
			break;
		case 2: // is pre-alpha version
			printf("pre-alpha.%d\n", ManySnakes_VERSION_PATCH);
			break;
		case 3: // is alpha version
			printf("alpha-%d.%d\n", ManySnakes_VERSION_MINOR, ManySnakes_VERSION_PATCH);
			break;
		case 4: // is beta version
			printf("beta-%d.%d.%d\n", ManySnakes_VERSION_MAJOR, ManySnakes_VERSION_MINOR, ManySnakes_VERSION_PATCH);
			break;
		default: // what happened here???
			printf("Unknown???\n");
	}

	// print c version
	SDL_Log("C Version %d\n", __STDC_VERSION__ );
	
	// sdl version structs
	SDL_version compiled;
	SDL_version linked;
	
	// print compiled, linked sdl version
	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	SDL_Log("Compiled against SDL version %u.%u.%u\n", compiled.major, compiled.minor, compiled.patch);
	SDL_Log("Linking against SDL version %u.%u.%u\n", linked.major, linked.minor, linked.patch);


	// initialize sdl. if error, return 
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_EVENTS) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
		return 1;
	}
	// register sdl shutdown on program closure
	atexit(SDL_Quit);


	// diagonal, horizontal, vertical dpi
	float ddpi, hdpi, vdpi;
	// get display dpi at display 0
	SDL_GetDisplayDPI(1, &ddpi, &hdpi, &vdpi);
	SDL_Log("%f %f %f", ddpi, hdpi, vdpi);

	// create window
	SDL_Window *window = SDL_CreateWindow("ManySnakes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, SDL_WINDOW_RESIZABLE);
	// if window doesn't exist, print error and return
	if (!window)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
		return 1;
	}
	// window options
	SDL_SetWindowResizable(window, SDL_FALSE);

	// create renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	// if rendere dne, print error, destroy window and return
	if (!renderer)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
		SDL_DestroyWindow(window);
		return 1;
	}


	SDL_Event event;
	bool isRunning = true;
	// main loop	
	while (isRunning)
	{
		// poll until all events handled
		while(SDL_PollEvent(&event))
		{
			if (SDL_QUIT == event.type) // if quit, stop event poll and exit main loop
			{
				isRunning = false;
				break;
			}
			else if (SDL_KEYDOWN == event.type) // if key pressed down, handle it!
			{
				// log the name of pressed key
				SDL_Log("%s", SDL_GetKeyName(event.key.keysym.sym));

				if (event.key.keysym.sym == SDLK_f) // key f
				{
					SDL_Log("Hewwo!!");

					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red
					SDL_RenderDrawPoint(renderer, 0, 0);

					// change color to white
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
					SDL_RenderDrawLine(renderer, 1, 1, 990, 990);
					
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red
					SDL_RenderDrawPoint(renderer, 999, 999);
					
					SDL_RenderPresent(renderer);
				}
			}
		}

		// update game state, draw current frame

	} // main loop end


	// cleanup
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	// print sdl subsystem initialization flags
	// printf("%X\n\n", SDL_WasInit(SDL_INIT_EVERYTHING));

	return 0;
}
