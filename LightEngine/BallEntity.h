#pragma once

#include "Entity.h"
#include <SFML/System/Vector2.hpp>

class BallEntity : public Entity
{
public:
    void OnUpdate() override;
    void OnCollision(Entity* other) override;

    void MoveTo(sf::Vector2f target, float speed);
    bool IsMoving() const;
    void SetIgnoreCollision(Entity* entity);
    void DisableRigidBodyTemporarily(float duration);

private:
    void CollideWithBoundaries();

    sf::Vector2f targetPosition;
    float movementSpeed = 0.0f;
    bool isMoving = false;
    Entity* ignoredEntity = nullptr;
    bool rigidBodyDisabled = false;
    float rigidBodyDisableTimer = 0.0f;
};
