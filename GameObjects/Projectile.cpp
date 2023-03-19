#include "Projectile.h"
#include <iostream>

Projectile::Projectile(sf::Vector2f position, float rotation)
{

	if (!m_ProjectileTexture.loadFromFile("./Sprites/Projectile.png"))
	{
		std::cout << "Obstacle Texture load fail" << std::endl;
		return;
	}

	m_ProjectileTexture.setSmooth(true);
	m_ProjectileSprite.setTexture(m_ProjectileTexture);
	m_ProjectileSprite.setOrigin(m_ProjectileSprite.getLocalBounds().width / 2, m_ProjectileSprite.getLocalBounds().height / 2);
	m_ProjectileSprite.setPosition(position);
	m_ProjectileSprite.setRotation(rotation);
	m_ProjectileSprite.setScale({0.15, 0.15});

	m_ProjectileSpritePtr = &m_ProjectileSprite;
}

void Projectile::Update(float dt)
{
	sf::Vector2f pos = m_ProjectileSprite.getPosition();
	pos.x += m_Velocity.x * dt;
	pos.y += m_Velocity.y * dt;
	m_ProjectileSprite.setPosition(pos);
}

bool Projectile::IsAlive(int width, int height)
{
	if (m_ProjectileSprite.getPosition().x > width || m_ProjectileSprite.getPosition().x < 0)
		return false;
	if (m_ProjectileSprite.getPosition().y > height || m_ProjectileSprite.getPosition().y < 0)
		return false;
	return true;
}
