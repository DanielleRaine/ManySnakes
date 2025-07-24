#include "texture.h"


bool RenderTexture(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Rect *srcrect, const SDL_Rect *dstrect)
{
	if (SDL_RenderCopy(renderer, texture, srcrect, dstrect) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
		return false;
	}

	return true;
}

static int L_RenderTexture(lua_State *L)
{
	SDL_Log("Bingus");

	lua_getfield(L, LUA_REGISTRYINDEX, "resource_manager");
	ResourceManager *manager = lua_touserdata(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, LUA_REGISTRYINDEX, "renderer");
	SDL_Renderer *renderer = lua_touserdata(L, -1);
	lua_pop(L, 1);

	//TODO Check if errors!
	// int filepath_len;
	
	lua_getfield(L, 1, "filepath");
	// const char *filepath = lua_getlstring(L, -1, &filepath_len);
	const char *filepath = lua_tolstring(L, -1, NULL);
	lua_pop(L, 1);

	lua_getfield(L, 1, "position");
	lua_getfield(L, -1, "x");
	lua_getfield(L, -2, "y");

	int x = (int) lua_tonumber(L, -2);
	int y = (int) lua_tonumber(L, -1);
	lua_pop(L, 3);

	lua_getfield(L, 1, "dimensions");
	lua_getfield(L, -1, "width");
	lua_getfield(L, -2, "height");
	
	int width = (int) lua_tonumber(L, -2);
	int height = (int) lua_tonumber(L, -1);
	lua_pop(L, 3);

	const SDL_Rect dstrect = {x, y, width, height};

	SDL_Texture *texture = GetTextureResource(manager, renderer, filepath);

	RenderTexture(renderer, texture, NULL, &dstrect);

	return 0;
}

static const struct luaL_Reg ManySnakesTextures [] =
{
	{"RenderTexture", L_RenderTexture},
	{NULL, NULL}
};

int luaopen_ManySnakesTextures(lua_State *L)
{
	luaL_newlib(L, ManySnakesTextures);
	return 1;
}

//Texture *CreateTexture(SDL_Renderer *renderer, const SDL_Rect *bounds, const char *texture_path)
//{
//	// Create the struct.
//	Texture *texture = malloc(sizeof(Texture));
//	if (!texture)
//	{
//		SDL_SetError("Failed to create texture. (malloc texture)");
//		return NULL;
//	}
//
//	texture->texture = IMG_LoadTexture(renderer, texture_path);
//	if (!texture->texture)
//	{
//		SDL_SetError("Failed to create texture. (Failed to load texture from %s)", texture_path);
//		free(texture);
//		return NULL;
//	}
//
//	texture->texture_path = (char*) calloc(strlen(texture_path) + 1, sizeof(char));
//	strcpy(texture->texture_path, texture_path);
//
//	if (bounds)
//	{
//		texture->bounds = (SDL_Rect*) malloc(sizeof(SDL_Rect));
//		if (!texture->bounds)
//		{
//			SDL_SetError("Failed to create texture. (malloc bounds)");
//			SDL_DestroyTexture(texture->texture);
//			free(texture);
//
//			return NULL;
//		}
//
//		*texture->bounds = *bounds;
//	}
//	else
//	{	
//		texture->bounds = NULL;
//	}
//
//	return texture;
//}
//
//bool RenderTexture(SDL_Renderer *renderer, Texture *texture)
//{
//	if (SDL_RenderCopy(renderer, texture->texture, NULL, texture->bounds))
//	{
//		SDL_SetError("Failed to render texture. (RenderTexture)");
//		return false;
//	}
//
//	return true;
//}
//
//bool RenderTextures(SDL_Renderer *renderer, Texture **textures)
//{
//	int size = sizeof(textures) / sizeof(Texture);
//	for (int i = 0; i < size ; ++i)
//	{
//		if (!RenderTexture(renderer, textures[i]))
//		{
//			SDL_ClearError();
//			SDL_SetError("Failed to render texture %d. (RenderTextures)", i);
//			return false;
//		}
//	}
//
//	return true;
//}
//
//void DestroyTexture(Texture *texture)
//{
//	SDL_DestroyTexture(texture->texture);
//	free(texture->bounds);
//	free(texture);
//}
//
//void DestroyTextures(Texture **textures)
//{
//	int size = sizeof(textures) / sizeof(Texture);
//	for (int i = 0; i < size; ++i)
//	{
//		DestroyTexture(textures[i]);
//	}
//}
