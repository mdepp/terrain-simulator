#include <SDL2/SDL.h>
#include "perlin.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <vector>

using namespace std;

SDL_Surface* createSurfaceFromHeightmap(const vector<vector<double>>& height_map);

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

    const int imageSize = 800;
    const int nCells = 10;
    PerlinNoise pn(nCells, nCells);

    // A quick hack to get noise displaying
	auto pixels = pn.generateNoise({ 0, 0 }, {nCells, nCells }, { imageSize, imageSize });
	auto noise_surface = createSurfaceFromHeightmap(pixels);

    auto window = SDL_CreateWindow("Perlin noise",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        imageSize, imageSize, 0);

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

/*
 * Create an greyscale SDL surface with a copy of the data provided. Makes some
 * assumptions about byte order, bits per pixel, and other stuff, so will really
 * have to be revised at some point.
 * Uses size of height_map as width, and size of first vector in height_map as
 * height. Will return nullptr if an empty map is requested or something else fails.
 */
SDL_Surface* createSurfaceFromHeightmap(const vector<vector<double>>& height_map)
{
	if (height_map.empty()) return nullptr;
	if (height_map[0].empty()) return nullptr;
	auto width = height_map.size(), height = height_map[0].size();

	const int bitsPerByte = 8;
	auto surface = SDL_CreateRGBSurface(0, width, height, sizeof(uint32_t)*bitsPerByte, 0xFF0000, 0x00FF00, 0x0000FF, 0);

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			// Clamp height map to [0, 1] and calculate rgb colour components
			int colourComponent = 0x0000FF * std::max(0.0, std::min(1.0, height_map[x][y]));
			uint32_t pixelValue = colourComponent + (colourComponent << bitsPerByte) + (colourComponent << (bitsPerByte * 2));
			((uint32_t*) surface->pixels)[y*width + x] = pixelValue;
		}
	}
	return surface;
}