/* ManyS  nakes by Danielle Raine
 * Start Date: June 8th, 2024
 */

#include "config.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


int main(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Declare constants and enums.
	 */

	typedef enum
	{
		RIGHT = 'r',
		UP = 'u',
		LEFT = 'l',
		DOWN = 'd'
	} Direction;

	typedef enum
	{
		APPLE = 'a'
	} FoodType;


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Declare snake, snake body structs.
	 */	

	typedef struct SnakeNode
	{
		struct SnakeNode *next;
		int xPos;
		int yPos;
	} SnakeNode;

	typedef struct 
	{
		SnakeNode *head;
		Uint64 speed;	// how many milliseconds it takes to move one square
		Direction currentDirection;	// current direction of the snake
		Direction pendingDirection;	// direction for the next move of the snake
		Uint64 lastMoveTime;	// time in milliseconds since last move
		Uint64 nextMoveTime;	// time in milliseconds until next move
		char name[];	// name of the snake
	} Snake;

	typedef struct
	{
		FoodType type;
		int xPos;
		int yPos;
		char filepath[];
	} Food;


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

	Snake *player = malloc(sizeof(Snake));
	
	// set the speed and direction of the player
	player->speed = 250;
	player->currentDirection = UP;
	player->pendingDirection = UP;
	
	// create the head and its position
	player->head = malloc(sizeof(SnakeNode));
	player->head->next = NULL;
	player->head->xPos = WINDOW_WIDTH / 2;
	player->head->yPos = WINDOW_HEIGHT / 2;

	SnakeNode *curPlayerNode = player->head;

	// create body
	for (int i = 0; i < 2; i++)
	{
		int xPosNext = curPlayerNode->xPos;
		int yPosNext = curPlayerNode->yPos + MOVE_H;

		curPlayerNode->next = malloc(sizeof(SnakeNode));
		curPlayerNode = curPlayerNode->next;
		curPlayerNode->next = NULL;
		curPlayerNode->xPos = xPosNext;
		curPlayerNode->yPos = yPosNext;
	}
	
	// seed rand
	srand(SDL_GetTicks());

	// set player mover times
	player->lastMoveTime = SDL_GetTicks64();
	player->nextMoveTime = player->lastMoveTime + player->speed;


	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Main loop, poll for events!
	 */

	SDL_Event event;
	bool isRunning = true;
	// main loop	
	while (isRunning)
	{
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
		 * Player's snake moves every NEXT_FRAME, so if current time is greater than or equal to the next
		 * time the snake is supposed to move, update the position of the snake depending on the direction 
		 * that the player pressed.
		 */

		Uint64 timeNow = SDL_GetTicks64();

		if (timeNow >= player->nextMoveTime)
		{
			player->nextMoveTime = timeNow + player->speed;

			int xPosDelta, yPosDelta;
			// update direction
			player->currentDirection = player->pendingDirection;

			SDL_Log("%c\n", player->currentDirection);
			
			// displace the snake depending on the direction it was set to move
			switch (player->currentDirection)
			{
				case RIGHT:
					xPosDelta = MOVE_W;
					yPosDelta = 0;
					break;
				case UP:
					xPosDelta = 0;
					yPosDelta = -MOVE_H;
					break;
				case LEFT:
					xPosDelta = -MOVE_W;
					yPosDelta = 0;
					break;
				case DOWN:
					xPosDelta = 0;
					yPosDelta = MOVE_H;
					break;
				default:
					SDL_Log("Huh??");
			}

			curPlayerNode = player->head;
			int xPosNext = curPlayerNode->xPos + xPosDelta;
			int yPosNext = curPlayerNode->yPos + yPosDelta;

			// if the snake is out of bounds, loop to other side
			if (xPosNext < 0) // snake went left out of bounds
			{
				xPosNext += WINDOW_WIDTH;
			}

			if (xPosNext >= WINDOW_WIDTH) // snake went right out of bounds
			{
				xPosNext = 0;
			}

			if (yPosNext < 0) // snake went up out of bounds
			{
				yPosNext += WINDOW_HEIGHT;
			}

			if (yPosNext >= WINDOW_HEIGHT) // snake went down out of bounds
			{
				yPosNext = 0;
			}

			// update body of the snake to move it
			while (curPlayerNode)
			{
				int xPosOld = curPlayerNode->xPos;
				int yPosOld = curPlayerNode->yPos;
				curPlayerNode->xPos = xPosNext;
				curPlayerNode->yPos = yPosNext;
				xPosNext = xPosOld;
				yPosNext = yPosOld;
				curPlayerNode = curPlayerNode->next;
			}
		}


		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		 * Draw the snake and wait, wait until next frame, draw frame.
		 */
		
		curPlayerNode = player->head;
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		while (curPlayerNode)
		{
			// SDL_SetRenderDrawColor(renderer, rand() % 256, rand() % 256, rand() % 256, 255);
			SDL_Rect drawRect = {curPlayerNode->xPos, curPlayerNode->yPos, MOVE_W, MOVE_H};
			SDL_RenderFillRect(renderer, &drawRect);
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

	curPlayerNode = player->head;
	while (curPlayerNode)
	{
		 SnakeNode *playerNextNode = curPlayerNode->next;
		 free(curPlayerNode);
		 curPlayerNode = playerNextNode;
	}

	// if you really love them, let them go
	free(player);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	// print sdl subsystem initialization flags
	// printf("%X\n\n", SDL_WasInit(SDL_INIT_EVERYTHING));

	return 0;
}
