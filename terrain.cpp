#include "terrain.h"
#include "utils.h"
#include <algorithm>
#include <random>
#include <vector>

Terrain::Terrain(glm::ivec2 pixel_dimensions, glm::ivec2 cell_dimensions)
    : m_pixel_dimensions(pixel_dimensions), m_perlin_noise(cell_dimensions)
{
    // Initialize initial terrain values with Perlin noise
    m_initial_seed = m_perlin_noise.generateNoise(cell_dimensions);
    m_height_map = m_perlin_noise.getNoiseMap({ 0, 0 }, cell_dimensions, pixel_dimensions);
}

void Terrain::rain()
{
    const int num_drops = 1000;

    std::uniform_int_distribution<int> x_dist(0, 10);
    std::uniform_int_distribution<int> y_dist(0, 10);

    std::random_device dev;
    std::mt19937 rng(dev());

    for (int i = 0; i < num_drops; ++i)
    {
        simulateDroplet({ x_dist(rng), y_dist(rng) });
    }
}

auto Terrain::getHeight(glm::ivec2 pos)
{
    auto wrapped = wrapPosition(pos);
    return m_height_map[wrapped.x][wrapped.y];
}

glm::ivec2 Terrain::wrapPosition(glm::ivec2 pos)
{
    return { (pos.x%m_pixel_dimensions.x + m_pixel_dimensions.x) % m_pixel_dimensions.x,
        (pos.y%m_pixel_dimensions.y + m_pixel_dimensions.y) % m_pixel_dimensions.y };
}

double Terrain::depositSediment(glm::ivec2 pos, double amount)
{
    auto wrapped_pos = wrapPosition(pos);
    auto initial = m_height_map[wrapped_pos.x][wrapped_pos.y] + amount;
    auto final_clamped = clamp(initial+amount, 0.0, 1.0);
    m_height_map[wrapped_pos.x][wrapped_pos.y] = final_clamped;
    return final_clamped - initial; // Return actual amount of sediment deposited
}

glm::vec2 Terrain::getDownVector(glm::ivec2 point)
{
    static const std::vector<glm::ivec2> directions = {
        { -1, -1 },{ -1, 0 },{ -1, 1 },
        { 0, -1 },{ 0, 1 },
        { 1, -1 },{ 1, 0 },{ 1, 1 }
    };
    // Weight each of these directions by inverse height
    glm::vec2 down_vector(0);
    for (auto& dir : directions)
    {
        auto height = 1.0-getHeight(point + dir);
        down_vector += glm::vec2(dir.x*height, dir.y*height);
    }

    return down_vector;
}

void Terrain::simulateDroplet(glm::ivec2 pos)
{
    const float velocity_factor = 2.f;
    const float erosion_factor = 0.1f;
    const int max_iterations = 100; // To prevent infinite loops

    glm::vec2 vel(0, 0);
    float sediment = 0.f;

    int iterations = 0;
    while (iterations < max_iterations)
    {
        // Get the vector pointing downhill from current point
        auto dir = getDownVector(pos);
        // Update particle velocity from normal force of this incline
        vel += velocity_factor*dir;
        // Calculate new position of particle from downhill vector (not velocity); this models non-constant time
        auto new_pos = pos + glm::ivec2(vel); // glm::ivec2(glm::normalize(vel)*2.f);
        // If particle has stopped, deposit remaining sediment and quit
        if (vel.length() <= 0.0001f)
        {
            break;
        }
        // Erode sediment proportional to velocity in direction of downhill (if negative, then deposits instead)
        auto eroded = glm::dot(vel, dir)*erosion_factor / vel.length() / dir.length();
        
        auto deposited = depositSediment(pos, -eroded);
        sediment -= deposited;

        // Move droplet and repeat
        pos = new_pos;

        ++iterations;
    }
    //depositSediment(pos, sediment);
}