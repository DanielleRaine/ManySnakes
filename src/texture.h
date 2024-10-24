#ifndef TEXT_H
#define TEXT_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Declare Text struct.
 */

typedef struct Texture
{
	SDL_Rect box;
	SDL_Texture *texture;
} Texture;

typedef struct Textbox
{
	Texture *texture;
	int borderwidth;
	SDL_Color boxcolor;
	SDL_Color bordercolor;
	TTF_Font *font;
	SDL_Color fontcolor;
	const char *text;
	bool isUpdated;
} Textbox;

typedef struct Textbutton
{
	SDL_Rect mouseArea;
	Textbox *button;
	Textbox *buttonHighlighted;
	Textbox *buttonPressed;
} Textbutton;


Texture *CreateTexture(SDL_Renderer *renderer, SDL_Rect *box, const char *imagepath);
Textbox *CreateTextbox(SDL_Renderer *renderer, SDL_Rect *box, int borderwidth, SDL_Color *boxcolor, SDL_Color *bordercolor, TTF_Font *font, SDL_Color *fontcolor, const char *text);
bool RenderTexture(SDL_Renderer *renderer, Texture *texture);
bool RenderTextures(SDL_Renderer *renderer, Texture **textures, int size);
bool RenderTextbox(SDL_Renderer *renderer, Textbox *textbox);
bool RenderTextboxes(SDL_Renderer *renderer, Textbox **textboxes, int size);
void DestroyTexture(Texture *texture);
void DestroyTextures(Texture **textures, int size);
void DestroyTextbox(Textbox *textbox);
void DestroyTextboxes(Textbox **textboxes, int size);

Textbutton *CreateTextbutton(SDL_Rect *mouseArea, Textbox *button, Textbox *buttonHighlighted, Textbox *buttonPressed);
bool PressedTextbutton(Textbutton *textbutton);
bool RenderTextbutton(SDL_Renderer *renderer, Textbutton *textbutton, Uint32 mousestate, int x, int y);
//bool RenderButtons(SDL_Renderer *renderer, Button **buttons, int size);
void DestroyTextbutton(Textbutton *textbutton);
//void DestroyButtons(Button **buttons, int size);

#endif
