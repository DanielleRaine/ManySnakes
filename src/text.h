#ifndef TEXT_H
#define TEXT_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Declare Text struct.
 */

typedef struct Text
{
	SDL_Rect box;
	SDL_Texture *texture;
} Text;

typedef struct Button
{
	SDL_Rect border;
	SDL_Color color;
	SDL_Color highlight;
	Text *text;
} Button;

Text *CreateText(SDL_Renderer *renderer, SDL_Rect *box, TTF_Font *font, SDL_Color *color, const char *content);
bool RenderText(SDL_Renderer *renderer, Text *text);
bool RenderTexts(SDL_Renderer *renderer, Text **texts, int size);
void DestroyText(Text *text);
void DestroyTexts(Text **texts, int size);

Button *CreateButton(Text *text, SDL_Rect *border, SDL_Color *color, SDL_Color *highlight);
//bool PressedButton(Button *button, SDL_MouseButtonEvent)
bool RenderButton(SDL_Renderer *renderer, Button *button);
//bool RenderButtons(SDL_Renderer *renderer, Button **buttons, int size);
void DestroyButton(Button *button);
//void DestroyButtons(Button **buttons, int size);

#endif
