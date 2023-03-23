#pragma once
#include <SFML/Graphics.hpp>

class Projectile
{
public:

	Projectile(sf::Vector2f position, float rotation);
	void Update(float dt);

private:

	sf::Texture m_ProjectileTexture;
	sf::Sprite m_ProjectileSprite;
	sf::Sprite* m_ProjectileSpritePtr;
	sf::Vector2f m_Velocity;
	bool m_CanCollide = true;
	bool m_Visible = true;

public:

	sf::Sprite* GetProjectileSprite() const { return m_ProjectileSpritePtr; }
	bool IsAlive(int width, int height);
	void SetVelocity(sf::Vector2f vel) { m_Velocity = vel; }
	void SetCollision(bool x) { m_CanCollide = x; }
	bool GetCollision() const { return m_CanCollide; }
	void SetVisibility(bool x) { m_Visible = x; }
	bool GetVisibility() const { return m_Visible; }
};
