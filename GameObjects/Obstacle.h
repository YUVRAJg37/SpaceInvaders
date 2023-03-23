#pragma once
#include <SFML/Graphics.hpp>

struct ObstacleTransform
{
	sf::Vector2f position;
	float rotation;
	sf::Vector2f scale;
};

class Obstacle
{
public:

	Obstacle(ObstacleTransform obstacleTransform);
	void Update(float dt);

private:

	sf::Texture m_ObstacleTexture;
	sf::Sprite m_ObstacleSprite;
	sf::Sprite* m_ObstacleSpritePtr;
	sf::Vector2f m_Velocity = {0, 0};
	bool m_CanCollide = true;
	bool m_Visible = true;

public:

	sf::Sprite* GetObstacle() const { return m_ObstacleSpritePtr; }
	void SetVelocity(sf::Vector2f vel) { m_Velocity = vel; }
	void SetCollision(bool x) { m_CanCollide = x; }
	bool GetCollision() const { return m_CanCollide; }
	void SetVisibility(bool x) { m_Visible = x; }
	bool GetVisibility() const { return m_Visible; }
};

