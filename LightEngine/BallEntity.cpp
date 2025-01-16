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

	SampleScene::CollideWithBoundaries(this);

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
