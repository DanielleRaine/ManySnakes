/* ManyS  nakes by Danielle Raine
 * Start Date: June 8th, 2024
 */

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
//#include <SDL2/SDL_timer.h>


int main(void)
{
	typedef struct SnakeNode
	{
		struct SnakeNode *next;
		int xPos;
		int yPos;
	} SnakeNode;


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Print the game's description, Github URL, and version numbers of the game, C, and SDL.
	 */

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
	SDL_Log("C Version %ld\n", __STDC_VERSION__ );
	
	// sdl version structs
	SDL_version compiled;
	SDL_version linked;
	
	// print compiled, linked sdl version
	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	SDL_Log("Compiled against SDL version %u.%u.%u\n", compiled.major, compiled.minor, compiled.patch);
	SDL_Log("Linking against SDL version %u.%u.%u\n", linked.major, linked.minor, linked.patch);


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Initialize SDL and register SDL_Quit at exit.
	 */

	// initialize sdl. if error, return 
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
		return 1;
	}

	// register sdl shutdown on program closure
	atexit(SDL_Quit);


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Get display and window bounds, create a window and renderer.
	 */

	// display size struct
	SDL_Rect displayBounds;
	// get display bounds. if error, return
	if (SDL_GetDisplayBounds(0, &displayBounds) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
		return 1;
	}

	// window dimensions
	const int WINDOW_WIDTH = displayBounds.w * 2 / 3;
	const int WINDOW_HEIGHT = displayBounds.h * 2 / 3;

	// create window
	SDL_Window *window = SDL_CreateWindow("ManySnakes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	// if window doesn't exist, print error and return
	if (!window)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
		return 1;
	}

	// create renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	// if render doesn't exist, print error, destroy window and return
	if (!renderer)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
		SDL_DestroyWindow(window);
		return 1;
	}

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Create the player's snake and seed rand.
	 */

	SnakeNode *playerHead = malloc(sizeof(SnakeNode));
	playerHead->next = NULL;
	playerHead->xPos = WINDOW_WIDTH / 2;
	playerHead->yPos = WINDOW_HEIGHT / 2;

	SnakeNode *playerCurNode = playerHead;

	for (int i = 0; i < 2; i++)
	{
		int xPosNext = playerCurNode->xPos;
		int yPosNext = playerCurNode->yPos + 24;

		playerCurNode->next = malloc(sizeof(SnakeNode));
		playerCurNode = playerCurNode->next;
		playerCurNode->next = NULL;
		playerCurNode->xPos = xPosNext;
		playerCurNode->yPos = yPosNext;
	}
	
	srand(SDL_GetTicks());

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Main loop, poll for events!
	 */

	char playerDirection = 'u';
	//Uint64 timeLastMove = SDL_GetTicks64();
	Uint64 timeNextMove = SDL_GetTicks64() + 500;

	SDL_Event event;
	bool isRunning = true;
	// main loop	
	while (isRunning)
	{
		// clear frame
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		// set the earliest time the next frame occurs
		Uint64 timeNextFrame = SDL_GetTicks64() + (1000 / 60);
		
		// poll events
		while (SDL_PollEvent(&event))
		{
			if (SDL_QUIT == event.type) // if quit, stop event poll and exit main loop
			{
				isRunning = false;
				break;
			}
			else if (SDL_KEYDOWN == event.type) // if key pressed down, handle it!
			{
				SDL_Keycode pressedKey = event.key.keysym.sym;

				// log the name of pressed key
				SDL_Log("%s", SDL_GetKeyName(pressedKey));

				if (pressedKey == SDLK_RIGHT && playerDirection != 'l') // pressed right key, skip if direction is left
				{
					playerDirection = 'r'; // right
				}
				else if (pressedKey == SDLK_UP && playerDirection != 'd') // pressed up key, skip if direction is down
				{
					playerDirection = 'u'; // up
				}
				else if (pressedKey == SDLK_LEFT && playerDirection != 'r') // pressed left key, skip if direction is right
				{
					playerDirection = 'l'; // left
				}
				else if (pressedKey == SDLK_DOWN && playerDirection != 'u') // pressed down key, skip if direction is up
				{
					playerDirection = 'd'; // down
				}
				//else if (event.key.keysym.sym == SDLK_f) // key f
				//{
				//	// draw a random box on a snake space
				//	SDL_Rect drawRect = {rand() % 40 * WINDOW_WIDTH / 40, rand() % 30 * WINDOW_HEIGHT / 30, WINDOW_WIDTH / 40, WINDOW_HEIGHT / 30};
				//	SDL_Log("Rect at %d, %d with %d, %d.", drawRect.x, drawRect.y, drawRect.w, drawRect.h);
				//	if (SDL_SetRenderDrawColor(renderer, rand() % 256, rand() % 256, rand() % 256, 255) || SDL_RenderFillRect(renderer, &drawRect))
				//	{
				//		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
				//		SDL_DestroyRenderer(renderer);
				//		SDL_DestroyWindow(window);
				//		return 1;
				//	
				//	}
				//}
			}

		}

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		 * Player's snake moves every 1/2 of a second, so if current time is greater than or equal to the next
		 * time the snake is supposed to move, update the position of the snake depending on the direction 
		 * that the player pressed.
		 */

		Uint64 timeNow = SDL_GetTicks64();

		if (timeNow >= timeNextMove)
		{
			timeNextMove = timeNow + 500;

			int xPosDelta, yPosDelta;

			SDL_Log("%d\n", playerDirection);
			
			switch (playerDirection)
			{

				case 'r':
					xPosDelta = WINDOW_WIDTH / 40;
					yPosDelta = 0;
					break;
				case 'u':
					xPosDelta = 0;
					yPosDelta = -(WINDOW_HEIGHT / 30);
					break;
				case 'l':
					xPosDelta = -(WINDOW_WIDTH / 40);
					yPosDelta = 0;
					break;
				case 'd':
					xPosDelta = 0;
					yPosDelta = WINDOW_HEIGHT / 30;
					break;
				default:
					SDL_Log("Huh??");
			}

			playerCurNode = playerHead;
			int xPosNext = playerCurNode->xPos + xPosDelta;
			int yPosNext = playerCurNode->yPos + yPosDelta;
			while (playerCurNode)
			{
				int xPosOld = playerCurNode->xPos;
				int yPosOld = playerCurNode->yPos;
				playerCurNode->xPos = xPosNext;
				playerCurNode->yPos = yPosNext;
				xPosNext = xPosOld;
				yPosNext = yPosOld;
				playerCurNode = playerCurNode->next;
			}
		}

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		 * Draw the snake and wait, wait until next frame, draw frame.
		 */
		
		playerCurNode = playerHead;
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		while (playerCurNode)
		{
			// SDL_SetRenderDrawColor(renderer, rand() % 256, rand() % 256, rand() % 256, 255);
			SDL_Rect drawRect = {playerCurNode->xPos, playerCurNode->yPos, WINDOW_WIDTH / 40, WINDOW_HEIGHT / 30};
			SDL_RenderFillRect(renderer, &drawRect);
			playerCurNode = playerCurNode->next;
		}
		
		// wait until next frame
		while (SDL_GetTicks64() < timeNextFrame);
		
		// update game state, draw current frame
		SDL_RenderPresent(renderer);
		
	} // main loop end


	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Free the player's snake's nodes and destroy renderer, window.
	 */

	playerCurNode = playerHead;
	while (playerCurNode)
	{
		 SnakeNode *playerNextNode = playerCurNode->next;
		 free(playerCurNode);
		 playerCurNode = playerNextNode;
	}	

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	// print sdl subsystem initialization flags
	// printf("%X\n\n", SDL_WasInit(SDL_INIT_EVERYTHING));

	return 0;
}
