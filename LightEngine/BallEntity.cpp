#include "BallEntity.h"

#include "SampleScene.h"
#include "PlayerEntity.h"

void BallEntity::OnUpdate()
{
	float deltaTime = GetDeltaTime();

	if (rigidBodyDisabled)
	{
		rigidBodyDisableTimer -= deltaTime;
		if (rigidBodyDisableTimer <= 0.0f)
		{
			rigidBodyDisabled = false;
			this->SetRigidBody(true);
		}
	}

	if (isMoving)
	{
		sf::Vector2f currentPosition = GetPosition();
		sf::Vector2f direction = targetPosition - currentPosition;
		float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

		if (distance > 0.1f)
		{
			direction /= distance;
			sf::Vector2f velocity = direction * movementSpeed * GetDeltaTime();
			if (std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y) >= distance)
			{
				SetPosition(targetPosition.x, targetPosition.y);
				isMoving = false;
			}
			else
			{
				SetPosition(currentPosition.x + velocity.x, currentPosition.y + velocity.y);
			}
		}
		else
		{
			SetPosition(targetPosition.x, targetPosition.y);
			isMoving = false;
		}
	}

	CollideWithBoundaries();

	SetSpeed(GetSpeed() * 0.98F);
}

void BallEntity::OnCollision(Entity* other)
{
	if (rigidBodyDisabled)
		return;
	PlayerEntity* player = dynamic_cast<PlayerEntity*>(other);
	if (!player) return;

	// TODO : possession here.
}

void BallEntity::CollideWithBoundaries()
{
	sf::Vector2f p0 = GetPosition(0.0f, 0.0f);
	sf::Vector2f p1 = GetPosition(1.0f, 1.0f);
	sf::Vector2f off;
	p1 -= sf::Vector2f(GetScene()->GetWindowWidth(), GetScene()->GetWindowHeight());

	if (p0.x < 0.0F) off.x = p0.x;
	if (p1.x > 0.0F) off.x = p1.x;
	if (p0.y < 0.0F) off.y = p0.y;
	if (p1.y > 0.0F) off.y = p1.y;

	if (off.x != 0.0F || off.y != 0.0F) {
		SetPosition(p0.x - off.x, p0.y - off.y, 0, 0);

		sf::Vector2f dir = GetDirection();
		if (off.x != 0.0F) {
			dir.x = -dir.x;
		}
		if (off.y != 0.0F) {
			dir.y = -dir.y;
		}
		SetDirection(dir.x, dir.y);
	}
}

void BallEntity::MoveTo(sf::Vector2f target, float speed)
{
	targetPosition = target;
	movementSpeed = speed;
	isMoving = true;
}

bool BallEntity::IsMoving() const
{
	return isMoving;
}

void BallEntity::SetIgnoreCollision(Entity* entity)
{
	ignoredEntity = entity;
}

void BallEntity::DisableRigidBodyTemporarily(float duration)
{
	rigidBodyDisabled = true;
	rigidBodyDisableTimer = duration;
	this->SetRigidBody(false);
}
