#ifndef TEXT_H
#define TEXT_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Declare Text struct.
 */

typedef struct Texture
{
	int  ;
	SDL_Texture *texture;
} Text;

typedef struct Button
{
	SDL_Color color;
	Text *text;
} Button;

Text *CreateTextTexture(SDL_Renderer *renderer, SDL_Rect *, TTF_Font *font, SDL_Color *color, const char *content);
bool RenderTexture(SDL_Renderer *renderer, Text *text);
bool RenderTextures(SDL_Renderer *renderer, Text **texts, int size);
void DestroyTexture(Text *text);
void DestroyTextures(Text **texts, int size);

Button *CreateButton(Text *text, SDL_Color *color);
bool PressedButton(Button *button, SDL_MouseButtonEvent)
bool RenderButton(SDL_Renderer *renderer, Button *button);
//bool RenderButtons(SDL_Renderer *renderer, Button **buttons, int size);
void DestroyButton(Button *button);
//void DestroyButtons(Button **buttons, int size);

#endif
