#include "snake.h"

Snake *MNYSNKS_CreateSnake(Uint64 speed, Direction direction, int xPos, int yPos, int wStep, int hStep, int size)
{
    if (size < 1)
    {
        return NULL;
    }

    Snake *snake = malloc(sizeof(Snake));
    snake->speed = speed;
    snake->currentDirection = snake->pendingDirection = direction;
    
    snake->head = snake->tail = malloc(sizeof(SnakeNode));    
    snake->head->body = (SDL_Rect) {xPos, yPos, wStep, hStep};
    // *snake->head->body = (SDL_Rect) {x = xPos, y = yPos, w = wStep, h = hStep};
    // snake->head->body.x = xPos;
    // snake->head->body.y = yPos;
    // snake->head->body.w = wStep;
    // snake->head->body.h = hStep;

    for (int i = 1; i < size; ++i)
    {
        snake->tail->next = malloc(sizeof(SnakeNode));
        snake->tail->next->body = (SDL_Rect) {
            snake->tail->body.x,
            snake->tail->body.y + hStep,
            snake->tail->body.w,
            snake->tail->body.h,
        };
        snake->tail = snake->tail->next;
    }

    snake->tail->next = NULL;

    return snake;
}

void MNYSNKS_DestroySnake(Snake *snake)
{
    while (snake->head)
    {
        SnakeNode *nextSnakeNode = snake->head->next;
        free(snake->head);
        snake->head = nextSnakeNode;
    }

    free(snake);
}

void MNYSNKS_StepSnake(Snake *snake, int xLeftBound, int xRightBound, int yUpBound, int yDownBound)
{
    snake->currentDirection = snake->pendingDirection;
    int xPosDelta, yPosDelta;
    // displace the snake depending on the direction it was set to move
    switch (snake->currentDirection)
    {
        case RIGHT:
            xPosDelta = snake->head->body.w;
            yPosDelta = 0;
            break;
        case UP:
            xPosDelta = 0;
            yPosDelta = -snake->head->body.h;
            break;
        case LEFT:
            xPosDelta = -snake->head->body.w;
            yPosDelta = 0;
            break;
        case DOWN:
            xPosDelta = 0;
            yPosDelta = snake->head->body.h;
            break;
        default:
            return;
    }
    SnakeNode *curPlayerNode = snake->head;
    int xPosNext = curPlayerNode->body.x + xPosDelta;
    int yPosNext = curPlayerNode->body.y + yPosDelta;

    // if the snake is out of bounds, loop to other side
    if (xPosNext < xLeftBound) // snake went left out of bounds
    {
        xPosNext += xRightBound;
    }

    if (xPosNext >= xRightBound) // snake went right out of bounds
    {
        xPosNext = xLeftBound;
    }

    if (yPosNext < yUpBound) // snake went up out of bounds
    {
        yPosNext += yDownBound;
    }

    if (yPosNext >= yDownBound) // snake went down out of bounds
    {
        yPosNext = yUpBound;
    }

    // update body of the snake to move it
    while (curPlayerNode)
    {
        int xPosOld = curPlayerNode->body.x;
        int yPosOld = curPlayerNode->body.y;
        curPlayerNode->body.x = xPosNext;
        curPlayerNode->body.y = yPosNext;
        xPosNext = xPosOld;
        yPosNext = yPosOld;
        curPlayerNode = curPlayerNode->next;
    }
}