#include <iostream>
#include <cstdlib>

#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>

#include "Utils/Maths/MathUtils.h"
#include "GameObjects/Obstacle.h"
#include "GameObjects/Projectile.h"
#include "GameObjects/Ship.h"
#include "Utils/Miscellaneous/CollisionHandler.h"

constexpr int Width = 800;
constexpr int Height = 800;

void SpriteWrap(sf::Sprite* sprite);
void DestroyObstacle(std::vector<Obstacle*>& Obstacles, sf::Sprite* obstacle,sf::Vector2f normal, sf::Texture* texture);

template <typename t>
float Rand(t min, t max);

int main()
{
	sf::RenderWindow window(sf::VideoMode(Width, Height), "Space Invader");
	ImGui::SFML::Init(window);
	window.setFramerateLimit(144);

	int Score = 0;
	float BulletSpeed = 250.0f;
	bool bSpacePressed = false;
	bool bGameOver = false;
	float ShipMovingSpeed = 100.0f;
	float ShipRotationSpeed = 100.0f;
	float ShipMaxSpeed = 20.0f;
	float ShipDrag = 0.99;

	sf::Texture ShipTexture;
	sf::Texture SmallObstacleTexture;
	sf::Texture ObstacleTexture;

	std::vector<Obstacle*> Obstacles;
	std::vector<Projectile*> Projectiles;

	sf::Vector2f ObstacleMaxVelocity{ 20, 20 };
	sf::Vector2f ShipMomentum;

	sf::Font TextFont;
	sf::Text ScoreText;
	sf::Text GameOverText;

	sf::Clock deltaClock;


	if (!ObstacleTexture.loadFromFile("./Sprites/Obstacle.png"))
		return 0;
	if (!SmallObstacleTexture.loadFromFile("./Sprites/SmallObstacle.png"))
		return 0;
	if (!ShipTexture.loadFromFile("./Sprites/Ship.png"))
		return 0;
	if (!TextFont.loadFromFile("../../Fonts/OpenSans-Regular.ttf"))
		return 0;


	Ship ship(&ShipTexture);
	ship.GetShip()->setPosition(Width / 2, Height / 2);
	ship.SetShipMovingSpeed(100.0f);
	ship.SetShipRotationSpeed(100.0f);

	SmallObstacleTexture.setSmooth(true);
	ObstacleTexture.setSmooth(true);

	ScoreText.setFont(TextFont);
	ScoreText.setCharacterSize(20);
	GameOverText.setFont(TextFont);
	GameOverText.setCharacterSize(74);
	GameOverText.setString("Game Over");
	GameOverText.setOrigin(GameOverText.getLocalBounds().width / 2, GameOverText.getLocalBounds().height / 2);
	GameOverText.setPosition(Width / 2, Height / 2);

	for (int i = 0; i < 10; i++)
	{
		ObstacleTransform transform;
		transform.position = { (float)Rand<int>(0, Width), (float)Rand<int>(0, Height) };
		const float scale = Rand<float>(0.5, 0.2);
		transform.scale = { scale, scale };
		transform.rotation = Rand<int>(0, 360);
		Obstacle* obs = new Obstacle(transform, &ObstacleTexture);
		obs->SetVelocity({ Rand<int>(-ObstacleMaxVelocity.x, ObstacleMaxVelocity.x), Rand<int>(-ObstacleMaxVelocity.y, ObstacleMaxVelocity.y) });
		Obstacles.push_back(obs);
	}

	while (window.isOpen())
	{
		sf::Event event;
		sf::Time deltaTime = deltaClock.restart();

		float shipRotation = ship.GetShip()->getRotation();
		sf::Vector2f shipPosition = ship.GetShip()->getPosition();

		ship.UpdateShip(deltaTime.asSeconds());

		while(window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
				window.close();

			std::string scoreString = "Score : " + std::to_string(Score);
			ScoreText.setString(scoreString);

			if(event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Space && !bSpacePressed)
				{
					bSpacePressed = true;

					sf::Vector2f shipTip = { ship.GetShip()->getPosition().x, ship.GetShip()->getPosition().y - ship.GetShip()->getGlobalBounds().height / 2 + 5.0f };
					Projectile* pro = new Projectile(RotateAroundPoint(shipTip, ship.GetShip()->getPosition(), DegreeToRadians(ship.GetShip()->getRotation())), ship.GetShip()->getRotation());

					pro->SetVelocity({ ship.GetShipDirection().x * BulletSpeed, ship.GetShipDirection().y * BulletSpeed });
					Projectiles.push_back(pro);
				}
			}

			if(event.type == sf::Event::KeyReleased && !bGameOver)
			{
				if(event.key.code == sf::Keyboard::Space)
				{
					bSpacePressed = false;
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			shipRotation -= ship.GetShipRotationSpeed() * deltaTime.asSeconds();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			shipRotation += ship.GetShipRotationSpeed() * deltaTime.asSeconds();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			ShipMomentum.x += ship.GetShipDirection().x * ship.GetShipMovingSpeed() * deltaTime.asSeconds();
			ShipMomentum.y += ship.GetShipDirection().y * ship.GetShipMovingSpeed() * deltaTime.asSeconds();
			
			if(VectorMangnitude(ship.GetShipVelocity()) > ship.GetShipMaxVelocity())
			{
				ShipMomentum.x *= ship.GetDrag();
				ShipMomentum.y *= ship.GetDrag();
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			if (VectorMangnitude(ship.GetShipVelocity()) < ship.GetShipMaxVelocity())
			{
				ShipMomentum.x -= ship.GetShipDirection().x * ship.GetShipMovingSpeed() * deltaTime.asSeconds();
				ShipMomentum.y -= ship.GetShipDirection().y * ship.GetShipMovingSpeed() * deltaTime.asSeconds();
			}

			if (VectorMangnitude(ship.GetShipVelocity()) > ship.GetShipMaxVelocity())
			{
				ShipMomentum.x *= ship.GetDrag();
				ShipMomentum.y *= ship.GetDrag();
			}
		}
		else
		{
			ShipMomentum.x *= ship.GetDrag();
			ShipMomentum.y *= ship.GetDrag();
		}

		if(Projectiles.size()!=0)
		{
			for(Projectile* pro : Projectiles)
			{
				pro->Update(deltaTime.asSeconds());
			}

			int i = 0;
			while(i<Projectiles.size() && !Projectiles[i]->IsAlive(Width, Height))
			{
				Projectiles.erase(Projectiles.begin());
				i++;
			}
		}

		shipPosition.x += ShipMomentum.x*deltaTime.asSeconds();
		shipPosition.y += ShipMomentum.y*deltaTime.asSeconds();

		ship.GetShip()->setPosition(shipPosition);
		ship.GetShip()->setRotation(shipRotation);

		for(Obstacle* obs : Obstacles)
		{
			obs->Update(deltaTime.asSeconds());
			SpriteWrap(obs->GetObstacle());
		}


		if(!Obstacles.empty() && !Projectiles.empty())
		{
			CollisionHandler collisionHandler;

			for(Projectile* pro : Projectiles)
			{
				if (pro->GetCollision())
				{
					for (Obstacle* obs : Obstacles)
					{
						if(obs->GetCollision())
						{
							if (collisionHandler.BoundingBoxDetection(obs->GetObstacle(), pro->GetProjectile()))
							{
								Score++;

								pro->SetCollision(false);
								pro->SetVisibility(false);
								obs->SetVisibility(false);
								obs->SetCollision(false);

								sf::Vector2f edge = { pro->GetProjectile()->getPosition().x - obs->GetObstacle()->getPosition().x,
									pro->GetProjectile()->getPosition().y - obs->GetObstacle()->getPosition().y };
								sf::Vector2f axis1 = { -edge.y, edge.x };
								sf::Vector2f axis2 = { edge.y, -edge.x };

								DestroyObstacle(Obstacles, obs->GetObstacle() ,axis1, &SmallObstacleTexture);
								DestroyObstacle(Obstacles, obs->GetObstacle(),axis2, &SmallObstacleTexture);
								break;
							}
						}
					}
				}
			}
		}


		if(!Obstacles.empty())
		{
			CollisionHandler collisionHandler;
			for(Obstacle* obs : Obstacles)
			{
				if(obs->GetCollision())
				{
					if(collisionHandler.BoundingBoxDetection(obs->GetObstacle(), ship.GetShip()))
						bGameOver = true;
				}
			}
		}

		SpriteWrap(ship.GetShip());

		ImGui::SFML::Update(window, deltaTime);

		ImGui::Begin("Window Title");
		ImGui::InputFloat("Ship Speed", &ShipMovingSpeed);
		ImGui::InputFloat("Ship Rotation Speed", &ShipRotationSpeed);
		ImGui::InputFloat("Drag", &ShipDrag);
		ImGui::InputFloat("Max Velocity", &ShipMaxSpeed);
		ImGui::InputFloat("Bullet Speed", &BulletSpeed);
		ImGui::End();

		ship.SetShipMovingSpeed(ShipMovingSpeed);
		ship.SetShipRotationSpeed(ShipRotationSpeed);
		ship.SetDrag(ShipDrag);
		ship.SetMaxVelocity(ShipMaxSpeed);

		window.clear(sf::Color(18,33,43));

		if (!bGameOver)
			window.draw(*ship.GetShip());
		else
			window.draw(GameOverText);

		for (Obstacle* obs : Obstacles)
		{
			if (obs->GetVisibility())
			{
				window.draw(*obs->GetObstacle());
			}
		}

		for(Projectile* pro : Projectiles)
		{
			if (pro->GetVisibility())
			{
				window.draw(*pro->GetProjectile());
			}
		}

		window.draw(ScoreText);

		ImGui::SFML::Render(window);
		window.display();
	}

	for(Obstacle* obs : Obstacles)
	{
		delete obs;
	}

	ImGui::SFML::Shutdown();
	return 0;
}

void SpriteWrap(sf::Sprite* sprite)
{
	if (sprite->getPosition().y > Height)
		sprite->setPosition({ sprite->getPosition().x, 0 });
	if (sprite->getPosition().y < 0)
		sprite->setPosition({ sprite->getPosition().x, Height });
	if (sprite->getPosition().x > Width)
		sprite->setPosition({ 0, sprite->getPosition().y});
	if (sprite->getPosition().x < 0)
		sprite->setPosition({ Width, sprite->getPosition().y });

}

template <typename t>
float Rand(t min, t max)
{
	float x = min + static_cast <t> (rand()) / (static_cast <t> (RAND_MAX / (max - min)));
	return x;
}

void DestroyObstacle(std::vector<Obstacle*>& Obstacles, sf::Sprite* obstacle, sf::Vector2f normal, sf::Texture* texture)
{
	if (obstacle->getScale().x <= 0.15)
		return;

	ObstacleTransform transform;
	transform.position = obstacle->getPosition();

	float scale = Rand<float>(0.1, obstacle->getScale().x);
	transform.scale = { scale, scale };

	transform.rotation = Rand<int>(0, 360);

	Obstacle* obs = new Obstacle(transform, texture);

	obs->SetVelocity({normal.x, normal.y});
	obs->GetObstacle()->setTexture(*texture);
	Obstacles.push_back(obs);
}