/** 
 * @file
 * @author Danielle Raine
 * @date Created June 6th, 2024
 * @date Last Modified January 7th, 2025
 * @brief ManySnakes by Danielle Raine
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "version.h"
#include "snake.h"
#include "texture.h"
#include "resource.h"


void PrintGameInfo();
void PrintError();
int MainMenu(SDL_Window *window, SDL_Renderer *renderer, lua_State *L);
int Play(SDL_Window *window, SDL_Renderer *renderer, lua_State *L);
int Pause(SDL_Window *window, SDL_Renderer *renderer, lua_State *L, SDL_Texture *buffer);


int main(void)
{
	PrintGameInfo();


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Initialize SDL, IMG, and TTF. Register their respective quit functions at exit.
	 */

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0 || TTF_Init() != 0)
	{
		PrintError();
		return 1;
	}

	atexit(SDL_Quit);
	atexit(TTF_Quit);

	IMG_Init(IMG_INIT_PNG);
	atexit(IMG_Quit);


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Create the Lua state for reading Lua scripts.
	 */

	lua_State *L = luaL_newstate();
	if (!L)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create Lua State (main)");
		return 1;
	}

	luaL_openlibs(L);


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Get the window bounds from config.lua. Get the display bounds. Create the window and the renderer.
	 */
	
	if (luaL_dofile(L, "scripts/config.lua") != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Cannot run configuration file %s (main)", lua_tostring(L, -1));
		lua_close(L);
		return 1;
	}

	lua_getglobal(L, "window_width");
	if (!lua_isnumber(L, -1))
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "'window_width' should be a number (main)");
		lua_close(L);
		return 1;
	}
	
	lua_getglobal(L, "window_height");
	if (!lua_isnumber(L, -1))
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "'window_height' should be a number (main)");
		lua_close(L);
		return 1;
	}

	int window_width = (int) lua_tonumber(L, -2);
	int window_height = (int) lua_tonumber(L, -1);
	lua_pop(L, 2);

	//SDL_Rect display_bounds;
	//if (SDL_GetDisplayBounds(0, &display_bounds) != 0)
	//{
	//	SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
	//	return 1;
	//}

	SDL_Window *window = SDL_CreateWindow("ManySnakes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, 0);
	if (!window)
	{
		PrintError();
		lua_close(L);
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer || SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) != 0)
	{
		PrintError();
		SDL_DestroyWindow(window);
		lua_close(L);
		return 1;
	}


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Call the main menu.
	 */

	//FIXME Change to system time.
	srand(SDL_GetTicks());

	int return_code = MainMenu(window, renderer, L);	
	SDL_Log("Exit MainMenu (%d)\n", return_code);


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Close the Lua state. Destroy the renderer and the window. Quit IMG and SDL, then return.
	 */

	lua_close(L);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	return return_code;
}


void PrintGameInfo()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Print the game's description, Github URL, and versions of the game, C, and SDL.
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
	
	// print compiled, linked sdl version
	SDL_version compiled;
	SDL_version linked;
	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	SDL_Log("Compiled SDL version %u.%u.%u\n", compiled.major, compiled.minor, compiled.patch);
	SDL_Log("Linked against SDL version %u.%u.%u\n", linked.major, linked.minor, linked.patch);
}

void PrintError()
{
	SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
	SDL_ClearError();
}

