#include <SFML/System/Vector2.hpp>

#include <cmath>

namespace Utils 
{
    bool Normalize(sf::Vector2f& vector)
    {
        float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);

		if (magnitude != 0)
		{
			vector.x /= magnitude;
			vector.y /= magnitude;
		
			return true;
		}

		return false;
    }
}