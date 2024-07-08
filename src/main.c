/* ManySnakes by Danielle Raine
 * Created June 6th, 2024
 */

#include "config.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "snake.h"
#include "text.h"

void PrintGameInfo();
void PrintError();
int MainMenu(SDL_Window *window, SDL_Renderer *renderer);
int Play(SDL_Window *window, SDL_Renderer *renderer);
int Pause(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *buffer);

int main(void)
{
	// print the game's credits and versions
	PrintGameInfo();

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Initialize SDL, IMG, and TTF.
	 */

	// initialize sdl and ttf. if error, return 
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0 || TTF_Init() != 0)
	{
		PrintError();
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
		PrintError();
		return 1;
	}

	// create renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	// if renderer doesn't exist, print error, destroy window and return
	if (!renderer || SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) != 0)
	{
		PrintError();
		SDL_DestroyWindow(window);
		return 1;
	}

	

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Seed rand and begin main loop.
	 */

	// seed rand
	srand(SDL_GetTicks());

	int returnCode = MainMenu(window, renderer);	
	printf("Exit MainMenu: %d\n", returnCode);

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Destroy renderer and window, quit IMG and SDL, then return.
	 */

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return returnCode;
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

int MainMenu(SDL_Window *window, SDL_Renderer *renderer)
{
	// get window and box size
	int WINDOW_W, WINDOW_H;
	SDL_GetWindowSize(window, &WINDOW_W, &WINDOW_H);
	// const int BOX_W = 20, BOX_H = 20;

	// initial clear frame
	if (SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0x00, 0xFF) != 0 || SDL_RenderClear(renderer) != 0)
	{
		PrintError();
		return -2;
	}


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Create font and textboxes.
	 */

	// get font location
	char fontpath[128] = ROOT_DIR;
	strcat(fontpath, "/Roboto_Mono/RobotoMono-VariableFont_wght.ttf");
	
	// create font
	TTF_Font *font = TTF_OpenFont(fontpath, 50);
	if (!font)
	{
		PrintError();
		return -2;
	}

	// create array of texts
	int textsSize = 2;
	Text **texts = calloc(textsSize, sizeof(Text));

	// set title text box dimensions and color
	SDL_Rect box = {WINDOW_W / 2 - 200, WINDOW_H / 8, 400, 100};
	SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
	
	// create title text box
	texts[0] = CreateText(renderer, &box, font, &color, "ManySnakes");
	if (!texts[0])
	{
		PrintError();
		TTF_CloseFont(font);
		return -2;
	}

	// create author text box
	box = (SDL_Rect) {WINDOW_W / 2 - 150, WINDOW_H / 4, 300, 50};
	texts[1] = CreateText(renderer, &box, font, &color, "By Danielle Raine");
	if (!texts[1])
	{
		PrintError();
		TTF_CloseFont(font);
		return -2;
	}

	// create play button


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Set next frame time and begin main menu loop.
	 */

	// set the next time a frame is presented
	Uint64 nextFrameTime = SDL_GetTicks64() + (1000 / 60);

	// main menu loop
	int returnCode = 0;
	bool isRunning = true;
	while (isRunning)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (SDL_QUIT == event.type)
			{ 
				// window is closed
				returnCode = -1;
				break;
			}
			else if (SDL_KEYDOWN == event.type)
			{
				// key pressed down
				SDL_Log("Key pressed!");
				SDL_Keycode key = event.key.keysym.sym;
				if (SDLK_RETURN == key)
				{
					returnCode = Play(window, renderer);
					SDL_Log("Exit Play: %d", returnCode);
					if (returnCode != 0)
						break;
				}
			}
		}

		if (returnCode != 0)
			break;

		if (!RenderTexts(renderer, texts, textsSize))
		{
			PrintError();
			returnCode = -2;
			break;
		}

		// display next frame once the next frame time is reached
		Uint64 currentTime = SDL_GetTicks64();
		if (currentTime >= nextFrameTime) 
		{
			nextFrameTime = currentTime + (1000 / 60);
			SDL_RenderPresent(renderer);
			if (SDL_SetRenderDrawColor(renderer, 0x5F, 0x00, 0xFF, 0xFF) != 0 || SDL_RenderClear(renderer) != 0)
			{
				PrintError();
				returnCode = -2;
				break;
			}
		}
	}
	
	DestroyTexts(texts, textsSize);
	TTF_CloseFont(font);
	return ~returnCode;
}

