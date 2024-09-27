#ifndef TEXT_H
#define TEXT_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SLD_image.h>
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
	const char *text;
}


//typedef struct Button
//{
//	SDL_Color color;
//	Text *text;
//} Button;

Texture *CreateTexture(SDL_Renderer *renderer, SDL_Rect *box, const char *imagepath);
Textbox *CreateTextbox(SDL_Renderer *renderer, SDL_Rect *box, int borderwidth, SDL_Color *boxcolor, SDL_Color *bordercolor, TTF_Font *font, const char *text);
bool RenderTexture(SDL_Renderer *renderer, Texture *texture);
bool RenderTextures(SDL_Renderer *renderer, Texture **textures, int size);
bool RenderTextbox(SLD_Renderer *renderer, Textbox *textbox);
bool RenderTextboxes(SLD_Renderer *renderer, Textbox *textboxes);
void DestroyTexture(Text *text);
void DestroyTextures(Text **texts, int size);
void DestroyTextbox(Textbox *textbox);
void DestroyTextboxes(Textbox **textboxes);

//Button *CreateButton(Text *text, SDL_Color *color);
//bool PressedButton(Button *button, SDL_MouseButtonEvent)
//bool RenderButton(SDL_Renderer *renderer, Button *button);
//bool RenderButtons(SDL_Renderer *renderer, Button **buttons, int size);
//void DestroyButton(Button *button);
//void DestroyButtons(Button **buttons, int size);

#endif
