/**
 * @file
 * @author Danielle Raine
 * @date 2024-12-19
 * @brief Defines user interface structures, such as text boxes and buttons, and functions to manage them.
 *
 * Detailed description goes here.
 *
 */

#ifndef UI_H
#define UI_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "texture.h"


/**
 * @brief TODO add documentation
 */
typedef struct TextBox
{
	Texture *texture;	/**< @c Texture of the text box. */
	Texture *text_texture;	/**< @c Texture of the text to be rendered. */
	char *text;		/**< Text to be rendered */
	int text_speed;		/**< Interval of miliseconds that the @b user must call @c NextCharTextBox to add the next character to be rendered. If @p text_speed is @c 0, then all of the text is rendered at once and @c NextCharTextBox should not be called. */
	int render_text_length;	/**< Number of characters from @p text to be rendered. */
	int total_text_length;	/**< The number of total characters that will be rendered. If the length of @p text is smaller, or if @p render_num_chars is smaller, then the remainder rendered will be spaces. */
} TextBox;

/**
 * @brief TODO add documentation
 */
typedef struct Button
{
	Texture *button;
	Texture *highlighted_button;
	SDL_Color highlighted_color;
	Texture *pressed_button;
	SDL_Color pressed_color;
	bool is_pressed;
}


/**
 * @brief TODO add documentation
 */
TextBox *CreateTextBox(SDL_Renderer *renderer, const SDL_Rect *bounds, const char *texture_path, TTF_Font *font, const SDL_Color *font_color, const char *text);

/**
 * @brief TODO add documentation
 */
bool RenderTextBox(SDL_Renderer *renderer, TextBox *text_box);

/**
 * @brief TODO add documentation
 */
bool RenderTextBoxes(SDL_Renderer *renderer, TextBox **text_boxes, int size);

/**
 * @brief TODO add documentation
 */
void DestroyTextBox(TextBox *text_box);

/**
 * @brief TODO add documentation
 */
void DestroyTextBoxes(TextBox **text_boxes, int size);


/**
 * @brief TODO add documentation
 */
//Button *CreateButton(SDL_Renderer *renderer, const SDL_Rect *bounds, const char *texture_path, TTF_Font *font, const SDL_Color *font_color, const char *text);

/**
 * @brief TODO add documentation
 */
//bool AddHighlightedButton(SDL_Renderer *renderer, Button *button, const SDL_Rect *bounds, const char *texture_path, TTF_Font *font, const SDL_Color *font_color, const char *text);

/**
 * @brief TODO add documentation
 */
//bool PressedTextbutton(Textbutton *textbutton);

/**
 * @brief TODO add documentation
 */
//bool RenderTextbutton(SDL_Renderer *renderer, Textbutton *textbutton, Uint32 mousestate, int x, int y);

/**
 * @brief TODO add documentation
 */
//bool RenderButtons(SDL_Renderer *renderer, Button **buttons, int size);

/**
 * @brief TODO add documentation
 */
//void DestroyTextbutton(Textbutton *textbutton);

/**
 * @brief TODO add documentation
 */
//void DestroyButtons(Button **buttons, int size);


#endif
