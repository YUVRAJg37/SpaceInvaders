#pragma once
#include <SFML/Graphics.hpp>

class CollisionHandler
{
public:

	CollisionHandler();

	bool BoundingBoxDetection(sf::Sprite* sprite1, sf::Sprite* sprite2);

private:

	void Projection(std::vector<sf::Vector2f>& vertices, sf::Vector2f axis, float& min, float& max);
	void GetVertices(sf::Sprite* rect, std::vector<sf::Vector2f>& vertices);
	void ScaleVertices(std::vector<sf::Vector2f>& vertices, float scale);
};

