#include "Entity.h"

#include "GameManager.h"
#include "Utils.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>

Entity::Entity(float radius, const sf::Color& color)
{
	mDirection = sf::Vector2f(0.0f, 0.0f);
	mSpeed = 0.0f;
	mToDestroy = false;
	mTag = -1;

	mShape.setOrigin(0.f, 0.f);
	mShape.setRadius(radius);
	mShape.setFillColor(color);
}

bool Entity::IsColliding(Entity* other) const
{
	sf::Vector2f distance = GetPosition(0.5f, 0.5f) - other->GetPosition(0.5f, 0.5f);

	float sqrLength = (distance.x * distance.x) + (distance.y * distance.y);

	float radius1 = mShape.getRadius();
	float radius2 = other->mShape.getRadius();

	float sqrRadius = (radius1 + radius2) * (radius1 + radius2);

	return sqrLength < sqrRadius;
}

bool Entity::IsInside(float x, float y) const
{
	sf::Vector2f position = GetPosition(0.5f, 0.5f);

	float dx = x - position.x;
	float dy = y - position.y;

	float radius = mShape.getRadius();

	return (dx * dx + dy * dy) < (radius * radius);
}

void Entity::SetPosition(float x, float y, float ratioX, float ratioY)
{
	float size = mShape.getRadius() * 2;

	x -= size * ratioX;
	y -= size * ratioY;

	mShape.setPosition(x, y);
}

sf::Vector2f Entity::GetPosition(float ratioX, float ratioY) const
{
	float size = mShape.getRadius() * 2;
	sf::Vector2f position = mShape.getPosition();

	position.x += size * ratioX;
	position.y += size * ratioY;

	return position;
}

void Entity::GoToDirection(float x, float y, float speed)
{
	if(speed > 0)
		mSpeed = speed;

	sf::Vector2f position = GetPosition(0.5f, 0.5f);
	mDirection = sf::Vector2f(x - position.x, y - position.y);
	Utils::Normalize(mDirection);

	bool success = Utils::Normalize(mDirection);

	_ASSERT(success);
}

void Entity::Update()
{
	OnUpdate();

	float dt = GameManager::Get()->GetDeltaTime();

	sf::Vector2f translation;

	translation.x += dt * mSpeed * mDirection.x;
	translation.y += dt * mSpeed * mDirection.y;

	mShape.move(translation);
}

Scene* Entity::GetScene() const
{
	return GameManager::Get()->GetScene();
}
