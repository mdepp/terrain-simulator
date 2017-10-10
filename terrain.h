#ifndef TERRAIN_H
#define TERRAIN_H

#include "perlin.h"
#include "utils.h"
#include <vector>
#include <algorithm>

class Terrain
{
    Terrain(glm::ivec2 pixel_dimensions, glm::ivec2 cell_dimensions)
        : m_pixel_dimensions(pixel_dimensions), m_perlin_noise(cell_dimensions)
    {
        // Initialize initial terrain values with Perlin noise
        m_initial_seed = m_perlin_noise.generateNoise(cell_dimensions);
        m_height_map = m_perlin_noise.getNoiseMap({ 0, 0 }, cell_dimensions, pixel_dimensions);
    }

    auto& getHeightmap() { return m_height_map; }

    /*
     * Get the value of the terrain heightmap at a certain point. Will
     * wrap if point is outside of heightmap dimensions.
     */
    auto getHeight(glm::ivec2 pos)
    {
        auto wrapped = wrapPosition(pos);
        return m_height_map[wrapped.x][wrapped.y];
    }
private:

    /*
     * Wrap a position to within pixel dimensions.
     */
    glm::ivec2 wrapPosition(glm::ivec2 pos)
    {
        return { (pos.x%m_pixel_dimensions.x + m_pixel_dimensions.x) % m_pixel_dimensions.x,
            (pos.y%m_pixel_dimensions.y + m_pixel_dimensions.y) % m_pixel_dimensions.y };
    }

    /*
     */
    void addSediment(glm::ivec2 pos, double amount)
    {
        auto wrapped_pos = wrapPosition(pos);
        m_height_map[wrapped_pos.x][wrapped_pos.y] = clamp(m_height_map[wrapped_pos.x][wrapped_pos.y]+amount, 0.0, 1.0);
    }

    /*
     * Return the direction vector pointing downhill from a given point.
     */
    glm::ivec2 getDownVector(glm::ivec2 point)
    {
        const std::vector<glm::ivec2> directions = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1}, {0, 0}, {0, 1},
            {1, -1}, {1, 0}, {1, 1}
        };

        auto downhill_direction = std::min(directions.begin(), directions.end(), [&](auto first, auto second)
        {
            return getHeight(point + first) < getHeight(point + second);
        });

        return *downhill_direction;
    }

    void simulateDroplet(glm::ivec2 pos)
    {
        const float velocity_factor = 10.f;

        glm::vec2 vel(0, 0);
        float sediment = 0.f;

        while (true)
        {
            // Get the vector pointing downhill from current point
            auto dir = getDownVector(pos);
            // Update particle velocity from normal force of this incline
            vel += velocity_factor*glm::vec2(dir);
            // Calculate new position of particle from this velocity
            auto new_pos = pos + glm::ivec2(vel);
            // If particle has stopped, deposit remaining sediment and quit
            if (new_pos == pos)
            {
                addSediment(pos, sediment);
                return;
            }
            // 
        }
    }

    const glm::ivec2 m_pixel_dimensions; // Number of pixels in terrain heightmap
    const glm::ivec2 m_cell_dimensions; // Number of cells in terrain heightmap (for Perlin noise)
    std::vector<std::vector<double>> m_height_map;

    PerlinNoise m_perlin_noise; // Perlin noise generator
    PerlinNoise::SeedType m_initial_seed; // Seed to reconstruct original terrain noise
};

#endif