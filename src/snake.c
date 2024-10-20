#include "snake.h"


Snake *CreateSnake(int xPos, int yPos, int nodeWidth, int nodeHeight, Uint64 speed, int length, SnakeDirection direction, SDL_Color *color)
{
	// a snake cannot have zero length. if less than 1, return NULL
	if (length < 1)
    	{
		return NULL;
    	}

	// create snake struct and set speed and direction
    	Snake *snake = malloc(sizeof(Snake));

	if (!snake)
	{
		return NULL;
	}

	snake->speed = speed;
	snake->currentDirection = snake->pendingDirection = direction;

	// set shape of each SnakeNode
	snake->nodeWidth = nodeWidth;
	snake->nodeHeight = nodeHeight;

	// create snake head and set starting position and size of each node
    	snake->head = snake->tail = malloc(sizeof(SnakeNode));    
	if (!snake->head)
	{
		free(snake);
		return NULL;
	}

    	snake->head->xPos = xPos;
    	snake->head->yPos = yPos;
	snake->length = length;

	// create the rest of the snake
    	for (int i = 1; i < length; ++i)
    	{
		//FIXME check snake node malloc
    	    	snake->tail->next = malloc(sizeof(SnakeNode));
    	    	snake->tail->next->xPos = snake->tail->xPos;
    	    	snake->tail->next->yPos = snake->tail->yPos + 1;
    	    	snake->tail = snake->tail->next;
    	} //FIXME Make it so that snake can start in different directions.
    	snake->tail->next = NULL;

	snake->color = *color;

    	return snake;
}

void StepSnake(Snake *snake, int xMax, int yMax)
{
    	snake->currentDirection = snake->pendingDirection;
    	int xDelta, yDelta;
    	// displace the snake depending on the direction it was set to move
    	switch (snake->currentDirection)
    	{
		case SNAKE_RIGHT:
			xDelta = 1;
    	        	yDelta = 0;
    	        	break;
    	    	case SNAKE_UP:
    	    	    	xDelta = 0;
    	    	    	yDelta = -1;
    	    	    	break;
    	    	case SNAKE_LEFT:
    	    	    	xDelta = -1;
    	    	    	yDelta = 0;
    	    	    	break;
    	    	case SNAKE_DOWN:
    	    	    	xDelta = 0;
    	    	    	yDelta = 1;
    	    	    	break;
    	    	default:
    	    	    	return;
    	}
    	SnakeNode *cur = snake->head;
    	int xNext = cur->xPos + xDelta;
    	int yNext = cur->yPos + yDelta;

    	// if the snake is out of bounds, loop to other side
    	if (xNext < 0) // snake went left out of bounds
    	{
    	    	xNext += xMax;
    	}

    	if (xNext >= xMax) // snake went right out of bounds
    	{
    	    	xNext = 0;
    	}

    	if (yNext < 0) // snake went up out of bounds
    	{
    	    	yNext += yMax;
    	}

    	if (yNext >= yMax) // snake went down out of bounds
    	{
    	    	yNext = 0;
    	}

    	// update each node of snake to move it
    	while (cur)
    	{
    	    	int xLast = cur->xPos;
    	    	int yLast = cur->yPos;
    	    	cur->xPos = xNext;
    	    	cur->yPos = yNext;
    	    	xNext = xLast;
    	    	yNext = yLast;
    	    	cur = cur->next;
    	}
}

void GrowSnake(Snake *snake, int xNew, int yNew)
{
	snake->tail->next = malloc(sizeof(SnakeNode));
	//FIXME Add validation for malloc.
	snake->tail = snake->tail->next;
	snake->tail->xPos = xNew;
	snake->tail->yPos = yNew;
	snake->tail->next = NULL;
	++snake->length;
}

bool CheckCollisionSnake(Snake *snake)
{
	// a snake of less than 5 nodes cannot collide with itself
	if (snake->length < 5)
	{
		return false;
	}

	// a snake of minimum 5 nodes has a 5th node to start from
	SnakeNode *cur = snake->head->next->next->next->next;
	while (cur)
	{
		// if the position of a node is the same as the head, then collision
		if (cur->xPos == snake->head->xPos && cur->yPos == snake->head->yPos)
		{
			return true;
		}

		cur = cur->next;
	}

	return false;
}

bool RenderSnake(SDL_Renderer *renderer, Snake *snake, int xOrigin, int yOrigin, int xMultiplier, int yMultiplier)
{
	SnakeNode *cur = snake->head;

	if (SDL_SetRenderDrawColor(renderer, snake->color.r, snake->color.g, snake->color.b, snake->color.a) != 0)
	{
		return false;
	}

	while (cur)
	{
		SDL_Rect rect = {xOrigin + cur->xPos * xMultiplier, yOrigin + cur->yPos * yMultiplier, snake->nodeWidth, snake->nodeHeight};
		if (SDL_RenderFillRect(renderer, &rect) != 0)
		{
			return false;
		}
		cur = cur->next;
	}

	return true;
}

void DestroySnake(Snake *snake)
{
	// loop through every SnakeNode and free it
    	while (snake->head)
    	{
    	    	SnakeNode *next = snake->head->next;
    	    	free(snake->head);
    	    	snake->head = next;
    	}

	// free Snake struct
    	free(snake);
}

Food *CreateFood(SDL_Renderer *renderer, FoodType type, int xPos, int yPos, int textureWidth, int textureHeight, const char *filepath)
{
	Food *food = malloc(sizeof(Food));
	//FIXME Validate food malloc.
	food->type = type;
	food->texture = IMG_LoadTexture(renderer, filepath);
	if (!(food && filepath))
	{
		free(food);
		return NULL;
	}

	food->xPos = xPos;
	food->yPos = yPos;
	food->textureWidth = textureWidth;
	food->textureHeight = textureHeight;

	return food;
}

void RandPosFood(Food *food, Snake *snake, int xMax, int yMax)
{
	//FIXME Check if snake spans entire screen.
	
	bool validPos = false;
	while (!validPos)
	{
		//FIXME I should probably set these to a temporary variable just in case.
		// get a random x and y position in a grid layout
		food->xPos = rand() % xMax;
		food->yPos = rand() % yMax;

		validPos = true;
		SnakeNode *cur = snake->head;
		while(cur)
		{
			// if the food exists in the same place as the snake, find new random position
			if (food->xPos == cur->xPos && food->yPos == cur->yPos) // I fixed a bug here! Can you guess it? :)
			{
				validPos = false;
				break;
			}

			cur = cur->next;
		}
	}
}

bool RenderFood(SDL_Renderer *renderer, Food *food, int xOrigin, int yOrigin, int xMultiplier, int yMultiplier)
{
	SDL_Rect rect = {xOrigin + food->xPos * xMultiplier, yOrigin + food->yPos * yMultiplier, food->textureWidth, food->textureHeight};
	if (SDL_RenderCopy(renderer, food->texture, NULL, &rect) != 0)
	{
		return false;
	}

	return true;	
}

void DestroyFood(Food *food)
{
	SDL_DestroyTexture(food->texture);
	free(food);
}

