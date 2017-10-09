#include <SDL2/SDL.h>

#include "perlin.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    const int imageSize = 800;
    const int nCells = 10;
    PerlinNoise pn(nCells, nCells);

    // A quick hack to get noise displaying
    vector<char> pixels;
    for(int i=0; i<imageSize; ++i)
    {
        for (int j=0; j<imageSize; ++j)
        {
            auto x = i/(double)imageSize*nCells;
            auto y = j/(double)imageSize*nCells;
			auto val = pn.getPointValue({ x, y });
			val = std::max(0.0, std::min(1.0, val));
			pixels.push_back(0x0000FF * val);
        }
    }

    SDL_Init(SDL_INIT_VIDEO);
    auto window = SDL_CreateWindow("Perlin noise",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        imageSize, imageSize, 0);
    
    auto noise_surface = SDL_CreateRGBSurfaceFrom(
        (void*)pixels.data(),
        imageSize, imageSize,
        24,
        imageSize,
        0xFF0000,
        0x00FF00,
        0x0000FF,
        0);

    auto screen = SDL_GetWindowSurface(window);

	SDL_Event e;
	bool running = true;
	while (running)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) running = false;
			if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE) running = false;
		}

		SDL_BlitSurface(noise_surface, nullptr, screen, nullptr);
		SDL_UpdateWindowSurface(window);
	}

    SDL_FreeSurface(noise_surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}