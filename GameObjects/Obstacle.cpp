#include "Obstacle.h"

#include <iostream>

Obstacle::Obstacle(ObstacleTransform obstacleTransform, sf::Texture* texture)
{

	if (!texture)
		return;

	m_ObstacleSprite.setTexture(*texture);
	m_ObstacleSprite.setOrigin(m_ObstacleSprite.getLocalBounds().width / 2, m_ObstacleSprite.getLocalBounds().height / 2);
	m_ObstacleSprite.setPosition(obstacleTransform.position);
	m_ObstacleSprite.setRotation(obstacleTransform.rotation);
	m_ObstacleSprite.setScale(obstacleTransform.scale);

	m_ObstacleSpritePtr = &m_ObstacleSprite;
}

void Obstacle::Update(float dt)
{
	sf::Vector2f pos = m_ObstacleSprite.getPosition();
	pos.x += m_Velocity.x * dt;
	pos.y += m_Velocity.y * dt;
	m_ObstacleSprite.setPosition(pos);
}
