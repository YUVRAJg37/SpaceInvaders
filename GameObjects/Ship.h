#pragma once
#include <SFML/Graphics.hpp>

class Ship
{
public:

	Ship(sf::Texture* texture);

private:

	sf::Sprite m_ShipSprite;
	sf::Sprite* m_ShipSpritePtr;
	float m_ShipMovingSpeed;
	float m_ShipRotationSpeed;
	sf::Vector2f m_ShipDirection;
	sf::Vector2f m_Velocity{ 0, 0 };
	float m_Drag;
	sf::Vector2f m_PreviousPosition;
	sf::Vector2f m_CurrentPosition;
	float m_MaxVelocity;

public:

	void UpdateShip(float dt);

	sf::Sprite* GetShip() const { return m_ShipSpritePtr; }
	void SetShipMovingSpeed(float speed) { m_ShipMovingSpeed = speed; }
	void SetShipRotationSpeed(float speed) { m_ShipRotationSpeed = speed; }
	void SetMaxVelocity(float speed) { m_MaxVelocity = speed; }
	sf::Vector2f GetShipDirection() const { return m_ShipDirection; }
	sf::Vector2f GetShipVelocity() const { return m_Velocity; }
	float GetShipRotationSpeed() const { return m_ShipRotationSpeed; }
	float GetShipMovingSpeed() const { return m_ShipMovingSpeed; }
	float GetShipMaxVelocity() const { return m_MaxVelocity; }
	float GetDrag() const { return m_Drag; }
	void SetDrag(float value) { m_Drag = value; }
};

