#include "ui.c"


TextBox *CreateTextbox(SDL_Renderer *renderer, SDL_Rect *bounds, char *texture_path, TTF_Font *font, SDL_Color *font_color, char *text)
{
	// Create the struct.
	Textbox *textbox = malloc(sizeof(TextBox));
	if (!textbox)
	{
		SDL_SetError("Failed to create TextBox. (Failed to create Textbox struct)");
		return NULL;
	}

	// Create the texture struct.
	textbox->texture = CreateTexture(renderer, bounds, texture_path);
	if (!textbox->texture)
	{
		SDL_ClearError();
		SDL_SetError("Failed to create textbox. (Failed to create Texture)");
		free(textbox);
		return NULL;
	}

	// set the font color of the text
	if (!font_color) // set it to black if it is null
	{
		*textbox->font_color = & (SDL_Color) {0, 0, 0, 0xff};
	}
	else
	{
		*textbox->font_color = *font_color;
	}

	if (text)
	{
		textbox->text = text;
		
		// Create the text surface.
		SDL_Surface *surface = TTF_RenderText_Solid(font, text, *font_color);
		if (!surface)
		{
			SDL_SetError("Failed to create textbox. (Failed to create text SDL_Surface)");
			free(textbox->texture);
			free(textbox);
			return NULL;
		}

		// Create the text texture.
		textbox->text_texture = SDL_CreateTextureFromSurface(renderer, surface);
		// Free the text surface.	
		SDL_FreeSurface(surface);
		
		if (!textbox->text_texture)
		{
			SDL_SetError("Failed to create textbox. (Failed to create text SDL_Texture from SDL_Surface)");
			free(textbox->texture);
			free(textbox);
			return NULL;
		}
	}
	else
	{
		textbox->text = NULL;
		textbox->text_texture = NULL;
	}

	textbox->texture->bounds = *bounds;
	textbox->font = font;

	return textbox;
}


//bool RenderTextbox(SDL_Renderer *renderer, Textbox *textbox)
//{	
//	int borderwidth = textbox->borderwidth;
//	SDL_Color color = textbox->bordercolor;
//	SDL_Rect borderbox = {textbox->texture->box.x - borderwidth, textbox->texture->box.y - borderwidth, textbox->texture->box.w + borderwidth * 2, textbox->texture->box.h + borderwidth * 2};
//
//	if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) != 0 || SDL_RenderFillRect(renderer, &borderbox) != 0)
//	{
//		SDL_SetError("Failed to render textbox. (Border failed to render)");
//		return false;
//	}
//
//	color = textbox->boxcolor;
//
//	if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) != 0 || SDL_RenderFillRect(renderer, &textbox->texture->box) != 0)
//	{
//		SDL_SetError("Failed to render textbox. (Box failed to render)");
//		return false;
//	}
//
//
//	if (!RenderTexture(renderer, textbox->texture))
//	{
//		SDL_ClearError();
//		SDL_SetError("Failed to render textbox. (Texture failed to render)");
//		return false;
//	}
//
//	return true;
//}

//bool RenderTextboxes(SDL_Renderer *renderer, Textbox **textboxes, int size)
//{
//	for (int i = 0; i < size; ++i)
//	{
//		if (!RenderTextbox(renderer, textboxes[i]))
//		{
//			SDL_ClearError();
//			SDL_SetError("Failed to render textbox %d. (textbox)", i);
//			return false;
//		}
//	}
//
//	return true;
//}



//void DestroyTextbox(Textbox *textbox)
//{
//	DestroyTexture(textbox->texture);
//	free(textbox);
//}
//
//void DestroyTextboxes(Textbox **textboxes, int size)
//{
//	for (int i = 0; i < size; ++i)
//	{
//		DestroyTextbox(textboxes[i]);
//	}
//}
//
//Textbutton *CreateTextbutton(SDL_Rect *mouseArea, Textbox *button, Textbox *buttonHighlighted, Textbox *buttonPressed)
//{
//	Textbutton *textbutton = malloc(sizeof(Textbutton));
//	if (!textbutton)
//	{
//		return NULL;
//	}
//
//	textbutton->mouseArea = *mouseArea;
//	textbutton->button = button;
//	textbutton->buttonHighlighted = buttonHighlighted;
//	textbutton->buttonPressed = buttonPressed;
//
//	return textbutton;
//}
//
//bool RenderTextbutton(SDL_Renderer *renderer, Textbutton *textbutton, Uint32 mousestate, int x, int y)
//{
//	SDL_Point cursorPos = {x, y};
//	Textbox *button;
//	if (!SDL_PointInRect(&cursorPos, &textbutton->mouseArea))
//	{
//		button = textbutton->button;
//	}
//	else if (mousestate == SDL_BUTTON_LEFT)
//	{
//		button = textbutton->buttonPressed;
//	}
//	else
//	{
//		button = textbutton->buttonHighlighted;
//	}
//
//	if (!button)
//	{
//		return true;
//	}
//
//	if (!RenderTextbox(renderer, button))
//	{
//		SDL_ClearError();
//		SDL_SetError("Failed to render textbutton. (Textbox failed to render)");
//		return false;
//	}
//
//	return true;
//}
//
//void DestroyTextbutton(Textbutton *textbutton)
//{
//	free(textbutton);
//}
