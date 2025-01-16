#pragma once

#include "Entity.h"
#include <SFML/System/Vector2.hpp>

class PlayerEntity;

class BallEntity : public Entity
{
public:
    void OnUpdate() override;
    void OnCollision(Entity* other) override;

    void ThrowAt(PlayerEntity* thrower, const sf::Vector2f& target, float speed);

    PlayerEntity* GetThrower() const { return mThrower; }

private:
    void CollideWithBoundaries();

    PlayerEntity* mThrower = nullptr;
    float mThrowTimer;
};
