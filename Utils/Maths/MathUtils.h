#pragma once
#include "SFML/Graphics.hpp"

float DegreeToRadians(float angle);
float VectorMangnitude(sf::Vector2f vector);
void Normalize(sf::Vector2f& vector);
sf::Vector2f RotateAroundPoint(sf::Vector2f point, sf::Vector2f origin, float angle);
float Dot(sf::Vector2f v1, sf::Vector2f v2);