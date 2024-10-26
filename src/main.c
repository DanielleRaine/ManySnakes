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
#include "texture.h"

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

	// display size struct
	SDL_Rect displayBounds;
	// get display bounds. if error, return
	if (SDL_GetDisplayBounds(0, &displayBounds) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
		return 1;
	}

	// window dimensions
	const int WINDOW_WIDTH = displayBounds.w;
	const int WINDOW_HEIGHT = displayBounds.h;

	// create window
	SDL_Window *window = SDL_CreateWindow("ManySnakes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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
	int WINDOW_WIDTH, WINDOW_HEIGHT;
	SDL_GetWindowSize(window, &WINDOW_WIDTH, &WINDOW_HEIGHT);

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

	// Create an array of textboxes.
	int textboxesSize = 2;
	Textbox **textboxes = calloc(textboxesSize, sizeof(Textbox));

	// Set dimensions and colors of title textbox.
	SDL_Rect box = {WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 8, 400, 100};
	SDL_Color boxcolor = {0xFF, 0xFF, 0xFf, 0xFF};
	SDL_Color bordercolor = {0xFF, 0xA0, 0xD0, 0xFF};
	SDL_Color fontcolor = {0xFF, 0x00, 0xFF, 0xFF};
	
	// Create title textbox.
	textboxes[0] = CreateTextbox(renderer, &box, 15, &boxcolor, &bordercolor, font, &fontcolor, "ManySnakes");
	if (!textboxes[0])
	{
		PrintError();
		TTF_CloseFont(font);
		return -2;
	}

	// Create author textbox.
	box = (SDL_Rect) {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 4, 300, 50};
	textboxes[1] = CreateTextbox(renderer, &box, 10, &boxcolor, &bordercolor, font, &fontcolor, "By Danielle Raine");
	if (!textboxes[1])
	{
		PrintError();
		TTF_CloseFont(font);
		return -2;
	}


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Create play button.
	 */

	typedef struct Button
	{
		Textbutton *button;
		Textbox **textboxes;
	} Button;

	Button playButton = {NULL, calloc(3, sizeof(Textbox))};

	if (!playButton.textboxes)
	{
		SDL_Log("Failed to create play button. (textboxes array)");
		TTF_CloseFont(font);
		return -2;
	}

	box = (SDL_Rect) {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 4 * 3, 300, 100};
	playButton.textboxes[0] = CreateTextbox(renderer, &box, 10, &boxcolor, &bordercolor, font, &fontcolor, "Play!");
	playButton.textboxes[1] = CreateTextbox(renderer, &box, 15, &bordercolor, &fontcolor, font, &boxcolor, "Play!");
	playButton.textboxes[2] = CreateTextbox(renderer, &box, 20, &fontcolor, &boxcolor, font, &bordercolor, "Play!");

	playButton.button = CreateTextbutton(&box, playButton.textboxes[0], playButton.textboxes[1], playButton.textboxes[2]);

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

		// display next frame once the next frame time is reached
		Uint64 currentTime = SDL_GetTicks64();
		if (currentTime >= nextFrameTime) 
		{
			nextFrameTime = currentTime + (1000 / 60);
			
			if (SDL_SetRenderDrawColor(renderer, 0xA0, 0x00, 0xA0, 0xFF) != 0 || SDL_RenderClear(renderer) != 0)
			{
				PrintError();
				returnCode = -2;
				break;
			}

			//SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
			
			if (!RenderTextboxes(renderer, textboxes, textboxesSize))
			{
				PrintError();
				returnCode = -2;
				break;
			}

			int mouseX, mouseY;

			RenderTextbutton(renderer, playButton.button, SDL_GetMouseState(&mouseX, &mouseY), mouseX, mouseY);

			SDL_RenderPresent(renderer);
		}
	}
	
	DestroyTextboxes(textboxes, textboxesSize);
	DestroyTextboxes(playButton.textboxes, 3);
	DestroyTextbutton(playButton.button);
	TTF_CloseFont(font);
	return ~returnCode;
}

