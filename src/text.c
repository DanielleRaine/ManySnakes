#include "text.h"


Text *CreateText(SDL_Renderer *renderer, SDL_Rect *box, TTF_Font *font, SDL_Color *color, const char *content)
{
	Text *text = malloc(sizeof(Text));
	if (!text)
	{
		SDL_SetError("Failed to create text :c (malloc)");
		return NULL;
	}

	SDL_Surface *surface = TTF_RenderText_Solid(font, content, *color);
	if (!surface)
	{
		SDL_SetError("Failed to create text :c (text)");
		free(text);
		return NULL;
	}

	text->texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!text->texture)
	{
		SDL_SetError("Failed to create text :c (texture)");
		SDL_FreeSurface(surface);
		free(text);
		return NULL;
	}

	SDL_FreeSurface(surface);
	text->box = *box;

	return text;
}

bool RenderText(SDL_Renderer *renderer, Text *text)
{
	if (SDL_RenderCopy(renderer, text->texture, NULL, &text->box))
	{
		SDL_SetError("Failed to render text :c (render)");
		return false;
	}

	return true;
}

bool RenderTexts(SDL_Renderer *renderer, Text **texts, int size)
{
	for (int i = 0; i < size; ++i)
	{
		if (!RenderText(renderer, texts[i]))
			return false;
	}

	return true;
}

void DestroyText(Text *text)
{
	SDL_DestroyTexture(text->texture);
	free(text);
}

void DestroyTexts(Text **texts, int size)
{
	for (int i = 0; i < size; ++i)
	{
		DestroyText(texts[i]);
	}
}

Button *CreateButton(Text *text, SDL_Color *color)
{
	Button *button = malloc(sizeof(Button));
	if (!button)
	{
		return NULL;
	}

	button->color = *color;
	button->text = text;

	return button;
}

bool RenderButton(SDL_Renderer *renderer, Button *button)
{
	Uint8 r, g, b, a;
	if (SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a) != 0)
	{
		return false;
	}

	SDL_Point cursor;
	SDL_GetMouseState(&cursor.x, &cursor.y);
	SDL_Color color;

	if (SDL_PointInRect(&cursor, &button->border))
	{
		color = button->highlight;
	}
	else
	{
		color = button->color;
	}

	if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) != 0 || SDL_RenderFillRect(renderer, &button->border) != 0)
	{
		return false;
	}

	//FIXME Add checl
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	return true;
}

void DestroyButton(Button *button)
{
	free(button);
}
