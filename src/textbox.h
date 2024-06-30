#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Declare TextBox struct.
 */

typedef struct TextBox
{
	SDL_Rect body;
	SDL_Texture *texture;
} TextBox;

TextBox *CreateTextBox(SDL_Renderer *renderer, SDL_Rect *body, const char *text, TTF_Font *font, SDL_Color *color);
bool RenderTextBox(SDL_Renderer *renderer, TextBox *textbox);
void DestroyTextBox(TextBox *textbox);

#endif
