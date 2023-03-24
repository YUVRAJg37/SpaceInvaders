#include "Ship.h"
#include "../Utils/Maths/MathUtils.h"

Ship::Ship(sf::Texture* texture)
{
	if (!texture)
		return;

	texture->setSmooth(true);
	m_ShipSprite.setTexture(*texture);
	m_ShipSprite.setOrigin(m_ShipSprite.getLocalBounds().width / 2, m_ShipSprite.getLocalBounds().height / 2);
	m_ShipSprite.scale(0.5, 0.5);
	m_ShipSpritePtr = &m_ShipSprite;
}

void Ship::UpdateShip(float dt)
{
	sf::Vector2f shipPosition = m_ShipSprite.getPosition();
	float shipRotation = m_ShipSprite.getRotation();

	m_PreviousPosition = m_CurrentPosition;
	m_CurrentPosition = shipPosition;
	m_Velocity.x = (m_CurrentPosition.x - m_PreviousPosition.x) / dt;
	m_Velocity.y = (m_CurrentPosition.y - m_PreviousPosition.y) /dt;

	m_ShipDirection.x = (float)cos(DegreeToRadians(shipRotation - 90.0f));
	m_ShipDirection.y = (float)sin(DegreeToRadians(shipRotation - 90.0f));
	Normalize(m_ShipDirection);
}
