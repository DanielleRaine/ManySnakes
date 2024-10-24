#include "texture.h"


Texture *CreateTexture(SDL_Renderer *renderer, SDL_Rect *box, const char *imagepath)
{
	// Create the struct.
	Texture *texture = malloc(sizeof(Texture));
	if (!texture)
	{
		SDL_SetError("Failed to create texture. (Failed to create Texture struct)");
		return NULL;
	}

	// Create the texture if imagepath specified.
	if (imagepath)
	{
		texture->texture = IMG_LoadTexture(renderer, imagepath);

		if (!texture->texture)
		{
			SDL_SetError("Failed to create texture. (Texture failed to load from imagepath)");
			free(texture);
			return NULL;
		}
	}
	else // Otherwise, set to NULL.
	{
		texture->texture = NULL;
	}

	// Set the position of the texture.
	texture->box = *box;

	return texture;
}

Textbox *CreateTextbox(SDL_Renderer *renderer, SDL_Rect *box, int borderwidth, SDL_Color *boxcolor, SDL_Color *bordercolor, TTF_Font *font, SDL_Color *fontcolor, const char *text)
{
	// Create the struct.
	Textbox *textbox = malloc(sizeof(Textbox));
	if (!textbox)
	{
		SDL_SetError("Failed to create textbox. (Failed to create Textbox struct)");
		return NULL;
	}

	// Create the texture struct.
	textbox->texture = CreateTexture(renderer, box, NULL);
	if (!textbox->texture)
	{
		SDL_ClearError();
		SDL_SetError("Failed to create textbox. (Failed to create Texture)");
		free(textbox);
		return NULL;
	}

	// Create the text surface.
	SDL_Surface *surface = TTF_RenderText_Solid(font, text, *fontcolor);
	if (!surface)
	{
		SDL_SetError("Failed to create textbox. (Failed to create text SDL_Surface)");
		free(textbox->texture);
		free(textbox);
		return NULL;
	}

	// Create the text texture.
	textbox->texture->texture = SDL_CreateTextureFromSurface(renderer, surface);
	// Free the text surface.	
	SDL_FreeSurface(surface);
	if (!textbox->texture->texture)
	{
		SDL_SetError("Failed to create textbox. (Failed to create text SDL_Texture from SDL_Surface)");
		free(textbox->texture);
		free(textbox);
		return NULL;
	}

	textbox->texture->box = *box;
	textbox->borderwidth = borderwidth;
	textbox->boxcolor = *boxcolor;
	textbox->bordercolor = *bordercolor;
	textbox->font = font;
	textbox->fontcolor = *fontcolor;
	textbox->text = text;
	textbox->isUpdated = false;

	return textbox;
}

bool RenderTexture(SDL_Renderer *renderer, Texture *texture)
{
	if (SDL_RenderCopy(renderer, texture->texture, NULL, &texture->box))
	{
		SDL_SetError("Failed to render texture. (texture)");
		return false;
	}

	return true;
}

bool RenderTextures(SDL_Renderer *renderer, Texture **textures, int size)
{
	for (int i = 0; i < size; ++i)
	{
		if (!RenderTexture(renderer, textures[i]))
		{
			SDL_ClearError();
			SDL_SetError("Failed to render texture %d. (texture)", i);
			return false;
		}
	}

	return true;
}

bool RenderTextbox(SDL_Renderer *renderer, Textbox *textbox)
{	
	int borderwidth = textbox->borderwidth;
	SDL_Color color = textbox->bordercolor;
	SDL_Rect borderbox = {textbox->texture->box.x - borderwidth, textbox->texture->box.y - borderwidth, textbox->texture->box.w + borderwidth * 2, textbox->texture->box.h + borderwidth * 2};

	if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) != 0 || SDL_RenderFillRect(renderer, &borderbox) != 0)
	{
		SDL_SetError("Failed to render textbox. (Border failed to render)");
		return false;
	}

	color = textbox->boxcolor;

	if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) != 0 || SDL_RenderFillRect(renderer, &textbox->texture->box) != 0)
	{
		SDL_SetError("Failed to render textbox. (Box failed to render)");
		return false;
	}


	if (!RenderTexture(renderer, textbox->texture))
	{
		SDL_ClearError();
		SDL_SetError("Failed to render textbox. (Texture failed to render)");
		return false;
	}

	return true;
}

bool RenderTextboxes(SDL_Renderer *renderer, Textbox **textboxes, int size)
{
	for (int i = 0; i < size; ++i)
	{
		if (!RenderTextbox(renderer, textboxes[i]))
		{
			SDL_ClearError();
			SDL_SetError("Failed to render textbox %d. (textbox)", i);
			return false;
		}
	}

	return true;
}

void DestroyTexture(Texture *texture)
{
	SDL_DestroyTexture(texture->texture);
	free(texture);
}

void DestroyTextures(Texture **textures, int size)
{
	for (int i = 0; i < size; ++i)
	{
		DestroyTexture(textures[i]);
	}
}

void DestroyTextbox(Textbox *textbox)
{
	DestroyTexture(textbox->texture);
	free(textbox);
}

void DestroyTextboxes(Textbox **textboxes, int size)
{
	for (int i = 0; i < size; ++i)
	{
		DestroyTextbox(textboxes[i]);
	}
}

Textbutton *CreateTextbutton(SDL_Rect *mouseArea, Textbox *button, Textbox *buttonHighlighted, Textbox *buttonPressed)
{
	Textbutton *textbutton = malloc(sizeof(Textbutton));
	if (!textbutton)
	{
		return NULL;
	}

	textbutton->mouseArea = *mouseArea;
	textbutton->button = button;
	textbutton->buttonHighlighted = buttonHighlighted;
	textbutton->buttonPressed = buttonPressed;

	return textbutton;
}

bool RenderTextbutton(SDL_Renderer *renderer, Textbutton *textbutton, Uint32 mousestate, int x, int y)
{
	SDL_Point cursorPos = {x, y};
	Textbox *button;
	if (!SDL_PointInRect(&cursorPos, &textbutton->mouseArea))
	{
		button = textbutton->button;
	}
	else if (mousestate == SDL_BUTTON_LEFT)
	{
		button = textbutton->buttonPressed;
	}
	else
	{
		button = textbutton->buttonHighlighted;
	}

	if (!button)
	{
		return true;
	}

	if (!RenderTextbox(renderer, button))
	{
		SDL_ClearError();
		SDL_SetError("Failed to render textbutton. (Textbox failed to render)");
		return false;
	}

	return true;
}

void DestroyTextbutton(Textbutton *textbutton)
{
	free(textbutton);
}
