/**
 * @file
 * @author Danielle Raine
 * @date 2024-10-24
 * @brief 
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
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "resource.h"


bool RenderTexture(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Rect *srcrect, const SDL_Rect *dstrect);

int luaopen_ManySnakesTextures(lua_State *L);


#endif
