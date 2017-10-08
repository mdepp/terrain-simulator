/*
 * This algorithm is loosely based off of a few sources (listed below). While it
 * may be less efficient, I feel it is slightly more readable.
 *  - http://flafla2.github.io/2014/08/09/perlinnoise.html
 *  - https://en.wikipedia.org/wiki/Perlin_noise
 */

/*
 THE PERLIN NOISE ALGORITHM

 Split mesh into squares (unit squares are convenient). 
 Look at the corners of each square, and generate a gradient vector unique
 to that corner. Dot this to the distance from the corner to the point, giving
 a value for each corner. Average these values to get final value at that point.
 */

#include <algorithm>
#include <assert.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <glm/glm.hpp>

class PerlinNoise
{
public:
    PerlinNoise(int width, int height)
        : m_width(width), m_height(height),
		m_gradientChoices({ glm::vec2(1, 1), glm::vec2(1, -1), glm::vec2(-1, 1), glm::vec2(-1, -1) })
    {
        m_gradients.resize(width);
        for(auto& rows : m_gradients)
        {
            rows.resize(height);
            for (auto&  corner : rows)
            {
                corner = std::rand() % m_gradientChoices.size(); // TODO: Use better random generator
            }
        }
    }
    
    double getPointValue(glm::vec2 pos)
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

private:

    float ease(float t)
    {
		//auto val = (t*t*t)*(6.f*(t*t*t) - 15.f*t + 10.f);
		auto val = t*t*(3 - 2 * t);
		assert(val < 1.f);
		return val;
    }
	float interpolate(float first, float second, float amount)
	{
		return first + ease(amount)*(second - first);
	}
	
	float getInfluence(glm::ivec2 corner, glm::vec2 pos)
	{
		auto distance = pos - glm::vec2(corner);
		auto gradient = getGradient(corner);
		return glm::dot(distance, gradient);
	}

    glm::vec2 getGradient(glm::ivec2 corner)
    {
		auto x = (corner.x%m_width + m_width) % m_width; // Wrap greater/lesser values to [0, width-1]
		auto y = (corner.y%m_height + m_height) % m_height;
		return m_gradientChoices[m_gradients[x][y]];
    }

    const int m_width, m_height;
    
    std::vector<glm::vec2> m_gradientChoices;
    std::vector<std::vector<int>> m_gradients; // Stores an index into gradient choices

};