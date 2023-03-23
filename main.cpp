#include <iostream>
#include <cstdlib>

#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>

#include "Utils/Maths/MathUtils.h"
#include "GameObjects/Obstacle.h"
#include "GameObjects/Projectile.h"
#include "Utils/Miscellaneous/CollisionHandler.h"

constexpr int Width = 800;
constexpr int Height = 800;

void SpriteWrap(sf::Sprite* sprite);
void DestroyObstacle(std::vector<Obstacle*>& Obstacles, sf::Sprite* obstacle,sf::Vector2f normal);

template <typename t>
float Rand(t min, t max);

int main()
{
	sf::RenderWindow window(sf::VideoMode(Width, Height), "Space Invader");
	ImGui::SFML::Init(window);

	window.setFramerateLimit(60);

	sf::Texture shipTexture;
	if (!shipTexture.loadFromFile("./Sprites/Ship.png"))
	{
		std::cout << "Ship Texture Loading Failed";
		return 0;
	}

	std::vector<Obstacle*> Obstacles;
	std::vector<Projectile*> Projectiles;
	sf::Vector2f ObstacleMaxVelocity{ 20, 20 };
	bool bSpacePressed{ false };

	for (int i = 0; i < 10; i++)
	{
		ObstacleTransform transform;
		transform.position = { (float)Rand<int>(0, Width), (float)Rand<int>(0, Height) };
		float scale = Rand<float>(0.5, 0.2);
		transform.scale = { scale, scale };
		transform.rotation = Rand<int>(0, 360);
		Obstacle* obs = new Obstacle(transform);
		obs->SetVelocity({ Rand<int>(-ObstacleMaxVelocity.x, ObstacleMaxVelocity.x), Rand<int>(-ObstacleMaxVelocity.y, ObstacleMaxVelocity.y) });
		Obstacles.push_back(obs);
	}

	shipTexture.setSmooth(true);
	sf::Sprite shipSprite(shipTexture);
	shipSprite.setOrigin(shipSprite.getLocalBounds().width/2 , shipSprite.getLocalBounds().height/2);
	shipSprite.setPosition(Width / 2, Height / 2);
	shipSprite.scale(0.5, 0.5);

	float shipSpeed = 100.0f;
	float shipRotationSpeed = 100.0f;
	sf::Vector2f shipDirection;
	sf::Vector2f velocity(0,0);
	sf::Vector2f momentum(0,0);
	float drag = 0.99;
	sf::Vector2f previousPosition;
	sf::Vector2f currentPosition;
	float maxVelocity = 20.0f;
	float bulletSpeed = 250.0f;

	sf::Clock deltaClock;
	while (window.isOpen())
	{
		sf::Event event;
		sf::Time deltaTime = deltaClock.restart();
		sf::Vector2f shipPosition = shipSprite.getPosition();
		float shipRotation = shipSprite.getRotation();

		previousPosition = currentPosition;
		currentPosition = shipSprite.getPosition();
		velocity.x = (currentPosition.x - previousPosition.x) / deltaTime.asSeconds();
		velocity.y = (currentPosition.y - previousPosition.y) / deltaTime.asSeconds();

		shipDirection.x = (float)cos(DegreeToRadians(shipSprite.getRotation() - 90.0f));
		shipDirection.y = (float)sin(DegreeToRadians(shipSprite.getRotation() - 90.0f));
		Normalize(shipDirection);

		while(window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
				window.close();

			if(event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Space && !bSpacePressed)
				{
					bSpacePressed = true;
					sf::Vector2f shipTip = { shipSprite.getPosition().x, shipSprite.getPosition().y - shipSprite.getGlobalBounds().height / 2 + 5.0f };
					Projectile* pro = new Projectile(RotateAroundPoint(shipTip, shipSprite.getPosition(), DegreeToRadians(shipSprite.getRotation())), shipSprite.getRotation());
					pro->SetVelocity({ shipDirection.x * bulletSpeed, shipDirection.y * bulletSpeed });
					Projectiles.push_back(pro);
				}
			}

			if(event.type == sf::Event::KeyReleased)
			{
				if(event.key.code == sf::Keyboard::Space)
				{
					bSpacePressed = false;
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			shipRotation -= shipRotationSpeed * deltaTime.asSeconds();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			shipRotation += shipRotationSpeed * deltaTime.asSeconds();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			momentum.x += shipDirection.x * shipSpeed * deltaTime.asSeconds();
			momentum.y += shipDirection.y * shipSpeed * deltaTime.asSeconds();
			
			if(VectorMangnitude(velocity) > maxVelocity)
			{
				momentum.x *= drag;
				momentum.y *= drag;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			if (VectorMangnitude(velocity) < maxVelocity)
			{
				momentum.x -= shipDirection.x * shipSpeed * deltaTime.asSeconds();
				momentum.y -= shipDirection.y * shipSpeed * deltaTime.asSeconds();
			}

			if (VectorMangnitude(velocity) > maxVelocity)
			{
				momentum.x *= drag;
				momentum.y *= drag;
			}
		}
		else
		{
			momentum.x *= drag;
			momentum.y *= drag;
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

		shipPosition.x += momentum.x*deltaTime.asSeconds();
		shipPosition.y += momentum.y*deltaTime.asSeconds();

		shipSprite.setPosition(shipPosition);
		shipSprite.setRotation(shipRotation);

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
							if (collisionHandler.BoundingBoxDetection(obs->GetObstacle(), pro->GetProjectileSprite()))
							{
								std::cout << "Collision Detected " << std::endl;
								pro->SetCollision(false);
								pro->SetVisibility(false);
								obs->SetVisibility(false);
								obs->SetCollision(false);

								sf::Vector2f edge = { pro->GetProjectileSprite()->getPosition().x - obs->GetObstacle()->getPosition().x,
									pro->GetProjectileSprite()->getPosition().y - obs->GetObstacle()->getPosition().y };
								sf::Vector2f axis1 = { -edge.y, edge.x };
								sf::Vector2f axis2 = { edge.y, -edge.x };

								DestroyObstacle(Obstacles, obs->GetObstacle() ,axis1);
								DestroyObstacle(Obstacles, obs->GetObstacle(),axis2);
								break;
							}
						}
					}
				}
			}
		}


		SpriteWrap(&shipSprite);

		ImGui::SFML::Update(window, deltaTime);

		ImGui::Begin("Window Title");
		ImGui::InputFloat("Ship Speed", &shipSpeed);
		ImGui::InputFloat("Ship Rotation Speed", &shipRotationSpeed);
		ImGui::InputFloat("Drag", &drag);
		ImGui::InputFloat("Max Velocity", &maxVelocity);
		ImGui::InputFloat("Bullet Speed", &bulletSpeed);
		ImGui::End();

		window.clear(sf::Color(18,33,43));

		window.draw(shipSprite);

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
				window.draw(*pro->GetProjectileSprite());
			}
		}

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

void DestroyObstacle(std::vector<Obstacle*>& Obstacles, sf::Sprite* obstacle, sf::Vector2f normal)
{
	if (obstacle->getScale().x <= 0.15)
		return;

	ObstacleTransform transform;
	transform.position = obstacle->getPosition();

	float scale = Rand<float>(0.1, obstacle->getScale().x);
	transform.scale = { scale, scale };

	transform.rotation = Rand<int>(0, 360);

	Obstacle* obs = new Obstacle(transform);
	obs->SetVelocity({normal.x, normal.y});
	Obstacles.push_back(obs);
}