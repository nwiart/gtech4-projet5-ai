#pragma once

#include "Scene.h"

class PlayerEntity;
class BallEntity;

class DummyEntity;

class SampleScene : public Scene
{
	Entity* pEntitySelected;

private:
	void TrySetSelectedEntity(DummyEntity* pEntity, int x, int y);

public:
	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;
};


