#pragma once
#include <SFML/Graphics.hpp>

float DegreeToRadians(float angle)
{
	float radian = angle * (3.14 / 180);

	return radian;
}

float VectorMangnitude(sf::Vector2f vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

void Normalize(sf::Vector2f &vector)
{
	float magnitude = VectorMangnitude(vector);
	vector.x /= magnitude;
	vector.y /= magnitude;
}


