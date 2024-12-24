/**
 * @file
 * @author Danielle Raine
 * @date 2024-10-24
 * @brief Provides functions and structures for creating, managing, and freeing textures used in the game.
 *
 * Detailed description goes here.
 *
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


/**
 * @brief Brief description goes here.
 *
 * Detailed description goes here.
 */
typedef struct Texture
{
	SDL_Rect *bounds;	/**< Rectangle the @c SDL_Texture is bounded by. If @c NULL, the rendered @c SDL_Texture will span the @c SDL_Window. */
	SDL_Texture *texture;	/**< @c SDL_Texture that will be rendered. */
	char *texture_path;	/**< The file path to the file that will be loaded as a @c SDL_Texture */	
} Texture;


/**
 * @brief Creates a @c Texture.
 * @param renderer @c SDL_Renderer used to create @c SDL_Texture.
 * @param bounds @c SDL_Rect that bounds @c SDL_Texture.
 * @param texture_path Filepath to the texture.
 * @return @c Texture, @c NULL on error.
 */
Texture *CreateTexture(SDL_Renderer *renderer, const SDL_Rect *bounds, const char *texture_path);

/**
 * @brief Renders a @c Texture.
 * @param renderer @SDL_Renderer that will render @c SDL_Texture.
 * @param texture @Texture struct containing @c SDL_Texture.
 * @return @c bool, @c true on success, @c false on error.
 */
bool RenderTexture(SDL_Renderer *renderer, Texture *texture);

/**
 * @brief TODO add documentation
 */
bool RenderTextures(SDL_Renderer *renderer, Texture **textures );

/**
 * @brief TODO add documentation
 */
void DestroyTexture(Texture *texture);

/**
 * @brief TODO add documentation
 */
void DestroyTextures(Texture **textures);


#endif