int MainMenu(SDL_Window *window, SDL_Renderer *renderer, lua_State *L)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Get the window bounds. Clear the frame. Get the frames per second.
	 */

	int window_width, window_height;
	SDL_GetWindowSize(window, &window_width, &window_height);

	if (SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0x00, 0xFF) != 0 || SDL_RenderClear(renderer) != 0)
	{
		PrintError();
		return -2;
	}

	lua_getglobal(L, "frames_per_second");
	if (!lua_isnumber(L, -1))
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "'frames_per_second' should be a number ()");
		return -2;
	}

	int frames_per_second = (int) lua_tonumber(L, -1);
	lua_pop(L, 1);


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Create the font and main menu textboxes.
	 */

	//TTF_Font *font = TTF_OpenFont("assets/fonts/Roboto_Mono/RobotoMono-VariableFont_wght.ttf", 50);
	//if (!font)
	//{
	//	PrintError();
	//	return -2;
	//}

	//// create an array of textboxes
	//int textboxes_size = 2;
	//Textbox **textboxes = calloc(textboxes_size, sizeof(Textbox));

	//// set dimensions and colors of title textbox
	//SDL_Rect box = {window_width / 2 - 200, window_height / 8, 400, 100};
	//SDL_Color box_color = {0xFF, 0xFF, 0xFf, 0xFF};
	//SDL_Color border_color = {0xFF, 0xA0, 0xD0, 0xFF};
	//SDL_Color font_color = {0xFF, 0x00, 0xFF, 0xFF};
	//
	//// create title textbox
	//textboxes[0] = CreateTextbox(renderer, &box, 15, &box_color, &border_color, font, &font_color, "ManySnakes");
	//if (!textboxes[0])
	//{
	//	PrintError();
	//	TTF_CloseFont(font);
	//	return -2;
	//}

	//// create author textbox
	//box = (SDL_Rect) {window_width / 2 - 150, window_height / 4, 300, 50};
	//textboxes[1] = CreateTextbox(renderer, &box, 10, &box_color, &border_color, font, &font_color, "By Danielle Raine");
	//if (!textboxes[1])
	//{
	//	PrintError();
	//	TTF_CloseFont(font);
	//	return -2;
	//}


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Create play button.
	 */

	//typedef struct Button
	//{
	//	Textbutton *button;
	//	Textbox **textboxes;
	//} Button;

	//Button play_button = {NULL, calloc(3, sizeof(Textbox))};

	//if (!play_button.textboxes)
	//{
	//	SDL_Log("Failed to create play button. (textboxes array)");
	//	TTF_CloseFont(font);
	//	return -2;
	//}

	//box = (SDL_Rect) {window_width / 2 - 150, window_height / 4 * 3, 300, 100};
	//play_button.textboxes[0] = CreateTextbox(renderer, &box, 10, &box_color, &border_color, font, &font_color, "Play!");
	//play_button.textboxes[1] = CreateTextbox(renderer, &box, 15, &border_color, &font_color, font, &box_color, "Play!");
	//play_button.textboxes[2] = CreateTextbox(renderer, &box, 20, &font_color, &box_color, font, &border_color, "Play!");

	//play_button.button = CreateTextbutton(&box, play_button.textboxes[0], play_button.textboxes[1], play_button.textboxes[2]);


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Set next frame time and begin main menu loop.
	 */

	Uint64 next_frame_time = SDL_GetTicks64() + (1000 / frames_per_second);

	int return_code = 0;
	bool is_running = true;
	while (is_running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (SDL_QUIT == event.type) // window is closed
			{ 
				return_code = -1;
				break;
			}
			else if (SDL_KEYDOWN == event.type) // a key is pressed
			{
				SDL_Log("Key pressed!");
				SDL_Keycode key = event.key.keysym.sym;
				if (SDLK_RETURN == key)
				{
					return_code = Play(window, renderer, L);
					SDL_Log("Exit Play (%d)", return_code);
					if (return_code != 0)
						break;
				}
				else if (SDLK_F5 == key)
				{


				}
			}
			else if (SDL_MOUSEBUTTONUP == event.type)
			{
				//if (SDL_PointInRect(& (SDL_Point) {event.button.x, event.button.y}, &play_button.button->mouseArea))
				//{
				//	return_code = Play(window, renderer, L);
				//	SDL_Log("Exit Play (%d)", return_code);
				//	if (return_code != 0)
				//		break;
				//}
			}
		}

		if (return_code != 0)
			break;

		// display next frame once the next frame time is reached
		Uint64 current_time = SDL_GetTicks64();
		if (current_time >= next_frame_time) 
		{
			next_frame_time = current_time + (1000 / frames_per_second);

			if (SDL_SetRenderDrawColor(renderer, 0xA0, 0x00, 0xA0, 0xFF) != 0 || SDL_RenderClear(renderer) != 0)
			{
				PrintError();
				return_code = -2;
				break;
			}

			//SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
			
			//if (!RenderTextboxes(renderer, textboxes, textboxes_size))
			//{
			//	PrintError();
			//	return_code = -2;
			//	break;
			//}

			//int mouseX, mouseY;

			//RenderTextbutton(renderer, play_button.button, SDL_GetMouseState(&mouseX, &mouseY), mouseX, mouseY);

			SDL_RenderPresent(renderer);
		}
	}
	
	//DestroyTextboxes(textboxes, textboxes_size);
	//DestroyTextboxes(play_button.textboxes, 3);
	//DestroyTextbutton(play_button.button);
	//TTF_CloseFont(font);
	return ~return_code;
}

