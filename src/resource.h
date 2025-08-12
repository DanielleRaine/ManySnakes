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
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


//TODO Add comments
typedef enum
{
	USERDEF_RESOURCE = 0,
	TEXTURE_RESOURCE = 1
} ResourceType;

/**
 * @brief Brief description goes here.
 *
 * Detailed description goes here.
 */
typedef struct Resource
{
	void *resource;
	char key[0x100];
	struct Resource *next;
	ResourceType type;
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
	 double min_load_factor;
	 int (*hash_function)(const char*);
} ResourceManager;


int CustomHash(const char* key);

ResourceManager *CreateResourceManager(unsigned int initial_size, double max_load_factor, double min_load_factor, int (*hash_function)(const char*));
bool SetResource(ResourceManager *manager, const char* key, void *resource, ResourceType type);

void *GetResource(ResourceManager *manager, const char *key);
SDL_Texture *GetTextureResource(ResourceManager *manager, SDL_Renderer *renderer, const char *path);

// bool RenderResource(ResourceManager *manager, const char *key);
// bool RenderResources(ResourceManager *manager, const char **keys);

void *RemoveResource(ResourceManager *manager, const char *key);

// void DestroyResource(ResourceManager *manager, const char *key);
void DestroyResourceManager(ResourceManager *manager);



#endif
