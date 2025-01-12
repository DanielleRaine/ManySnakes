#include "resource.h"


static Resource *GetResourceNode(ResourceManager *manager, const char *key);
static bool RehashResourceManager(ResourceManager *manager);


int CustomHash(const char *key)
{
	int length = strlen(key);
	int sum = 0;

	for (int i = 0; i < length; ++i)
	{
		sum += key[i] * 31 ^ i;
	}

	return sum;
}

ResourceManager *CreateResourceManager(unsigned int initial_size, double max_load_factor, double min_load_factor_mult, int (*hash_function)(const char*))
{
	// create the resource manager
	ResourceManager *manager = calloc(initial_size, sizeof(ResourceManager));
	if (!manager)
	{
		return NULL;
	}

	manager->num_resources = 0;
	manager->size = initial_size;
	manager->load_factor = 0;
	manager->max_load_factor = max_load_factor;
	manager->min_load_factor_mult = min_load_factor_mult;
	manager->hash_function = hash_function;

	return manager;
}

bool SetResource(ResourceManager *manager, const char* key, void *resource, void (*destroy_function)(void*))
{
	// create a Resource node
	Resource *node = malloc(sizeof(Resource));
	if (!node)
	{
		return false;
	}

	// increase Resource count and rehash if needed
	++manager->num_resources;
	if (!RehashResourceManager(manager)) // manager needed to be rehashed but failed
	{
		--manager->num_resources;
		free(node);
		return false;
	}
	
	// get the hash of the key and compute its index
	int index = manager->hash_function(key) % manager->size;

	// set the members of the struct
	node->resource = resource;
	strcpy(node->key, key);

	// Put the resource at its hash index, sorting it asciibetically with other nodes
	Resource *cur = manager->resources[index];
	if (cur && strcmp(key, cur->key) >= 0) // there is a node at this index, and its key is smaller
	{
		while (cur->next && strcmp(key, cur->next->key) > 0) // loop until there is no next or key bigger
		{
			cur = cur->next;
		}

		if (!cur->next || strcmp(key, cur->next->key) < 0) // no next node or key bigger, insert node
		{
			node->next = cur->next;
			cur->next = node;
		}
		else // key is the same, overwrite node
		{
			// FIXME overwrite the item at this index
		}
	}
	else // there is not a node or if there is one its key is bigger
	{
		node->next = manager->resources[index];
		manager->resources[index] = node;
	}

	return true;
}

 Resource *GetResourceNode(ResourceManager *manager, const char *key)
{
	// get the hash of the key and compute its index
	int index = manager->hash_function(key);

	Resource *node = manager->resources[index];
	while (node)
	{
		if (strcmp(key, node->key) == 0)
		{
			return node;
		}
		
		node = node->next;
	}

	return NULL;
}


void *GetResource(ResourceManager *manager, const char *key)
{
	Resource *node = GetResourceNode(manager, key);

	return node ? node : NULL;
}

void *RemoveResource(ResourceManager *manager, const char *key)
{
	// get the hash of the key and compute its index
	int index = manager->hash_function(key);

	Resource *node = manager->resources[index];
	void *resource;

	if (strcmp(key, node->key) == 0)
	{
		resource = node->resource;
		manager->resources[index] = node->next;
		free(node);
		return resource;
	}

	while (node->next)
	{
		if (strcmp(key, node->next->key) == 0)
		{
			resource = node->next->resource;
			Resource *delete_node = node->next;
			node->next = node->next->next;
			free(delete_node);
			return resource;
		}
		
		node = node->next;
	}

	return NULL;
}

//TODO Document the fact that if there is no destruction function, a memory leak will happen.
void DestroyResource(ResourceManager *manager, const char *key)
{
	Resource *node = GetResourceNode(manager, key);
	if (node && node->destroy_function)
	{
		node->destroy_function(node->resource);
		node->resource = NULL;
		RemoveResource(manager, key);
	}
	else
	{
		RemoveResource(manager, key);
	}
}

static bool RehashResourceManager(ResourceManager *manager)
{
	unsigned int new_size;
	double new_load_factor = manager->num_resources / manager->size;
	if (new_load_factor >= manager->max_load_factor)
	{
		new_size = manager->size << 1;
	}
	else if (new_load_factor <= manager->max_load_factor * manager->min_load_factor_mult)
	{
		new_size = manager->size >> 1;
	}
	else
	{
		manager->load_factor = new_load_factor;
		return true;
	}

	Resource **new_resources = calloc(new_size, sizeof(Resource));
	if (!new_resources)
	{
		return false;
	}

	for (unsigned int i = 0; i < manager->size; ++i)
	{
		Resource *node = manager->resources[i];
		while (node)
		{
			int new_index = manager->hash_function(node->key) % new_size;
			Resource *next = node->next;
			node->next = new_resources[new_index];
			new_resources[new_index] = node;
			node = next;
		}
	}

	free(manager->resources);
	manager->resources = new_resources;
	manager->size = new_size;
	manager->load_factor = new_load_factor;

	return true;
}

void DestroyResourceManager(ResourceManager *manager)
{
	free(manager->resources);
	
	for (unsigned int i = 0; i < manager->size; ++i)
	{
		Resource *node = manager->resources[i];
		while (node)
		{
			Resource *delete_node = node;
			node = node->next;
			free(delete_node);
		}
	}

	free(manager);
}
