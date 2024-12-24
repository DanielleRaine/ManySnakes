#include "texture.h"


Texture *CreateTexture(SDL_Renderer *renderer, const SDL_Rect *bounds, const char *texture_path)
{
	// Create the struct.
	Texture *texture = malloc(sizeof(Texture));
	if (!texture)
	{
		SDL_SetError("Failed to create texture. (malloc texture)");
		return NULL;
	}

	texture->texture = IMG_LoadTexture(renderer, texture_path);
	if (!texture->texture)
	{
		SDL_SetError("Failed to create texture. (Failed to load texture from %s)", texture_path);
		free(texture);
		return NULL;
	}

	texture->texture_path = (char*) calloc(strlen(texture_path) + 1, sizeof(char));
	strcpy(texture->texture_path, texture_path);

	if (bounds)
	{
		texture->bounds = (SDL_Rect*) malloc(sizeof(SDL_Rect));
		if (!texture->bounds)
		{
			SDL_SetError("Failed to create texture. (malloc bounds)");
			SDL_DestroyTexture(texture->texture);
			free(texture);

			return NULL;
		}
	}
	else
	{	
		texture->bounds = NULL;
	}

	return texture;
}

bool RenderTexture(SDL_Renderer *renderer, Texture *texture)
{
	if (SDL_RenderCopy(renderer, texture->texture, NULL, texture->bounds))
	{
		SDL_SetError("Failed to render texture. (RenderTexture)");
		return false;
	}

	return true;
}

bool RenderTextures(SDL_Renderer *renderer, Texture **textures)
{
	int size = sizeof(textures) / sizeof(Texture);
	for (int i = 0; i < size ; ++i)
	{
		if (!RenderTexture(renderer, textures[i]))
		{
			SDL_ClearError();
			SDL_SetError("Failed to render texture %d. (RenderTextures)", i);
			return false;
		}
	}

	return true;
}

void DestroyTexture(Texture *texture)
{
	SDL_DestroyTexture(texture->texture);
	free(texture->bounds);
	free(texture);
}

void DestroyTextures(Texture **textures)
{
	int size = sizeof(textures) / sizeof(Texture);
	for (int i = 0; i < size; ++i)
	{
		DestroyTexture(textures[i]);
	}
}
