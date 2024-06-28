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


void PrintGameInfo();
void PrintError();
bool RenderFood(SDL_Renderer *renderer, Food *food);
bool RenderSnake(SDL_Renderer *renderer, Snake *snake);
bool Play(SDL_Window *window, SDL_Renderer *renderer);
bool Pause(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *buffer);

int main(void)
{
	// print the game's credits and versions
	PrintGameInfo();

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Initialize SDL, IMG and register SDL_Quit, IMG_QUIT at exit.
	 */

	// initialize sdl. if error, return 
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
		return 1;
	}

	// initialize img
	IMG_Init(IMG_INIT_PNG);


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
	if (!renderer || SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
		SDL_DestroyWindow(window);
		return 1;
	}

	

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 *
	 */

	// seed rand
	srand(SDL_GetTicks());
	
	while (true)
	{
		if (!Play(window, renderer))
			break;
	}


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Destroy the renderer and window, quit IMG and SDL, then return.
	 */

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();

	return 0;
}


void PrintGameInfo()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Print the game's description, Github URL, and version numbers of the game, C, and SDL.
	 */

	// print game description, github url
	SDL_Log("%s\n", ManySnakes_DESCRIPTION);
	SDL_Log("Homepage Url %s\n", ManySnakes_HOMEPAGE_URL);
	
	// print game version
	switch (ManySnakes_VERSION_PHASE)
	{
		case 0: // is release version
			SDL_Log("Game Version Release %d.%d.%d\n", ManySnakes_VERSION_MAJOR, ManySnakes_VERSION_MINOR, ManySnakes_VERSION_PATCH);
			break;
		case 1: // is release preview version
			SDL_Log("Game Version Release %d.%d.0-preview.%d\n", ManySnakes_VERSION_MAJOR, ManySnakes_VERSION_MINOR, ManySnakes_VERSION_PATCH);
			break;
		case 2: // is pre-alpha version
			SDL_Log("Game Version pre-alpha.%d\n", ManySnakes_VERSION_PATCH);
			break;
		case 3: // is alpha version
			SDL_Log("Game Version alpha-%d.%d\n", ManySnakes_VERSION_MINOR, ManySnakes_VERSION_PATCH);
			break;
		case 4: // is beta version
			SDL_Log("Game Version beta-%d.%d.%d\n", ManySnakes_VERSION_MAJOR, ManySnakes_VERSION_MINOR, ManySnakes_VERSION_PATCH);
			break;
		default: // what happened here???
			SDL_Log("Unknown???\n");
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
}

void PrintError()
{
	SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
	SDL_ClearError();
}

bool RenderFood(SDL_Renderer *renderer, Food *food)
{
	if (SDL_RenderCopy(renderer, food->image, NULL, &food->body) != 0)
	{
		PrintError();
		return false;
	}

	return true;	
}

bool RenderSnake(SDL_Renderer *renderer, Snake *snake)
{
	SnakeNode *cur = snake->head;
	SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0, 255);
	while (cur)
	{
		SDL_Rect rect = {cur->x, cur->y, snake->w, snake->h};
		if (SDL_RenderFillRect(renderer, &rect) != 0)
		{
			PrintError();
			return false;
		}
		cur = cur->next;
	}

	return true;
}

