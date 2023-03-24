#include "CollisionHandler.h"

#include "../Maths/MathUtils.h"

CollisionHandler::CollisionHandler()
{
}

bool CollisionHandler::BoundingBoxDetection(sf::Sprite *sprite1, sf::Sprite *sprite2)
{
	float minA = 0, maxA = 0, minB = 0, maxB = 0;

	std::vector<sf::Vector2f> vertices1, vertices2;
	GetVertices(sprite1, vertices1);
	GetVertices(sprite2, vertices2);
	ScaleVertices(vertices1, sprite1->getScale().x);

	for (int i = 0; i < vertices1.size(); i++)
	{
		sf::Vector2f edge = { vertices1[(i + 1) % vertices1.size()].x - vertices1[i].x,  vertices1[(i + 1) % vertices1.size()].y - vertices1[i].y };
		sf::Vector2f axis = { -edge.y, edge.x };

		Projection(vertices1, axis, minA, maxA);
		Projection(vertices2, axis, minB, maxB);

		if (minA >= maxB || minB >= maxA)
			return false;
	}

	for (int i = 0; i < vertices2.size(); i++)
	{
		sf::Vector2f edge = { vertices2[(i + 1) % vertices2.size()].x - vertices2[i].x,  vertices2[(i + 1) % vertices2.size()].y - vertices2[i].y };
		sf::Vector2f axis = { -edge.y, edge.x };

		Projection(vertices1, axis, minA, maxA);
		Projection(vertices2, axis, minB, maxB);

		if (minA >= maxB || minB >= maxA)
			return false;
	}

	return true;
}

void CollisionHandler::GetVertices(sf::Sprite* rect, std::vector<sf::Vector2f>& vertices)
{
	sf::FloatRect boxBound = rect->getLocalBounds();
	sf::Vector2f boxPosition = rect->getPosition();

	vertices.clear();
	vertices.push_back({ boxPosition.x - boxBound.width / 2, boxPosition.y - boxBound.height / 2 });
	vertices.push_back({ boxPosition.x - boxBound.width / 2, boxPosition.y + boxBound.height / 2 });
	vertices.push_back({ boxPosition.x + boxBound.width / 2, boxPosition.y + boxBound.height / 2 });
	vertices.push_back({ boxPosition.x + boxBound.width / 2, boxPosition.y - boxBound.height / 2 });

	if (rect->getRotation() != 0)
	{
		for (int i = 0; i < vertices.size(); i++)
		{
			vertices[i] = RotateAroundPoint(vertices[i], rect->getPosition(), DegreeToRadians(rect->getRotation()));
		}
	}
}

void CollisionHandler::Projection(std::vector<sf::Vector2f>& vertices, sf::Vector2f axis, float& min, float& max)
{
	max = FLT_MIN;
	min = FLT_MAX;

	for(int i=0; i<vertices.size(); i++)
	{
		float proj = Dot(vertices[i], axis);

		if (proj < min)
			min = proj;
		if (proj > max)
			max = proj;
	}
}

void CollisionHandler::ScaleVertices(std::vector<sf::Vector2f>& vertices, float scale)
{
	//std::cout << scale<<" ";
	sf::Vector2f midPoint = { (vertices[0].x + vertices[2].x) / 2, (vertices[0].y + vertices[2].y) / 2 };
	for(int i=0; i<vertices.size(); i++)
	{
		sf::Vector2f newPoint = { vertices[i].x - midPoint.x, vertices[i].y - midPoint.y };
		newPoint *= scale;
		vertices[i] = { newPoint.x + midPoint.x, newPoint.y + midPoint.y };
	}
}
