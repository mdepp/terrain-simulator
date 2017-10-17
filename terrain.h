#ifndef TERRAIN_H
#define TERRAIN_H

#include "perlin.h"
#include <vector>

class Terrain
{
public:
    Terrain(glm::ivec2 pixel_dimensions, glm::ivec2 cell_dimensions);

    /*
     * Test simulation of water droplets
     */
    void rain();

    auto& getHeightmap() { return m_height_map; }
    /*
     * Get the value of the terrain heightmap at a certain point. Will
     * wrap if point is outside of heightmap dimensions.
     */
    auto getHeight(glm::ivec2 pos);
private:
    /*
     * Wrap a position to within pixel dimensions.
     */
    glm::ivec2 wrapPosition(glm::ivec2 pos);
    /*
     */
    double depositSediment(glm::ivec2 pos, double amount);
    /*
     * Return the direction vector pointing downhill from a given point.
     */
    glm::vec2 getDownVector(glm::ivec2 point);

    /*
     * Simulate erosion of a single water droplet (needs water channels, etc)
     */
    void simulateDroplet(glm::ivec2 pos);

    const glm::ivec2 m_pixel_dimensions; // Number of pixels in terrain heightmap
    const glm::ivec2 m_cell_dimensions; // Number of cells in terrain heightmap (for Perlin noise)
    std::vector<std::vector<double>> m_height_map;

    PerlinNoise m_perlin_noise; // Perlin noise generator
    PerlinNoise::SeedType m_initial_seed; // Seed to reconstruct original terrain noise
};

#endif