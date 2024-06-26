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

//	// display size struct
//	SDL_Rect displayBounds;
//	// get display bounds. if error, return
//	if (SDL_GetDisplayBounds(0, &displayBounds) != 0)
//	{
//		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
//		return 1;
//	}
//
//	// window dimensions
//	const int WINDOW_WIDTH = displayBounds.w * 2 / 3;
//	const int WINDOW_HEIGHT = displayBounds.h * 2 / 3;
//
//	// move distance dimensions
//	const int MOVE_W = gcd(WINDOW_WIDTH, WINDOW_HEIGHT) / 2;
//	const int MOVE_H = MOVE_W;

	const int WINDOW_W = 1280;
	const int WINDOW_H = 720;
	const int BOX_W = 20, BOX_H = 20;


	// create window
	SDL_Window *window = SDL_CreateWindow("ManySnakes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, 0);
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

	SDL_Rect body = {WINDOW_W / 2, WINDOW_H / 2, BOX_W, BOX_H};
	SDL_Rect bounds = {(WINDOW_W - WINDOW_H) / 2, 0, WINDOW_H, WINDOW_H};

	Snake *player = MNYSNKS_CreateSnake(125, &body, 3, UP);
	
	// seed rand
	srand(SDL_GetTicks());


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Make the fooood!!!!!
	 */


	SDL_Log("%s%s", ROOT_DIR, "/images/apple.png");

	char rootdir[128] = ROOT_DIR;
	strcat(rootdir, "/images/apple.png");

	body.x = body.y = 0;
	Food *apple = MNYSNKS_CreateFood(renderer, APPLE, &body, rootdir);

	MNYSNKS_RandPosFood(apple, player, &bounds);


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

		// draw snake play area
		SDL_SetRenderDrawColor(renderer, 160, 82, 45, 255);
		SDL_RenderFillRect(renderer, &bounds);

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

			int xLast = player->tail->x;
			int yLast = player->tail->y;
			MNYSNKS_StepSnake(player, &bounds);

			if (MNYSNKS_CheckCollisionSnake(player))
			{
				isRunning = false;
			}

			if (player->head->x == apple->body.x && player->head->y == apple->body.y)
			{
				MNYSNKS_RandPosFood(apple, player, &bounds);
				MNYSNKS_GrowSnake(player, xLast, yLast);
				SDL_Log("Size: %d", player->size);
			}

		}


		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		 * Draw the fruit, snake, and wait until next frame, draw frame.
		 */


		if (SDL_RenderCopy(renderer, apple->image, NULL, &apple->body) != 0)
		{
			
		}
		
		SnakeNode *cur = player->head;
		SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
		while (cur)
		{
			SDL_Rect rect = {cur->x, cur->y, player->w, player->h};
			SDL_RenderFillRect(renderer, &rect);
			cur = cur->next;
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
	// MNYSNKS_DestroyFood(food);
	MNYSNKS_DestroySnake(player);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	return 0;
}
