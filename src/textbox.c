#include "textbox.h"

TextBox *CreateTextBox(SDL_Renderer *renderer, SDL_Rect *body, const char *text, TTF_Font *font, SDL_Color *color)
{
	TextBox *textbox = malloc(sizeof(TextBox));
	if (!textbox)
	{
		SDL_SetError("Failed to create textbox :c (malloc)");
		return NULL;
	}

	SDL_Surface *surface = TTF_RenderText_Solid(font, text, *color);
	if (!surface)
	{
		SDL_SetError("Failed to create textbox :c (text)");
		free(textbox);
		return NULL;
	}

	textbox->texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!textbox->texture)
	{
		SDL_SetError("Failed to create textbox :c (texture)");
		SDL_FreeSurface(surface);
		free(textbox);
		return NULL;
	}

	SDL_FreeSurface(surface);
	textbox->body = *body;

	return textbox;
}

bool RenderTextBox(SDL_Renderer *renderer, TextBox *textbox)
{
	if (SDL_RenderCopy(renderer, textbox->texture, NULL, &textbox->body))
	{
		SDL_SetError("Failed to render textbox :c (render)");
		return false;
	}

	return true;
}

void DestroyTextBox(TextBox *textbox)
{
	SDL_DestroyTexture(textbox->texture);
	free(textbox);
}