bool Play(SDL_Window *window, SDL_Renderer *renderer)
{
	// set window and box size
	int WINDOW_W, WINDOW_H;
	SDL_GetWindowSize(window, &WINDOW_W, &WINDOW_H);
	const int BOX_W = 20, BOX_H = 20;

	// create buffer to render to and then from for each frame
	SDL_Texture *buffer = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_TARGET, WINDOW_W, WINDOW_H);

	if (!buffer)
	{
		PrintError();
		return false;
	}

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Create the body segment size for snake and food, set play bounds,
	 * create the player's snake and seed rand.
	 */

	// set body segment and play bounds
	SDL_Rect body = {WINDOW_W / 2, WINDOW_H / 2, BOX_W, BOX_H};
	SDL_Rect bounds = {(WINDOW_W - WINDOW_H) / 2, 0, WINDOW_H, WINDOW_H};

	// create player's snake
	Snake *player = CreateSnake(125, &body, 3, UP);
	

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Set the food texture PNG path, create food, and give it a random position.
	 */

	char applePNG[128] = ROOT_DIR;
	strcat(applePNG, "/images/apple.png");

	body.x = body.y = 0;
	Food *apple = CreateFood(renderer, APPLE, &body, applePNG);

	RandPosFood(apple, player, &bounds);


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Set time of next player movement, main loop, poll for events!
	 */
	
	// set player mover times
	player->lastMoveTime = SDL_GetTicks64();
	player->nextMoveTime = player->lastMoveTime + player->speed;

	// main loop
	bool isRunning = true;
	bool isClosed = false;
	while (isRunning)
	{
		SDL_Event event;
		bool isPaused = false;
		
		if (SDL_SetRenderTarget(renderer, buffer) != 0)
		{
			PrintError();
			isClosed = true;
			break;
		}

		// clear frame
		if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0) != 0 || SDL_RenderClear(renderer) != 0)
		{
			PrintError();
			isClosed = true;
			break;
		}

		// set the earliest time the next frame occurs
		Uint64 nextFrameTime = SDL_GetTicks64() + (1000 / 60); //FIXME might change how works in future!
		
		// poll events
		while (SDL_PollEvent(&event))
		{
			if (SDL_QUIT == event.type) // if quit, stop event poll and exit main loop
			{
				isClosed = true;
				goto endPlay;
			}
			else if (SDL_KEYDOWN == event.type) // if key pressed down, handle it!
			{
				SDL_Keycode pressedKey = event.key.keysym.sym;

				// log the name of pressed key
				SDL_Log("%s", SDL_GetKeyName(pressedKey));

				if (pressedKey == SDLK_ESCAPE)
				{
					isPaused = true;
				}
				else if (pressedKey == SDLK_RIGHT && player->currentDirection != LEFT) // pressed right key, skip if direction is left
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
		
		// draw snake play area
		if (SDL_SetRenderDrawColor(renderer, 94, 64, 51, 255) != 0 || SDL_RenderFillRect(renderer, &bounds) != 0)
		{
			PrintError();
			isClosed = true;
			break;
		}


		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		 * Move snake when time has elapsed. If snake eats food, reposition the food and grow snake.
		 * If the snake hits itself, end game.
		 */

		Uint64 timeNow = SDL_GetTicks64();

		if (timeNow >= player->nextMoveTime)
		{
			// update next move time
			player->lastMoveTime = player->nextMoveTime;
			player->nextMoveTime = timeNow + player->speed;

			// store tail position for new tail if snake grows 
			int xTail = player->tail->x;
			int yTail = player->tail->y;

			// update the snake's position
			StepSnake(player, &bounds);
			
			// if snake hits itself, end game
			if (CheckCollisionSnake(player)) 
			{
				isRunning = false;
				SDL_Log("Game Over");
			}
 
			// if snake eats food, grow snake and move food
			if (player->head->x == apple->body.x && player->head->y == apple->body.y)
			{
				//FIXME add resolution case when snake covers whole map (probably not needed)
				RandPosFood(apple, player, &bounds);
				GrowSnake(player, xTail, yTail);
				SDL_Log("Size: %d", player->size);
			}

		}


		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		 * Draw the fruit, snake, and wait until next frame, draw frame.
		 */

		if (!(RenderFood(renderer, apple) && RenderSnake(renderer, player)))
		{
			isClosed = true;
			break;
		}	
		
		// draw current frame
		if (SDL_SetRenderTarget(renderer, NULL) != 0 || SDL_RenderCopy(renderer, buffer, NULL, NULL) != 0)
		{
			PrintError();
			isClosed = true;
			break;
		}

		// wait and present next frame
		while (SDL_GetTicks64() < nextFrameTime);
		SDL_RenderPresent(renderer);
		
		if (isPaused && !Pause(window, renderer, buffer))
		{
			isClosed = true;
			break;
		}
	} // main loop end
	
endPlay:
	// if you really love them, let them go
	DestroyFood(apple);
	DestroySnake(player);
	SDL_DestroyTexture(buffer);

	return !isClosed;
}

bool Pause(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *buffer)
{	
	int WINDOW_W, WINDOW_H;
	SDL_GetWindowSize(window, &WINDOW_W, &WINDOW_H);
	
	if (SDL_SetTextureBlendMode(buffer, SDL_BLENDMODE_BLEND) != 0 || SDL_SetTextureAlphaMod(buffer, 75) != 0)
	{
		PrintError();
		return false;
	}

	bool isRunning = true;
	bool isClosed = false;
	while (isRunning) // main loop
	{
		SDL_Event event;

		// clear frame
		if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0) != 0 || SDL_RenderClear(renderer) != 0)
		{
			PrintError();
			isClosed = true;
			break;
		}
		
		// set the earliest time the next frame occurs
		Uint64 nextFrameTime = SDL_GetTicks64() + (1000 / 60);
		
		// poll events
		while (SDL_PollEvent(&event))
		{
			if (SDL_QUIT == event.type) // if quit, stop event poll and exit main loop
			{
				isClosed = true;
				goto endPause;
			}
			else if (SDL_KEYDOWN == event.type) // if key pressed down, handle it!
			{
				SDL_Keycode pressedKey = event.key.keysym.sym;

				// log the name of pressed key
				SDL_Log("%s", SDL_GetKeyName(pressedKey));

				if (pressedKey == SDLK_ESCAPE)
				{
					isRunning = false;
				}
			}
		}
		
		if (SDL_SetRenderTarget(renderer, buffer) != 0)
		{
			PrintError();
			isClosed = true;
			break;
		}
		


		// draw current frame
		if (SDL_SetRenderTarget(renderer, NULL) != 0 || SDL_RenderCopy(renderer, buffer, NULL, NULL))
		{
			PrintError();
			isClosed = true;
			break;
		}
				
		// wait and present next frame
		while (SDL_GetTicks64() < nextFrameTime);
		SDL_RenderPresent(renderer);
	}

endPause:
	
	if (SDL_SetTextureBlendMode(buffer, SDL_BLENDMODE_NONE) != 0 || SDL_SetTextureAlphaMod(buffer, 0xFF) != 0)
	{
		PrintError();
		isClosed = true;
	}
	
	if (SDL_SetRenderTarget(renderer, NULL) != 0)
	{
		PrintError();
		isClosed = true;
	}
	
	return !isClosed;
}
