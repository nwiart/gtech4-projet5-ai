#pragma once

#include "Entity.h"

class BallEntity : public Entity
{
public:

	void OnUpdate() override;
	void OnCollision(Entity* other) override;

private:

	void CollideWithBoundaries();
};
