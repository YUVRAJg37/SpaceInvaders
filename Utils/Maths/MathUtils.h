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

sf::Vector2f RotateAroundPoint(sf::Vector2f point, sf::Vector2f origin, float angle)
{
	sf::Vector2f NewPointLocation;
	NewPointLocation.x = (point.x - origin.x) * (float)cos(angle) - (point.y - origin.y) * (float)sin(angle) + origin.x;
	NewPointLocation.y = (point.x - origin.x) * (float)sin(angle) + (point.y - origin.y) * (float)cos(angle) + origin.y;
	return NewPointLocation;
}

