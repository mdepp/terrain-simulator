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
    vector<uint32_t> pixels;
    for(int i=0; i<imageSize; ++i)
    {
        for (int j=0; j<imageSize; ++j)
        {
			auto x = j / (double)imageSize*nCells;
			auto y = i / (double)imageSize*nCells;
			auto val = pn.getPointValue({ x, y });
			int colourComponent = 0x0000FF * std::max(0.0, std::min(1.0, val));
			pixels.push_back(colourComponent + (colourComponent<<8) + (colourComponent<<16));
        }
    }

    SDL_Init(SDL_INIT_VIDEO);
    auto window = SDL_CreateWindow("Perlin noise",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        imageSize, imageSize, 0);
    
    auto noise_surface = SDL_CreateRGBSurfaceFrom(
        (void*)pixels.data(),
        imageSize, imageSize,
        sizeof(uint32_t)*8,
        imageSize * sizeof(uint32_t),
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