int Play(SDL_Window *window, SDL_Renderer *renderer, lua_State *L)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Get the window bounds. Create the frame buffer. Get the frames per second.
	 */

	int window_width, window_height;
	SDL_GetWindowSize(window, &window_width, &window_height);

	SDL_Texture *buffer = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_TARGET, window_width, window_height);
	if (!buffer)
	{
		PrintError();
		return -2;
	}

	lua_getglobal(L, "frames_per_second");
	int frames_per_second = (int) lua_tonumber(L, -1);
	lua_pop(L, 1);


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
	char applePNG[128] = "assets/textures/Apple.png";

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
	int return_code = 0;
	bool is_running = true;
	// set the earliest time the next frame occurs
	Uint64 next_frame_time = SDL_GetTicks64() + (1000 / frames_per_second);
	while (is_running)
	{
		SDL_Event event;
		
		bool is_paused = false;
		
		// poll events
		while (SDL_PollEvent(&event))
		{
			if (SDL_QUIT == event.type) // if quit, stop event poll and exit main loop
			{
				return_code = -1;
				break;
			}
			else if (SDL_KEYDOWN == event.type) // if key pressed down, handle it!
			{
				SDL_Keycode pressed_key = event.key.keysym.sym;

				// log the name of pressed key
				SDL_Log("%s", SDL_GetKeyName(pressed_key));

				if (pressed_key == SDLK_ESCAPE)
				{
					is_paused = true;
				}
				else if (pressed_key == SDLK_RIGHT && player->currentDirection != SNAKE_LEFT) // pressed right key, skip if direction is left
				{
					player->pendingDirection = SNAKE_RIGHT; 
				}
				else if (pressed_key == SDLK_UP && player->currentDirection != SNAKE_DOWN) // pressed up key, skip if direction is down
				{
					player->pendingDirection = SNAKE_UP; 
				}
				else if (pressed_key == SDLK_LEFT && player->currentDirection != SNAKE_RIGHT) // pressed left key, skip if direction is right
				{
					player->pendingDirection = SNAKE_LEFT; 
				}
				else if (pressed_key == SDLK_DOWN && player->currentDirection != SNAKE_UP) // pressed down key, skip if direction is up
				{
					player->pendingDirection = SNAKE_DOWN;
				}
			}
		}
		
		if (return_code != 0)
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
				is_running = false;
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
		
		Uint64 current_time = SDL_GetTicks64();
		if (current_time >= next_frame_time) 
		{
			next_frame_time = current_time + (1000 / frames_per_second);
			
			// set buffer as render target and clear frame
			if (SDL_SetRenderTarget(renderer, buffer) != 0 || SDL_SetRenderDrawColor(renderer, 0xad, 0xd8, 0xe6, 0xff) != 0 || SDL_RenderClear(renderer) != 0)
			{
				PrintError();
				return_code = -2;
				break;
			}
			
			// draw snake play area
			if (SDL_SetRenderDrawColor(renderer, 0xe0, 0xb0, 0xff, 0xff) != 0 || SDL_RenderFillRect(renderer, & (SDL_Rect) {0, 0, 800, 800}) != 0)
			{
				PrintError();
				return_code = -2;
				break;
			}


			// render food and snake, copy to renderer
			if (!(RenderFood(renderer, apple, 0, 0, 20, 20) && RenderSnake(renderer, player, 0, 0, 20, 20)) || SDL_SetRenderTarget(renderer, NULL) != 0 || SDL_RenderCopy(renderer, buffer, NULL, NULL) != 0)
			{
				PrintError();
				return_code = -2;
				break;
			}

			SDL_RenderPresent(renderer);
		}
		
		if (is_paused)
		{	
			Uint64 timeBeforePause = SDL_GetTicks64();
			return_code = Pause(window, renderer, L, buffer);
			SDL_Log("Exit Pause: %d", return_code);
			if (return_code == 0)
			{
				player->nextMoveTime += SDL_GetTicks64() - timeBeforePause;
			}
			else if (return_code > 0)
			{
				--return_code;
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

	return return_code;
}

int Pause(SDL_Window *window, SDL_Renderer *renderer, lua_State *L, SDL_Texture *buffer)
{	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Get the window bounds. Set the buffer to blend mode. Get the frames per second.
	 */

	int window_width, window_height;
	SDL_GetWindowSize(window, &window_width, &window_height);
	
	if (SDL_SetTextureBlendMode(buffer, SDL_BLENDMODE_BLEND) != 0 || SDL_SetTextureAlphaMod(buffer, 75) != 0)
	{
		PrintError();
		return -2;
	}

	lua_getglobal(L, "frames_per_second");
	int frames_per_second = (int) lua_tonumber(L, -1);
	lua_pop(L, 1);


	// main loop
	int return_code = 0;
	bool is_running = true;
	while (is_running)
	{
		SDL_Event event;

		// clear frame
		if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0) != 0 || SDL_RenderClear(renderer) != 0)
		{
			PrintError();
			return_code = -2;
			break;
		}
		
		// set the earliest time the next frame occurs
		Uint64 next_frame_time = SDL_GetTicks64() + (1000 / frames_per_second);
		
		// poll events
		while (SDL_PollEvent(&event))
		{
			if (SDL_QUIT == event.type) // if quit, stop event poll and exit main loop
			{
				return_code = -1;
				break;
			}
			else if (SDL_KEYDOWN == event.type) // if key pressed down, handle it!
			{
				SDL_Keycode pressed_key = event.key.keysym.sym;

				// log the name of pressed key
				SDL_Log("%s", SDL_GetKeyName(pressed_key));

				if (pressed_key == SDLK_ESCAPE)
				{
					is_running = false;
				}
			}
		}

		if (return_code != 0)
		{
			break;
		}
		
		// draw frame
		if (SDL_SetRenderTarget(renderer, buffer) != 0 || SDL_SetRenderTarget(renderer, NULL) != 0 || SDL_RenderCopy(renderer, buffer, NULL, NULL))
		{
			PrintError();
			return_code = -2;
			break;
		}
				
		// wait and present next frame
		while (SDL_GetTicks64() < next_frame_time);
		SDL_RenderPresent(renderer);
	}

	if (SDL_SetTextureBlendMode(buffer, SDL_BLENDMODE_NONE) != 0 || SDL_SetTextureAlphaMod(buffer, 0xFF) != 0 || SDL_SetRenderTarget(renderer, NULL) != 0)
	{
		PrintError();
		return_code = -2;
	}
	
	SDL_Log("%d", return_code);

	return return_code;
}
