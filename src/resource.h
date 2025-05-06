/**
 * @file
 * @author Danielle Raine
 * @date Created January 3rd, 2025
 * @date Last Modified January 12th, 2025
 * @brief Brief goes here...
 */

#ifndef RESOURCE_H
#define RESOURCE_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "texture.h"


/**
 * @brief Brief description goes here.
 *
 * Detailed description goes here.
 */
typedef struct Resource
{
	void *resource;
	char *key;
	struct Resource *next;
} Resource;

/**
 * @brief Brief description goes here.
 *
 * Detailed description goes here.
 */
typedef struct ResourceManager
{
	 Resource **resources;
	 int num_resources;
	 unsigned int size;
	 double load_factor;
	 double max_load_factor;
	 double min_load_factor_mult;
	 int (*hash_function)(const char*);
} ResourceManager;


int CustomHash(const char* key);

ResourceManager *CreateResourceManager(unsigned int initial_size, double max_load_factor, double min_load_factor_mult, int (*hash_function)(const char*));
bool SetResource(ResourceManager *manager, const char* key, void *resource);
void *GetResource(ResourceManager *manager, const char *key);
//bool RenderResource(ResourceManager *manager, const char *key);
//bool RenderResources(ResourceManager *manager, const char **keys);
void *RemoveResource(ResourceManager *manager, const char *key);
void DestroyResource(ResourceManager *manager, const char *key);
void DestroyResourceManager(ResourceManager *manager);

Texture *GetTextureResource(ResourceManager *manager, SDL_Renderer *renderer, const SDL_Rect *bounds, const char *path)


#endif
