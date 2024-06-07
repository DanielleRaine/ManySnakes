#include <stdio.h>
#include <SDL2/SDL.h>

int main(void)
{
	printf("C Version: %d\nGame Version: 0.0.1\n", __STDC_VERSION__);

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_EVENTS) != 0)
	{
		printf("%s", SDL_GetError());
		return 1;
	}

	return 0;
}
