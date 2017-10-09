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

#include <random>
#include <vector>
#include <glm/glm.hpp>

class PerlinNoise
{
public:
    PerlinNoise(int width, int height);
    
    double getPointValue(glm::vec2 pos);

	std::vector<std::vector<double>> generateNoise(glm::vec2 lower_bound, glm::vec2 upper_bound, glm::uvec2 num_pixels);

private:

    float ease(float t);
	float interpolate(float first, float second, float amount);
	float getInfluence(glm::ivec2 corner, glm::vec2 pos);
    glm::vec2 getGradient(glm::ivec2 corner);

    const int m_width, m_height;
    
    std::vector<glm::vec2> m_gradientChoices;
    std::vector<std::vector<int>> m_gradients; // Stores an index into gradient choices

	std::random_device m_rdev;
	std::mt19937 m_rnd;
	std::uniform_int_distribution<int> m_dist;
};