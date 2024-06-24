/* ManySnakes by Danielle Raine
 */

#include "config.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "snake.h"


int main(void)
{
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
	// SDL_version compiled;
	// SDL_version linked;
	
	// print compiled, linked sdl version
	// SDL_VERSION(&compiled);
	// SDL_GetVersion(&linked);
	// SDL_Log("Compiled against SDL version %u.%u.%u\n", compiled.major, compiled.minor, compiled.patch);
	// SDL_Log("Linking against SDL version %u.%u.%u\n", linked.major, linked.minor, linked.patch);


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Initialize SDL, IMG and register SDL_Quit, IMG_QUIT at exit.
	 */

	// initialize sdl. if error, return 
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
		return 1;
	}

	// register sdl shutdown on program closure
	atexit(SDL_Quit);

	// initialize img
	IMG_Init(IMG_INIT_PNG);

	// register img shutdown on program closure
	atexit(IMG_Quit);


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

	// move distance dimensions
	const int MOVE_W = gcd(WINDOW_WIDTH, WINDOW_HEIGHT) / 2;
	const int MOVE_H = MOVE_W;

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

	Snake *player = MNYSNKS_CreateSnake(250, UP, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, MOVE_W, MOVE_H, 5);
	
	// seed rand
	srand(SDL_GetTicks());


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Make the fooood!!!!!
	 */

//	Food *apple = malloc(sizeof(Food));
//	apple->type = APPLE;
//	apple->image = IMG_LoadTexture(renderer, "./images/apple.png");
//	if (!apple->image)
//	{
//		//FIXME
//	}
//
//


	// set player mover times
	player->lastMoveTime = SDL_GetTicks64();
	player->nextMoveTime = player->lastMoveTime + player->speed;


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Main loop, poll for events!
	 */

	bool isRunning = true;
	// main loop	
	while (isRunning)
	{
		SDL_Event event;

		// clear frame
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		// set the earliest time the next frame occurs
		Uint64 nextFrameTime = SDL_GetTicks64() + (1000 / 60);
		
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

				if (pressedKey == SDLK_RIGHT && player->currentDirection != LEFT) // pressed right key, skip if direction is left
				{
					player->pendingDirection = RIGHT; 
				}
				else if (pressedKey == SDLK_UP && player->currentDirection != DOWN) // pressed up key, skip if direction is down
				{
					player->pendingDirection = UP; 
				}
				else if (pressedKey == SDLK_LEFT && player->currentDirection != RIGHT) // pressed left key, skip if direction is right
				{
					player->pendingDirection = LEFT; 
				}
				else if (pressedKey == SDLK_DOWN && player->currentDirection != UP) // pressed down key, skip if direction is up
				{
					player->pendingDirection = DOWN;
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
		 * Snakes move on >= snake->nextMoveTime, so if current time is greater than or equal to the next
		 * time the snake is supposed to move, update the position of the snake depending on the direction 
		 * that the player pressed.
		 */

		Uint64 timeNow = SDL_GetTicks64();

		if (timeNow >= player->nextMoveTime)
		{
			player->lastMoveTime = player->nextMoveTime;
			player->nextMoveTime = timeNow + player->speed;

			MNYSNKS_StepSnake(player, 0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
		}


		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		 * Draw the snake and wait, wait until next frame, draw frame.
		 */
		
		SnakeNode *curPlayerNode = player->head;
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		while (curPlayerNode)
		{
			// SDL_SetRenderDrawColor(renderer, rand() % 256, rand() % 256, rand() % 256, 255);
			// SDL_Rect drawRect = {curPlayerNode->xPos, curPlayerNode->yPos, MOVE_W, MOVE_H};
			SDL_RenderFillRect(renderer, &curPlayerNode->body);
			curPlayerNode = curPlayerNode->next;
		}
		
		// wait until next frame
		while (SDL_GetTicks64() < nextFrameTime);
		
		// update game state, draw current frame
		SDL_RenderPresent(renderer);
		
	} // main loop end
	

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Free the player's snake's nodes and destroy renderer, window.
	 */
	
	// if you really love them, let them go
	MNYSNKS_DestroySnake(player);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	return 0;
}
