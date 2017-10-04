// Based off of http://flafla2.github.io/2014/08/09/perlinnoise.html

/*
 THE PERLIN NOISE ALGORITHM

 Split mesh into squares (unit squares are convenient). 
 Look at the corners of each square, and generate a gradient vector unique
 to that corner. Dot this to the distance from the corner to the point, giving
 a value for each corner. Average these values to get final value at that point.
 */

#include <cmath>
#include <vector>
#include <glm/glm.hpp>

class PerlinNoise
{
public:
    PerlinNoise(int width, int height)
        : m_width(width), m_height(height)
    {
        m_gradientChoices{{1,1},{-1,1},{1,-1},{-1,-1}};
        
        gradients.resize(width+1);
        for(auto& rows : gradients)
        {
            rows.resize(height+1);
            for (auto&  corner : rows)
            {
                corner = rand() % m_gradientChoices.length(); // TODO: Use better random generator
            }
        }
    }
    
private:
    double getPointValue(glm::vec2 pos)
    {
        // Calculate enclosing integer bounds
        auto lowerBound = glm::vec2(std::floor(pos.x)), std::floor(pos.y));
        auto upperBound = glm::vec2(std::ceil(pos.x)), std::ceil(pos.y));
        // Calculate position of all corners
        std::vector<glm::vec2> corners = {
            lowerBound,
            {lowerBound.x, upperBound.y},
            upperBound,
            {upperBound.x, lowerBound.y}
        };
        for (auto corner : corners)
        {
            auto distance = pos - corner;
            auto gradient = getGradient(corner);
            auto influence = glm::dot(distance, gradient);
            // Fade b/w influence for each corner...
        }

    }

    const int m_width, m_height;
    
    std::vector<glm::vec2> m_gradientChoices;
    std::vector<std::vector<int>> m_gradients; // Stores an index into gradient choices

}