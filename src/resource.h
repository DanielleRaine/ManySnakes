/**
 * @file
 * @author Danielle Raine
 * @date Created January 3rd, 2025
 * @date Last Modified January 3rd, 2025
 * @brief Brief goes here...
 */

#ifndef RESOURCE_H
#define RESOURCE_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

/**
 * @brief Brief description goes here.
 *
 * Detailed description goes here.
 */
typedef struct Resource
{
	void *resource;
	char key[0x20];
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

ResourceManager *InitializeResourceManager(unsigned int initial_size, double max_load_factor, double min_load_factor_mult, int (*hash_function)(const char*));
bool SetResource(ResourceManager *manager, const char* key, void *resource);
void *GetResource(ResourceManager *manager, const char *key);
void *RemoveResource(ResourceManager *manager, const char *key);
bool RehashResourceManager(ResourceManager *manager);
void DestroyResourceManager(ResourceManager *manager);


#endif
