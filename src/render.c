#include "render.h"


bool RenderTexture(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Rect *srcrect, const SDL_Rect *dstrect)
{
	if (SDL_RenderCopy(renderer, texture, srcrect, dstrect) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s (render.c)", SDL_GetError());
		return false;
	}

	return true;
}

static int L_RenderTexture(lua_State *L)
{
	lua_getfield(L, LUA_REGISTRYINDEX, "resource_manager");
	ResourceManager *manager = lua_touserdata(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, LUA_REGISTRYINDEX, "renderer");
	SDL_Renderer *renderer = lua_touserdata(L, -1);
	lua_pop(L, 1);

	//TODO Check if errors!
	// int filepath_len;
	
	lua_getfield(L, 1, "filepath");
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
	// SDL_Log("%d", texture == NULL);
	RenderTexture(renderer, texture, NULL, &dstrect);

	return 0;
}

static int L_RenderButton(lua_State *L)
{
	lua_getfield(L, LUA_REGISTRYINDEX, "resource_manager");
	ResourceManager *manager = lua_touserdata(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, LUA_REGISTRYINDEX, "renderer");
	SDL_Renderer *renderer = lua_touserdata(L, -1);
	lua_pop(L, 1);

	//TODO Check if errors!
	// int filepath_len;
	
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
	
	int mouse_x, mouse_y;
	Uint32 flags = SDL_GetMouseState(&mouse_x, &mouse_y);
	lua_getfield(L, 1, "filepaths");

	if (x <= mouse_x && mouse_x <= x + width && y <= mouse_y && mouse_y <= y + height)
	{
		if ((flags & SDL_BUTTON_LEFT) != 0)
		{
			lua_getfield(L, -1, "click");
		}
		else
		{
			lua_getfield(L, -1, "hover");
		}
	}
	else
	{
		lua_getfield(L, -1, "button");
	}
		
	const char *filepath = lua_tolstring(L, -1, NULL);
	lua_pop(L, 2);

	SDL_Texture *texture = GetTextureResource(manager, renderer, filepath);
	RenderTexture(renderer, texture, NULL, &dstrect);

	return 0;
}

static const struct luaL_Reg ManySnakesRender [] =
{
	// {"RenderClear", L_RenderClear},
	{"RenderTexture", L_RenderTexture},
	{"RenderButton", L_RenderButton},
	{NULL, NULL}
};

int luaopen_ManySnakesRender(lua_State *L)
{
	luaL_newlib(L, ManySnakesRender);
	return 1;
}