int Play(SDL_Window *window, SDL_Renderer *renderer)
{
	// get window and box size
	int WINDOW_W, WINDOW_H;
	SDL_GetWindowSize(window, &WINDOW_W, &WINDOW_H);
	const int BOX_W = 20, BOX_H = 20;

	// create frame buffer
	SDL_Texture *buffer = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_TARGET, WINDOW_W, WINDOW_H);
	if (!buffer)
	{
		PrintError();
		return -2;
	}

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Create the box size for snake and food, set play bounds, create the player's snake.
	 */

	// set box size and play bounds
	SDL_Rect box = {WINDOW_W / 2, WINDOW_H / 2, BOX_W, BOX_H};
	SDL_Rect bounds = {(WINDOW_W - WINDOW_H) / 2, 0, WINDOW_H, WINDOW_H};

	// create player's snake
	Snake *player = CreateSnake(125, &box, 3, UP);
	if (!player)
	{
		PrintError();
		SDL_DestroyTexture(buffer);
		return -2;
	}	

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Set the food texture PNG path, create food, and give it a random position.
	 */

	// get apple png path
	char applePNG[128] = ROOT_DIR;
	strcat(applePNG, "/images/apple.png");

	// set starting apple position to (0, 0)
	box.x = box.y = 0;

	// create apple
	Food *apple = CreateFood(renderer, APPLE, &box, applePNG);
	if (!apple)
	{
		PrintError();
		DestroySnake(player);
		SDL_DestroyTexture(buffer);
		return -2;
	}

	RandPosFood(apple, player, &bounds);


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Set time of next player movement, play loop, poll for events!
	 */
	
	// set player move times
	player->lastMoveTime = SDL_GetTicks64();
	player->nextMoveTime = player->lastMoveTime + player->speed;

	// play loop
	int returnCode = 0;
	bool isRunning = true;
	while (isRunning)
	{
		SDL_Event event;
		
		// set buffer as render target and clear frame
		if (SDL_SetRenderTarget(renderer, buffer) != 0 || SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0) != 0 || SDL_RenderClear(renderer) != 0)
		{
			PrintError();
			returnCode = -2;
			break;
		}

		// set the earliest time the next frame occurs
		Uint64 nextFrameTime = SDL_GetTicks64() + (1000 / 60); //FIXME might change how works in future!

		bool isPaused = false;
		
		// poll events
		while (SDL_PollEvent(&event))
		{
			if (SDL_QUIT == event.type) // if quit, stop event poll and exit main loop
			{
				returnCode = -1;
				break;
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
		
		if (returnCode != 0)
			break;
		
		// draw snake play area
		if (SDL_SetRenderDrawColor(renderer, 94, 64, 51, 255) != 0 || SDL_RenderFillRect(renderer, &bounds) != 0)
		{
			PrintError();
			returnCode = -2;
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
			if (player->head->x == apple->box.x && player->head->y == apple->box.y)
			{
				//FIXME add resolution case when snake covers whole map (probably not needed)
				RandPosFood(apple, player, &bounds);
				GrowSnake(player, xTail, yTail);
				SDL_Log("Size: %d", player->size);
			}

		}


		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		 * Draw the food, snake, and wait until next frame, draw frame.
		 */

		// render food and snake, copy to renderer
		if (!(RenderFood(renderer, apple) && RenderSnake(renderer, player)) || SDL_SetRenderTarget(renderer, NULL) != 0 || SDL_RenderCopy(renderer, buffer, NULL, NULL) != 0)
		{
			PrintError();
			returnCode = -2;
			break;
		}

		// wait and present next frame
		while (SDL_GetTicks64() < nextFrameTime);
		SDL_RenderPresent(renderer);
		
		if (isPaused)
		{	//FIXME Snake moves immediately after unpause if enough time elapsed during pause instead of during game
			Uint64 timeBeforePause = SDL_GetTicks64();
			returnCode = Pause(window, renderer, buffer);
			SDL_Log("Exit Pause: %d", returnCode);
			if (returnCode == 0)
			{
				player->nextMoveTime += SDL_GetTicks64() - timeBeforePause;
			}
			else if (returnCode > 0)
			{
				--returnCode;
			}
			else
			{
				break;
			}
		}
	} // play loop end
		
	DestroyFood(apple);
	DestroySnake(player);
	SDL_DestroyTexture(buffer);

	//FIXME return code wrong
	return returnCode;
}

int Pause(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *buffer)
{	
	int WINDOW_W, WINDOW_H;
	SDL_GetWindowSize(window, &WINDOW_W, &WINDOW_H);
	
	if (SDL_SetTextureBlendMode(buffer, SDL_BLENDMODE_BLEND) != 0 || SDL_SetTextureAlphaMod(buffer, 75) != 0)
	{
		PrintError();
		return -2;
	}

	// main loop
	int returnCode = 0;
	bool isRunning = true;
	while (isRunning)
	{
		SDL_Event event;

		// clear frame
		if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0) != 0 || SDL_RenderClear(renderer) != 0)
		{
			PrintError();
			returnCode = -2;
			break;
		}
		
		// set the earliest time the next frame occurs
		Uint64 nextFrameTime = SDL_GetTicks64() + (1000 / 60);
		
		// poll events
		while (SDL_PollEvent(&event))
		{
			if (SDL_QUIT == event.type) // if quit, stop event poll and exit main loop
			{
				returnCode = -1;
				break;
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

		if (returnCode != 0)
		{
			break;
		}
		
		// draw frame
		if (SDL_SetRenderTarget(renderer, buffer) != 0 || SDL_SetRenderTarget(renderer, NULL) != 0 || SDL_RenderCopy(renderer, buffer, NULL, NULL))
		{
			PrintError();
			returnCode = -2;
			break;
		}
				
		// wait and present next frame
		while (SDL_GetTicks64() < nextFrameTime);
		SDL_RenderPresent(renderer);
	}

	if (SDL_SetTextureBlendMode(buffer, SDL_BLENDMODE_NONE) != 0 || SDL_SetTextureAlphaMod(buffer, 0xFF) != 0 || SDL_SetRenderTarget(renderer, NULL) != 0)
	{
		PrintError();
		returnCode = -2;
	}
	
	SDL_Log("%d", returnCode);

	
	return returnCode;
}
