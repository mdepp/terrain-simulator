#include "perlin.h"

#include <algorithm>
#include <random>
#include <vector>
#include <glm/glm.hpp>

PerlinNoise::PerlinNoise(glm::uvec2 num_cells)
  : m_gradientChoices({ glm::vec2(1, 1), glm::vec2(1, -1), glm::vec2(-1, 1), glm::vec2(-1, -1) }),
  m_rnd(m_rdev()), m_dist(0, m_gradientChoices.size()-1)
{
    generateNoise(num_cells);
}

double PerlinNoise::getNoiseValue(glm::vec2 pos)
{
    auto lower_bound = glm::ivec2(std::floor(pos.x), std::floor(pos.y));
    auto upper_bound = glm::ivec2(lower_bound.x + 1, lower_bound.y + 1);

    auto weight = pos - glm::vec2(lower_bound);

    float influence_low_left = getInfluence({ lower_bound.x, lower_bound.y }, pos);
    float influence_low_right = getInfluence({ upper_bound.x, lower_bound.y }, pos);
    float influence_high_left = getInfluence({ lower_bound.x, upper_bound.y }, pos);
    float influence_high_right = getInfluence({ upper_bound.x, upper_bound.y }, pos);

    float influence_low = interpolate(influence_low_left, influence_low_right, weight.x);
    float influence_high = interpolate(influence_high_left, influence_high_right, weight.x);
    float influence = interpolate(influence_low, influence_high, weight.y);
    return (1.0+influence)/2.0; // Scale to [0, 1] for convenience
}

std::vector<std::vector<double>> PerlinNoise::getNoiseMap(glm::vec2 lower_bound, glm::vec2 upper_bound, glm::uvec2 num_pixels)
{
    std::vector<std::vector<double>> noise(num_pixels.x, std::vector<double>(num_pixels.y));
    for (int x = 0; x < num_pixels.x; ++x)
    {
        for (int y = 0; y < num_pixels.y; ++y)
        {
            auto scaled_x = (float(x) / num_pixels.x)*(upper_bound.x - lower_bound.x) + lower_bound.x;
            auto scaled_y = (float(y) / num_pixels.y)*(upper_bound.y - lower_bound.y) + lower_bound.y;
            noise[x][y] = getNoiseValue({ scaled_x, scaled_y });
        }
    }
    return noise;
}

PerlinNoise::SeedType PerlinNoise::generateNoise(glm::uvec2 num_cells)
{
    return generateNoise(num_cells, m_rdev());
}
PerlinNoise::SeedType PerlinNoise::generateNoise(glm::uvec2 num_cells, PerlinNoise::SeedType seed)
{
    m_width = num_cells.x;
    m_height = num_cells.y;

    m_rnd.seed(seed);

    m_gradients.resize(m_width);
    for (auto& rows : m_gradients)
    {
        rows.resize(m_height);
        for (auto& corner : rows)
        {
            corner = m_dist(m_rnd);
        }
    }
    return seed;
}

float PerlinNoise::ease(float t)
{
    //auto val = (t*t*t)*(6.f*(t*t*t) - 15.f*t + 10.f);
    auto val = t*t*(3 - 2 * t);
    assert(val < 1.f);
    return val;
}
float PerlinNoise::interpolate(float first, float second, float amount)
{
    return first + ease(amount)*(second - first);
}

float PerlinNoise::getInfluence(glm::ivec2 corner, glm::vec2 pos)
{
    auto distance = pos - glm::vec2(corner);
    auto gradient = getGradient(corner);
    return glm::dot(distance, gradient);
}

glm::vec2 PerlinNoise::getGradient(glm::ivec2 corner)
{
    auto x = (corner.x%m_width + m_width) % m_width; // Wrap greater/lesser values to [0, width-1]
    auto y = (corner.y%m_height + m_height) % m_height;
    return m_gradientChoices[m_gradients[x][y]];
}