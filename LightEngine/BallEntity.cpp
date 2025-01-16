#include "BallEntity.h"

#include "SampleScene.h"
#include "PlayerEntity.h"

void BallEntity::OnUpdate()
{
	float deltaTime = GetDeltaTime();

	if (mThrowTimer > 0.0F) {
		mThrowTimer -= deltaTime;
		if (mThrowTimer <= 0.0F) {
			mThrower = 0;
		}
	}

	CollideWithBoundaries();

	SetSpeed(GetSpeed() * 0.98F);
}

void BallEntity::OnCollision(Entity* other)
{
	PlayerEntity* player = dynamic_cast<PlayerEntity*>(other);
	if (!player) return;

	if (mThrower != player && !player->HasBall()) {
		GetScene<SampleScene>()->SetBallHolder(player);
	}
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

void BallEntity::ThrowAt(PlayerEntity* thrower, const sf::Vector2f& target, float speed)
{
	sf::Vector2f dir = target - this->GetPosition();
	float len = sqrt(dir.x * dir.x + dir.y * dir.y);
	dir.x /= len;
	dir.y /= len;

	SetDirection(dir.x, dir.y, speed);

	mThrower = thrower;
	mThrowTimer = 1.0F;
}