int Play(SDL_Window *window, SDL_Renderer *renderer)
{
	// get window and box size
	int WINDOW_WIDTH, WINDOW_HEIGHT;
	SDL_GetWindowSize(window, &WINDOW_WIDTH, &WINDOW_HEIGHT);

	// create frame buffer
	SDL_Texture *buffer = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!buffer)
	{
		PrintError();
		return -2;
	}

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Create the box size for snake and food, set play bounds, create the player's snake.
	 */

	// create player's snake
	Snake *player = CreateSnake(19, 19, 20, 20, 125, 3, SNAKE_UP, & (SDL_Color) {0x00, 0x00, 0xA0, 0xFF});
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

	// create apple
	Food *apple = CreateFood(renderer, FOOD_APPLE, 0, 0, 20, 20, applePNG);
	if (!apple)
	{
		PrintError();
		DestroySnake(player);
		SDL_DestroyTexture(buffer);
		return -2;
	}
	// randomize apple position
	RandPosFood(apple, player, 40, 40);


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Set time of next player movement, play loop, poll for events!
	 */
	
	// set player move times
	player->lastMoveTime = SDL_GetTicks64();
	player->nextMoveTime = player->lastMoveTime + player->speed;

	// play loop
	int returnCode = 0;
	bool isRunning = true;
	// set the earliest time the next frame occurs
	Uint64 nextFrameTime = SDL_GetTicks64() + (1000 / 60);
	while (isRunning)
	{
		SDL_Event event;
		
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
				else if (pressedKey == SDLK_RIGHT && player->currentDirection != SNAKE_LEFT) // pressed right key, skip if direction is left
				{
					player->pendingDirection = SNAKE_RIGHT; 
				}
				else if (pressedKey == SDLK_UP && player->currentDirection != SNAKE_DOWN) // pressed up key, skip if direction is down
				{
					player->pendingDirection = SNAKE_UP; 
				}
				else if (pressedKey == SDLK_LEFT && player->currentDirection != SNAKE_RIGHT) // pressed left key, skip if direction is right
				{
					player->pendingDirection = SNAKE_LEFT; 
				}
				else if (pressedKey == SDLK_DOWN && player->currentDirection != SNAKE_UP) // pressed down key, skip if direction is up
				{
					player->pendingDirection = SNAKE_DOWN;
				}
			}
		}
		
		if (returnCode != 0)
			break;
		
		
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
			int xTail = player->tail->xPos;
			int yTail = player->tail->yPos;

			// update the snake's position
			StepSnake(player, 40, 40);
			
			// if snake hits itself, end game
			if (CheckCollisionSnake(player)) 
			{
				isRunning = false;
				SDL_Log("Game Over");
			}
 
			// if snake eats food, grow snake and move food
			if (player->head->xPos == apple->xPos && player->head->yPos == apple->yPos)
			{
				//FIXME add resolution case when snake covers whole map (probably not needed)
				RandPosFood(apple, player, 40, 40);
				GrowSnake(player, xTail, yTail);
				SDL_Log("Size: %d", player->length);
			}

		}


		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		 * Draw the food, snake, and wait until next frame, draw frame.
		 */
		
		Uint64 currentTime = SDL_GetTicks64();
		if (currentTime >= nextFrameTime) 
		{
			nextFrameTime = currentTime + (1000 / 60);
			
			// set buffer as render target and clear frame
			if (SDL_SetRenderTarget(renderer, buffer) != 0 || SDL_SetRenderDrawColor(renderer, 0xad, 0xd8, 0xe6, 0xff) != 0 || SDL_RenderClear(renderer) != 0)
			{
				PrintError();
				returnCode = -2;
				break;
			}
			
			// draw snake play area
			if (SDL_SetRenderDrawColor(renderer, 0xe0, 0xb0, 0xff, 0xff) != 0 || SDL_RenderFillRect(renderer, & (SDL_Rect) {0, 0, 800, 800}) != 0)
			{
				PrintError();
				returnCode = -2;
				break;
			}


			// render food and snake, copy to renderer
			if (!(RenderFood(renderer, apple, 0, 0, 20, 20) && RenderSnake(renderer, player, 0, 0, 20, 20)) || SDL_SetRenderTarget(renderer, NULL) != 0 || SDL_RenderCopy(renderer, buffer, NULL, NULL) != 0)
			{
				PrintError();
				returnCode = -2;
				break;
			}

			SDL_RenderPresent(renderer);
		}
		
		if (isPaused)
		{	
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

	return returnCode;
}

int Pause(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *buffer)
{	
	int WINDOW_WIDTH, WINDOW_HEIGHT;
	SDL_GetWindowSize(window, &WINDOW_WIDTH, &WINDOW_HEIGHT);
	
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
