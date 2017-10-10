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
    PerlinNoise(glm::uvec2 num_cells);
    using SeedType = std::random_device::result_type;
    /*
     * Calculate the value (in range [0, 1]) for a single point. Will repeat if
     * 'pos' is not within [0, width]x[0, height]. T
     */
    double getNoiseValue(glm::vec2 pos);
    /*
     * Creates and fills a 2-dimensional std::vector with noise values between
     * the specified bounds.
     */
    std::vector<std::vector<double>> getNoiseMap(glm::vec2 lower_bound, glm::vec2 upper_bound, glm::uvec2 num_pixels);

    /*
     * Randomly generate new gradient vectors at corners of a specified number of horizontal
     * and vertical cells. This will result in new noise values being calculated with
     * getNoiseValue() and getNoiseMap(). Can optionally provide a seed to control noise
     * generation. If no seed is specified one is generated non-deterministically. Returns 
     * this seed, which can be used to replicate specific noise patterns later.
     */
    SeedType generateNoise(glm::uvec2 num_cells);
    SeedType generateNoise(glm::uvec2 num_cells, SeedType seed);


private:

    float ease(float t);
    float interpolate(float first, float second, float amount);
    float getInfluence(glm::ivec2 corner, glm::vec2 pos);
    glm::vec2 getGradient(glm::ivec2 corner);

    unsigned int m_width, m_height;
    
    std::vector<glm::vec2> m_gradientChoices;
    std::vector<std::vector<int>> m_gradients; // Stores an index into gradient choices

    std::random_device m_rdev;
    std::mt19937 m_rnd;
    std::uniform_int_distribution<int> m_dist;
};